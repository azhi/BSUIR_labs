#!/usr/bin/env ruby

require 'ruby_parser'
require 'pry'

$mayers = nil
$cheppin = nil


$spen = 0
$w = 0
$e = 0
$c = [0] * 4

def go_deeper! sexp = nil, vars = {}, level = 0
  return unless sexp
  case sexp.sexp_type
  when nil
    return
  # TODO: strings with insertions ("#{}", etc.)
  when :const
    puts ' ' * level + "Const: name=#{sexp[1].inspect}"
    # Name:     sexp[1]
  when :hash, :array
    puts ' ' * level + "Container: el=#{sexp.sexp_body}"
    sexp.sexp_body.each{ |sexp| go_deeper! sexp, {}, level+1 }
    # Elements: sexp.sexp_body   ungrouped
  when :lit                      # number/sumbol/etc.
    puts ' ' * level + "Literal: val=#{sexp[1].inspect}"
    # Value:    sexp[1]          s(:lit, 1) | s(:lit, :sym)
  when :str
    puts ' ' * level + "String: val=#{sexp[1].inspect}"
    # Value:    sexp[1]          s(:str, "string")

  when :lvar, :gvar, :ivar, :cvar
    puts ' ' * level + "Var: name=#{sexp[1].inspect}"
    # Name:     sexp[1]
  when :and, :or
    puts ' ' * level + "Logic op: name=#{sexp[1..2].inspect}"
    go_deeper! sexp[1], {}, level+1
    go_deeper! sexp[2], {}, level+1
    # Args:     sexp[1..2]
  when :op_asgn_and, :op_asgn_or
    puts ' ' * level + "Cond assignment: cond=#{sexp[1].inspect} asgn=#{sexp2.inspect}"
    go_deeper! sexp[1], {}, level+1
    go_deeper! sexp[2], {}, level+1
    # Cond:     sexp[1]          s(:?var, :name)
    # Asgn:     sexp[2]          s(:?asgn/:cvdecl, :name, :value)
  when :call
    puts ' ' * level + "Call: obj=#{sexp[1].inspect} name=#{sexp[2].inspect} args=#{sexp[3..-1].inspect}"
    go_deeper! sexp[1], {}, level+1 unless sexp[1].nil?
    sexp[3..-1].each{ |sexp| go_deeper! sexp, {}, level+1 }
    # Object:   sexp[1]          may be an expression
    # Name:     sexp[2]
    # Args:     sexp[3..-1]
  when :args
    puts ' ' * level + "Args: el=#{sexp.sexp_body}"
    sexp.sexp_body.each do |el|
      if el.is_a?(Sexp)
        go_deeper! el, {}, level+1 
      else
        # process argnames here
      end
    end
  when :iter
    puts ' ' * level + "Iterator: obj=#{sexp[1].inspect} locals=#{sexp[2].inspect} body=#{sexp[3].inspect}"
    go_deeper! sexp[1], {}, level+1
    go_deeper! sexp[2], {}, level+1
    go_deeper! sexp[3], {}, level+1
    # Object:   sexp[1]          Or s(:call, nil, :lambda)
    # Locals:   sexp[2]          s(:args, *names) | s(:args)
    # Body:     sexp[3]          block / expression

  when :if                       # and unless
    puts ' ' * level + "Condition: cond=#{sexp[1].inspect} then=#{sexp[2].inspect} else=#{sexp[3].inspect}"
    go_deeper! sexp[1], {}, level+1
    go_deeper! sexp[2], {}, level+1
    go_deeper! sexp[3], {}, level+1
    # Cond:     sexp[1]
    # Then:     sexp[2]          block / expression
    # Else:     sexp[3]          block / expression

  when :for
    puts ' ' * level + "For: where=#{sexp[1].inspect} what=#{sexp[2].inspect} body=#{sexp[3].inspect}"
    go_deeper! sexp[1], {}, level+1
    go_deeper! sexp[2], {}, level+1
    go_deeper! sexp[3], {}, level+1
    # Where:    sexp[1]          array / range
    # What:     sexp[2]          s(:lasgn, :name)
    # Body:     sexp[3]          block / expression
  when :while, :until
    puts ' ' * level + "Wh|unt loop: control=#{sexp[1].inspect} body=#{sexp[2].inspect}"
    go_deeper! sexp[1], {}, level+1
    go_deeper! sexp[2], {}, level+1
    go_deeper! sexp[3], {}, level+1
    # Control:  sexp[1]
    # Body:     sexp[2]
    # ???:      sexp[3]          Always true

  when :defn
    puts ' ' * level + "Defn: name=#{sexp[1].inspect} args=#{sexp[2].inspect} body=#{sexp[3].inspect}"
    go_deeper! sexp[2], {}, level+1
    sexp[3..-1].each{ |sexp| go_deeper! sexp, {}, level+1 }
    
    # Name:     sexp[1]
    # Args:     sexp[2]          s(:args, *arg_names, s(:block, *asgns))
    # Body:     sexp[3]          s(:scope, s(:block, *))
  when :gasgn, :lasgn, :iasgn, :cvdecl
    puts ' ' * level + "Assign: name=#{sexp[1].inspect} value=#{sexp[2].inspect}"
    go_deeper! sexp[2], {}, level+1
    # Name:     sexp[1]
    # Value:    sexp[2]

  when :return
    puts ' ' * level + "return"
    # No params

  when :nil

  when :block
    sexp.sexp_body.each{ |s| go_deeper! s, {}, level+1 }

  else
    puts ' ' * level + 'Error: can not parse sexp ' + sexp.inspect
  end
end

go_deeper! Ruby19Parser.new.parse(File.open(__FILE__){ |f| f.read })

puts "Spen:    #$spen"
puts "Mayers:  #$mayers"
puts "Cheppin: #$cheppin"
