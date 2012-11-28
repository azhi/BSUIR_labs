#!/usr/bin/env ruby

require 'ruby_parser'
require 'ruby-graphviz'
require 'pry'

$graphs = []
$defined_functions = []
$cheppin = 0
$mayers = nil
$variable_collection = []
$cheppin_counts = {calc: 0, io: 0, control: 0, unused: 0}
$w = 0
$e = 0

def init_graph
  graph = GraphViz.new( :G, :type => :digraph )
  graph.node[:color]    = "#ddaa66"
  graph.node[:style]    = "filled"
  graph.node[:shape]    = "box"
  graph.node[:penwidth] = "1"
  graph.node[:fontname] = "Trebuchet MS"
  graph.node[:fontsize] = "8"
  graph.node[:fillcolor]= "#ffeecc"
  graph.node[:fontcolor]= "#775500"
  graph.node[:margin]   = "0.0"

  graph.edge[:color]    = "#999999"
  graph.edge[:weight]   = "1"
  graph.edge[:fontsize] = "6"
  graph.edge[:fontcolor]= "#444444"
  graph.edge[:fontname] = "Verdana"
  graph.edge[:dir]      = "forward"
  graph.edge[:arrowsize]= "0.5"

  $graphs << graph
  return {graph: graph, last_node_id: 0}
end

def add_node graph, tailes, str
  node = graph[:graph].add_nodes "#{graph[:last_node_id]} - #{str}"
  graph[:last_node_id] += 1
  tailes.each{ |tail| graph[:graph].add_edges tail, node }
  [node]
end

def init_var name, types, ruby_type
  # :type - :calc, :io, :control, :unused
  # :ruby_type - :local, :global, :instance, :const

  m_types = {calc: false, io: false, control: false, unused: false}
  types.each do |type|
    raise "Unrecognized type #{type.inspect}" unless [:calc, :io, :control, :unused].include?(type)
    m_types[type] = true
  end

  unless [:local, :global, :instance, :const].include?(ruby_type)
    case ruby_type
    when :lvar, :lasgn
      ruby_type = :local
    when :gvar, :gasgn
      ruby_type = :global
    when :ivar, :iasgn
      ruby_type = :instance
    when :cvar, :cvdecl
      ruby_type = :const
    else
      raise "Unrecognized ruby_type #{ruby_type.inspect}"
    end
  end
  {name: name, type: m_types, ruby_type: ruby_type, spen: 0}
end

def find_var var_id_collection, name
  var_id_collection.find{ |id| $variable_collection[id][:name] == name }
end

def process_args sexp, ext_vars, int_vars, level, res_hash
  sexp[2][1..-1].each do |sexp|
    if sexp.is_a?(Symbol)
      $variable_collection << init_var(sexp, {}, :local)
      int_vars << $variable_collection.size - 1
    elsif sexp.is_a?(Sexp)
      sub_hash = go_deeper! sexp, ext_vars, int_vars, level+1
    else
      raise "Unrecognized args type #{sexp.inspect}"
    end
  end
end

def process_var sexp, ext_vars, int_vars, level, res_hash
  # Name:     sexp[1]
  puts ' ' * level + "Var: name=#{sexp[1].inspect}"
  [int_vars, ext_vars.reverse].each do |var_collection|
    var = find_var var_collection, sexp[1]
    unless var.nil?
      res_hash[:vars] << var
      $variable_collection[var][:spen] += 1
      return res_hash 
    end
  end
  $variable_collection << init_var(sexp[1], [], sexp[0])
  res_hash[:vars] << $variable_collection.size - 1
  return res_hash
end

def process_call sexp, ext_vars, int_vars, level, res_hash, cur_graph, tailes
  # Object:   sexp[1]          may be an expression
  # Name:     sexp[2]
  # Args:     sexp[3..-1]
  puts ' ' * level + "Call: obj=#{sexp[1].inspect} name=#{sexp[2].inspect} args=#{sexp[3..-1].inspect}"
  defined = $defined_functions.include?("#{sexp[1]}.#{sexp[2]}")
  # binding.pry if !sexp[1].nil? && sexp[1][0] == :iter
  res_hash[:tailes] = add_node cur_graph, tailes, "#{sexp[1]}.#{sexp[2]}" if defined
  obj_hash = {}
  obj_hash[:tailes] = tailes
  obj_hash = go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes unless sexp[1].nil?
  obj_var_ids = []
  obj_var_ids += obj_hash[:vars] unless obj_hash.nil? || obj_hash[:vars].nil?

  # binding.pry if !sexp[1].nil? && sexp[1][0] == :iter
  sub_expr_var_ids = []
  iter_tailes = defined ? res_hash[:tailes] : obj_hash[:tailes]
  sexp[3..-1].each do |sexp|
    sub_hash = go_deeper! sexp, ext_vars, int_vars, level+1, cur_graph, iter_tailes
    sub_expr_var_ids += sub_hash[:vars] unless sub_hash.nil? || sub_hash[:vars].nil?
    iter_tailes = sub_hash[:tailes] unless defined
    res_hash[:tailes] = sub_hash[:tailes] unless defined
  end
  # binding.pry if !sexp[1].nil? && sexp[1][0] == :iter

  obj_var_ids.each do |var_id|
    obj_hash[:vars].first
    $variable_collection[var_id][:type][:calc] = true if [:+, :-, :*, :/, :%].include?(sexp[2])
    $variable_collection[var_id][:type][:control] = true if %w[:== :!=
    :<= :>= :< :> :! :&& :||].include?(sexp[2]) || sexp[2].to_s[-1] == '?'
  end

  sub_expr_var_ids.each do |var_id|
    case sexp[2]
    when :+, :-, :*, :/, :%, :[]
      $variable_collection[var_id][:type][:calc] = true
    when :puts, :putc, :print, :printf, :get_byte, :getc, :getch, :gets
      $variable_collection[var_id][:type][:io] = true
    when :==, :!=, :<=, :>=, :<, :>, :!
      $variable_collection[var_id][:type][:control] = true
    end
  end

  res_hash[:vars] = obj_var_ids + sub_expr_var_ids
  return res_hash
end

def process_defn sexp, ext_vars, int_vars, level, res_hash, cur_graph, tailes
  # Name:     sexp[1]
  # Args:     sexp[2]          s(:args, *arg_names, s(:block, *asgns))
  # Body:     sexp[3]          *blocks
  puts ' ' * level + "Defn: name=#{sexp[1].inspect} args=#{sexp[2].inspect} body=#{sexp[3..-1].inspect}"
  $defined_functions << ".#{sexp[1]}"
  sub_graph = init_graph
  sub_graph[:last_node_id] += 1
  tailes = [sub_graph[:graph].add_nodes("Begin #{sexp[1]}")]
  process_args sexp, ext_vars, int_vars, level, res_hash
  sexp[3..-1].each do |sexp|
    sub_hash = go_deeper! sexp, ext_vars, int_vars, level+1, sub_graph, tailes
    tailes = sub_hash[:tailes]
    int_vars += sub_hash[:vars] unless sub_hash.nil? || sub_hash[:vars].nil?
  end
  ending = sub_graph[:graph].add_nodes "End"
  tailes.each{ |tail| sub_graph[:graph].add_edges tail, ending }
  
  return res_hash
end

def process_iterator sexp, ext_vars, int_vars, level, res_hash, cur_graph, tailes
  sexp[2][1..-1].each do |sexp|
    if sexp.is_a?(Symbol)
      $variable_collection << init_var(sexp, {}, :local)
      int_vars << $variable_collection.size - 1
    else
      raise "Unrecognized args type #{sexp.inspect}"
    end
  end

  iter_tail = add_node cur_graph, tailes, "iter"
  sub_hash = go_deeper! sexp[3], ext_vars, int_vars, level+1, cur_graph, iter_tail

  end_tail = []
  if sub_hash[:tailes] == iter_tail
    end_tail = add_node cur_graph, iter_tail, "iter block"
  else
    end_tail = sub_hash[:tailes]
  end
  
  end_tail.each{ |tail| cur_graph[:graph].add_edges tail, iter_tail }

  res_hash[:tailes] = iter_tail
  return res_hash
end

def process_assignment sexp, ext_vars, int_vars, level, res_hash, cur_graph, tailes
  # Name:     sexp[1]
  # Value:    sexp[2]
  puts ' ' * level + "Assign: name=#{sexp[1].inspect} value=#{sexp[2].inspect}"
  found = false
  [int_vars, ext_vars.reverse].each do |var_collection|
    var = find_var var_collection, sexp[1]
    unless var.nil?
      res_hash[:vars] << var
      found = true
      $variable_collection[var][:spen] += 1
      break
    end
  end
  unless found
    $variable_collection << init_var(sexp[1], [], sexp[0])
    res_hash[:vars] << $variable_collection.size - 1
    int_vars << $variable_collection.size - 1
  end
  sub_hash = go_deeper! sexp[2], ext_vars, int_vars, level+1, cur_graph, tailes
  res_hash[:tailes] = sub_hash[:tailes]
  return res_hash
end

def go_deeper! sexp = nil, ext_vars = [], int_vars = [], level = 0, cur_graph = nil, tailes = []
  # returns hash with possible parametres:
  # :vars - Array of variable ids

  res_hash = {vars: [], tailes: tailes}
  return res_hash unless sexp
  case sexp.sexp_type
  when nil
  when :const
    # Name:     sexp[1]
    puts ' ' * level + "Const: name=#{sexp[1].inspect}"
  when :hash, :array
    # Elements: sexp.sexp_body   ungrouped
    puts ' ' * level + "Container: el=#{sexp.sexp_body}"
    sexp.sexp_body.each{ |sexp| go_deeper! sexp, ext_vars, int_vars, level+1, cur_graph, tailes }
  when :lit                      # number/sumbol/etc.
    # Value:    sexp[1]          s(:lit, 1) | s(:lit, :sym)
    puts ' ' * level + "Literal: val=#{sexp[1].inspect}"
  when :array
    puts ' ' * level + "Container: el=#{sexp.sexp_body}"
    sexp.sexp_body.each{ |sexp| go_deeper! sexp, ext_vars, int_vars, level+1, cur_graph, tailes }
  when :dot2                     # range
    # From, to  sexp[1,2]
    go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes
    go_deeper! sexp[2], ext_vars, int_vars, level+1, cur_graph, tailes
  when :str
    # Value:    sexp[1]          s(:str, "string")
    puts ' ' * level + "String: val=#{sexp[1].inspect}"
  when :dstr
    # Init_val  sexp[1]
    # Concated  sexp[2..-1]      s(:evstr, expression) | s(:str "string")
    sexp[2..-1].each do |sexp|
      if sexp.sexp_type == :evstr
        go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes
      else
        go_deeper! sexp, ext_vars, int_vars, level+1, cur_graph, tailes
      end
    end
  when :lvar, :gvar, :ivar, :cvar
    process_var sexp, ext_vars, int_vars, level, res_hash
  when :and, :or
    # Args:     sexp[1..2]
    puts ' ' * level + "Logic op: name=#{sexp[1..2].inspect}"
    sub_hash = go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes
    sub_hash[:vars].each do |var_id|
      $variable_collection[var_id][:type][:control] = true
    end unless sub_hash.nil? || sub_hash[:vars].nil?
    sub_hash = go_deeper! sexp[2], ext_vars, int_vars, level+1, cur_graph, tailes
    sub_hash[:vars].each do |var_id|
      $variable_collection[var_id][:type][:control] = true
    end unless sub_hash.nil? || sub_hash[:vars].nil?
  when :op_asgn_and, :op_asgn_or
    # Cond:     sexp[1]          s(:?var, :name)
    # Asgn:     sexp[2]          s(:?asgn/:cvdecl, :name, :value)
    puts ' ' * level + "Cond assignment: cond=#{sexp[1].inspect} asgn=#{sexp[2].inspect}"
    go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes
    go_deeper! sexp[2], ext_vars, int_vars, level+1, cur_graph, tailes
  when :call
    sub_hash = process_call sexp, ext_vars, int_vars, level, res_hash, cur_graph, tailes
    res_hash[:tailes] = sub_hash[:tailes]
  when :iter
    # Object:   sexp[1]          Or s(:call, nil, :lambda)
    # Locals:   sexp[2]          s(:args, *names) | s(:args)
    # Body:     sexp[3]          block / expression
    puts ' ' * level + "Iterator: obj=#{sexp[1].inspect} locals=#{sexp[2].inspect} body=#{sexp[3].inspect}"
    sub_hash = go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes unless sexp[1] == s(:call, nil, :lambda)
    sub_hash = process_iterator sexp, ext_vars + int_vars, [], level, res_hash, cur_graph, sub_hash[:tailes]
    res_hash[:tailes] = sub_hash[:tailes]
  when :if                       # and unless
    # Cond:     sexp[1]
    # Then:     sexp[2]          block / expression
    # Else:     sexp[3]          block / expression
    puts ' ' * level + "Condition: cond=#{sexp[1].inspect} then=#{sexp[2].inspect} else=#{sexp[3].inspect}"
    sub_hash = go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes
    sub_hash[:vars].each do |var_id|
      $variable_collection[var_id][:type][:control] = true
    end unless sub_hash.nil? || sub_hash[:vars].nil?

    if_tail = add_node cur_graph, tailes, "if"
    if_tail.freeze
    then_tail = []
    else_tail = []
    unless sexp[2].nil?
      then_hash = go_deeper! sexp[2], ext_vars, int_vars, level+1, cur_graph, if_tail
      if then_hash[:tailes] == if_tail
        then_tail = add_node cur_graph, if_tail, "then block"
      else
        then_tail = then_hash[:tailes]
      end
    else
      then_tail = if_tail
    end
    unless sexp[3].nil?
      else_hash = go_deeper! sexp[3], ext_vars, int_vars, level+1, cur_graph, if_tail
      if else_hash[:tailes] == if_tail
        else_tail = add_node cur_graph, if_tail, "else block"
      else
        else_tail = else_hash[:tailes]
      end
    else
      else_tail = if_tail
    end
    res_hash[:tailes] = then_tail + else_tail
  when :case
    # Cond:     sexp[1]
    # When:     sexp[2..-2]      s(:when, s(:array, expression)),
    #                              *sexps
    # Else:     sexp[-1]         block / expression

    case_tail = add_node cur_graph, tailes, "case"
    when_tailes = []
    sub_hash = go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes
    sub_hash[:vars].each do |var_id|
      $variable_collection[var_id][:type][:control] = true
    end unless sub_hash.nil? || sub_hash[:vars].nil?
    sexp[2..-2].each do |sexp|
      when_tail = add_node cur_graph, case_tail, "when"
      go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes
      sexp[2..-1].each do |sexp|
        sub_hash = go_deeper! sexp, ext_vars, int_vars, level+1, cur_graph, when_tail
        when_tail = sub_hash[:tailes]
      end
      when_tailes += when_tail
    end

    else_tail = []
    unless sexp[-1].nil?
      else_tail = add_node cur_graph, case_tail, "else"
      sub_hash = go_deeper! sexp[-1], ext_vars, int_vars, level+1, cur_graph, else_tail
      else_tail = sub_hash[:tailes]
    end

    res_hash[:tailes] = when_tailes + else_tail
  when :for
    # Where:    sexp[1]          array / range
    # What:     sexp[2]          s(:lasgn, :name)
    # Body:     sexp[3]          block / expression
    puts ' ' * level + "For: where=#{sexp[1].inspect} what=#{sexp[2].inspect} body=#{sexp[3].inspect}"
    sub_hash = go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes
    sub_hash[:vars].each do |var_id|
      $variable_collection[var_id][:type][:control] = true
    end unless sub_hash.nil? || sub_hash[:vars].nil?
    go_deeper! sexp[2], ext_vars, int_vars, level+1, cur_graph, tailes

    for_tail = add_node cur_graph, tailes, "for"
    sub_hash = go_deeper! sexp[3], ext_vars, int_vars, level+1, cur_graph, tailes
    end_tail = []
    if sub_hash[:tailes] == for_tail
      end_tail = add_node cur_graph, for_tail, "for block"
    else
      end_tail = sub_hash[:tailes]
    end
    cur_graph[:graph].add_edges end_tail.first, for_tail

    res_hash[:tailes] = end_tail
  when :while, :until
    # Control:  sexp[1]
    # Body:     sexp[2]
    # ???:      sexp[3]          Always true
    puts ' ' * level + "Wh|unt loop: control=#{sexp[1].inspect} body=#{sexp[2].inspect}"
    sub_hash = go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes
    sub_hash[:vars].each do |var_id|
      $variable_collection[var_id][:type][:control] = true
    end unless sub_hash.nil? || sub_hash[:vars].nil?
    go_deeper! sexp[2], ext_vars, int_vars, level+1, cur_graph, tailes
  when :attrasgn
    # Object: sexp[1]
    # :[]=    sexp[2] (Always)
    # arg     sexp[3]
    # val     sexp[4]
    puts ' ' * level + "Attribute assign: obj=#{sexp[1].inspect} arg=#{sexp[3].inspect} val=#{sexp[4].inspect}"
    go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes
    go_deeper! sexp[3], ext_vars, int_vars, level+1, cur_graph, tailes
    sub_hash = go_deeper! sexp[4], ext_vars, int_vars, level+1, cur_graph, tailes
    res_hash[:tailes] = sub_hash[:tailes]
  when :defn
    sub_hash = process_defn sexp, ext_vars + int_vars, [], level, res_hash, cur_graph, tailes
    res_hash[:tailes] = sub_hash[:tailes]
  when :gasgn, :lasgn, :iasgn, :cvdecl
    sub_hash = process_assignment sexp, ext_vars, int_vars, level, res_hash, cur_graph, tailes
    res_hash[:tailes] = sub_hash[:tailes]
  when :op_asgn1
    # Object: sexp[1]
    # Arglist sexp[2]
    # Method  sexp[3]
    # Value   sexp[4]
    puts ' ' * level + "Op_asgn: obj=#{sexp[1].inspect} args=#{sexp[2].inspect} method=#{sexp[3].inspect} value=#{sexp[4].inspect}"
    go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes unless sexp[1].nil?
    sexp[2][1..-1].each{ |sexp| go_deeper! sexp, ext_vars, int_vars, level+1 }
    sub_hash = go_deeper! sexp[4], ext_vars, int_vars, level+1, cur_graph, tailes
    res_hash[:tailes] = sub_hash[:tailes]
  when :return
    # Returning expr  sexp[1]
    puts ' ' * level + "return " + sexp[1].inspect
    go_deeper! sexp[1], ext_vars, int_vars, level+1, cur_graph, tailes unless sexp[1].nil?
  when :break
    # No params
    puts ' ' * level + "break"
  when :block
    sub_hash = {}
    sub_hash[:tailes] = tailes.dup
    sexp.sexp_body.each do |s|
      sub_hash = go_deeper! s, ext_vars, int_vars, level+1, cur_graph, sub_hash[:tailes]
      res_hash[:tailes] = sub_hash[:tailes]
    end
  when :false
  when :true
  when :nil
  else
    puts ' ' * level + 'Error: can not parse sexp ' + sexp.inspect
  end
  return res_hash
end

graph = init_graph 
graph[:last_node_id] += 1
tailes = [graph[:graph].add_nodes("Begin main")]
sexp = Ruby19Parser.new.parse(File.open(__FILE__){ |f| f.read })
res_hash = go_deeper! sexp, [], [], 0, graph, tailes
ending = graph[:graph].add_nodes "End"
res_hash[:tailes].each{ |tail| graph[:graph].add_edges tail, ending }


unused_vars = $variable_collection.find_all{ |var| !var[:type][:calc] && !var[:type][:io] && !var[:type][:control] }
unused_vars.each{ |var| var[:type][:unused] = true }
puts "Variables:"
$variable_collection.each do |var|
  types_ar = []
  var[:type].each{ |k, v| types_ar << k.to_s if v }
  printf "%-30s %-30s %-15s\n", "Name=#{var[:name]}", "types=#{types_ar.join('/')}", "spen=#{var[:spen]}"
end
puts

$variable_collection.each do |var|
  $cheppin += 3 if var[:type][:control]
  $cheppin += 2 if var[:type][:calc]
  $cheppin += 1 if var[:type][:io]
  $cheppin += 0.5 if var[:type][:unused]
end
puts "Cheppin: #$cheppin"

puts "Mayers:  #$mayers"

$graphs.each.with_index{ |graph, i| graph.output( :png => "graph#{i}.png" ) }
# puts "DEBUG: " + Ruby19Parser.new.parse(File.open(__FILE__){ |f| f.read }).inspect
