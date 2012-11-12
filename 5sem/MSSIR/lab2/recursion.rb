#!/usr/bin/env ruby

require 'ruby_parser'
require 'pry'

$mayers = nil
$cheppin = nil


$spen = 0
$w = 0
$e = 0
$c = [0] * 4

def go_deeper! sexp = nil, vars = {}
  p sexp
  binding.pry
  return unless sexp
  case sexp.sexp_type
  when nil
    return
  # TODO: strings with insertions ("#{}", etc.)
  when :const
    # Name:     sexp[1]
  when :hash, :array
    # Elements: sexp.sexp_body   ungrouped
  when :lit                      # number/sumbol/etc.
    # Value:    sexp[1]          s(:lit, 1) | s(:lit, :sym)
  when :str
    # Value:    sexp[1]          s(:str, "string")

  when :lvar, :gvar, :ivar, :cvar
    # Name:     sexp[1]
  when :and, :or
    # Args:     sexp[1..2]
  when :op_asgn_and, :op_asgn_or
    # Cond:     sexp[1]          s(:?var, :name)
    # Asgn:     sexp[2]          s(:?asgn/:cvdecl, :name, :value)
  when :call
    # Object:   sexp[1]          may be an expression
    # Name:     sexp[2]
    # Args:     sexp[3..-1]
  when :iter
    # Object:   sexp[1]          Or s(:call, nil, :lambda)
    # Locals:   sexp[2]          s(:args, *names) | s(:args)
    # Body:     sexp[3]          block / expression

  when :if                       # and unless
    # Cond:     sexp[1]
    # Then:     sexp[2]          block / expression
    # Else:     sexp[3]          block / expression

  when :for
    # Where:    sexp[1]          array / range
    # What:     sexp[2]          s(:lasgn, :name)
    # Body:     sexp[3]          block / expression
  when :while, :until
    # Control:  sexp[1]
    # Body:     sexp[2]
    # ???:      sexp[3]          Always true

  when :defn
    # Name:     sexp[1]
    # Args:     sexp[2]          s(:args, *arg_names, s(:block, *asgns))
    # Body:     sexp[3]          s(:scope, s(:block, *))
  when :gasgn, :lasgn, :iasgn, :cvdecl
    # Name:     sexp[1]
    # Value:    sexp[2]

  when :return
    # No params

  when :block
    sexp.sexp_body.each{ |s| go_deeper! s }

  else
    puts 'Error: can not parse sexp ' + sexp.inspect
  end
end

go_deeper! Ruby19Parser.new.parse(File.open(__FILE__){ |f| f.read })

puts "Spen:    #$spen"
puts "Mayers:  #$mayers"
puts "Cheppin: #$cheppin"
