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
  when :const
    # Name:     sexp[1]
    puts ' ' * level + "Const: name=#{sexp[1].inspect}"
  when :hash, :array
    # Elements: sexp.sexp_body   ungrouped
    puts ' ' * level + "C2ontainer: el=#{sexp.sexp_body}"
    sexp.sexp_body.each{ |sexp| go_deeper! sexp, {}, level+1 }
  when :lit                      # number/sumbol/etc.
    # Value:    sexp[1]          s(:lit, 1) | s(:lit, :sym)
    puts ' ' * level + "Literal: val=#{sexp[1].inspect}"
  when :array
    sexp.sexp_body.each{ |sexp| go_deeper! sexp, {}, level+1 }
  when :dot2                     # range
    # From, to  sexp[1,2]
    go_deeper! sexp[1], {}, level+1
    go_deeper! sexp[2], {}, level+1
  when :str
    # Value:    sexp[1]          s(:str, "string")
    puts ' ' * level + "String: val=#{sexp[1].inspect}"
  when :dstr
    # Init_val  sexp[1]
    # Concated  sexp[2..-1]      s(:evstr, expression) | s(:str "string")
    sexp[2..-1].each do |sexp|
      if sexp.sexp_type == :evstr
        go_deeper! sexp[1]
      else
        go_deeper! sexp, {}, level+1
      end
    end

  when :lvar, :gvar, :ivar, :cvar
    # Name:     sexp[1]
    puts ' ' * level + "Var: name=#{sexp[1].inspect}"
  when :and, :or
    # Args:     sexp[1..2]
    puts ' ' * level + "Logic op: name=#{sexp[1..2].inspect}"
    go_deeper! sexp[1], {}, level+1
    go_deeper! sexp[2], {}, level+1
  when :op_asgn_and, :op_asgn_or
    # Cond:     sexp[1]          s(:?var, :name)
    # Asgn:     sexp[2]          s(:?asgn/:cvdecl, :name, :value)
    puts ' ' * level + "Cond assignment: cond=#{sexp[1].inspect} asgn=#{sexp[2].inspect}"
    go_deeper! sexp[1], {}, level+1
    go_deeper! sexp[2], {}, level+1
  when :call
    # Object:   sexp[1]          may be an expression
    # Name:     sexp[2]
    # Args:     sexp[3..-1]
    puts ' ' * level + "Call: obj=#{sexp[1].inspect} name=#{sexp[2].inspect} args=#{sexp[3..-1].inspect}"
    go_deeper! sexp[1], {}, level+1 unless sexp[1].nil?
    sexp[3..-1].each{ |sexp| go_deeper! sexp, {}, level+1 }
  when :iter
    # Object:   sexp[1]          Or s(:call, nil, :lambda)
    # Locals:   sexp[2]          s(:args, *names) | s(:args)
    # Body:     sexp[3]          block / expression
    puts ' ' * level + "Iterator: obj=#{sexp[1].inspect} locals=#{sexp[2].inspect} body=#{sexp[3].inspect}"
    go_deeper! sexp[1], {}, level+1 unless sexp[1] == s(:call, nil, :lambda)
    # process locals here
    go_deeper! sexp[3], {}, level+1

  when :if                       # and unless
    # Cond:     sexp[1]
    # Then:     sexp[2]          block / expression
    # Else:     sexp[3]          block / expression
    puts ' ' * level + "Condition: cond=#{sexp[1].inspect} then=#{sexp[2].inspect} else=#{sexp[3].inspect}"
    go_deeper! sexp[1], {}, level+1
    go_deeper! sexp[2], {}, level+1
    go_deeper! sexp[3], {}, level+1
  when :case
    # Cond:     sexp[1]
    # When:     sexp[2..-2]      s(:when, s(:array, expression),
    #                              block / expression)
    # Else:     sexp[-1]         block / expression
    go_deeper! sexp[1], {}, level+1
    sexp[2..-2].each do |sexp|
      go_deeper! sexp[1], {}, level+1
      go_deeper! sexp[2], {}, level+1
    end
    go_deeper! sexp[-1], {}, level+1

  when :for
    # Where:    sexp[1]          array / range
    # What:     sexp[2]          s(:lasgn, :name)
    # Body:     sexp[3]          block / expression
    puts ' ' * level + "For: where=#{sexp[1].inspect} what=#{sexp[2].inspect} body=#{sexp[3].inspect}"
    go_deeper! sexp[1], {}, level+1
    go_deeper! sexp[2], {}, level+1
    go_deeper! sexp[3], {}, level+1
  when :while, :until
    # Control:  sexp[1]
    # Body:     sexp[2]
    # ???:      sexp[3]          Always true
    puts ' ' * level + "Wh|unt loop: control=#{sexp[1].inspect} body=#{sexp[2].inspect}"
    go_deeper! sexp[1], {}, level+1
    go_deeper! sexp[2], {}, level+1

  when :defn
    # Name:     sexp[1]
    # Args:     sexp[2]          s(:args, *arg_names, s(:block, *asgns))
    # Body:     sexp[3]          *blocks
    puts ' ' * level + "Defn: name=#{sexp[1].inspect} args=#{sexp[2].inspect} body=#{sexp[3..-1].inspect}"
    #go_deeper! sexp[2], {}, level+1
    sexp[3..-1].each{ |sexp| go_deeper! sexp, {}, level+1 }

  when :gasgn, :lasgn, :iasgn, :cvdecl
    # Name:     sexp[1]
    # Value:    sexp[2]
    puts ' ' * level + "Assign: name=#{sexp[1].inspect} value=#{sexp[2].inspect}"
    go_deeper! sexp[2], {}, level+1

  when :return
    # No params
    puts ' ' * level + "return"

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
