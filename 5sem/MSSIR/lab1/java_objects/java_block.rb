require File.join(File.dirname(__FILE__), 'java_baseobject.rb')
require File.join(File.dirname(__FILE__), 'java_instuction.rb')

class JavaBlock < JavaBaseObject
  attr_reader :instructions
  attr_reader :variables

  def initialize source, id = nil, variables = []
    # if its block from method, variables - params of method
    @variables = variables

    @instructions = []
    # parse instructions

    @instructions.each do |instr|
      instr.variables.each do |var_name|
        var = add_var var_name
        case instr.type
        when :control
          var.types[:control] = true
        when :io
          var.types[:io] = true
        when :calc
          var.types[:calc] = true
        end
      end
    end

    @variables.each do |var|
      var.types[:unused] = true if !var.types[:control] && !var.types[:io] && !var.types[:calc]
    end
    super(source, IDHelper.get_next)
  end

  def add_var name
    var = @variables.find{ |var| var.identifier == name }
    res = nil
    if var.nil?
      res = JavaVariable.new name
      @variables << res
    else
      var.inc_count
      res = var
    end
    res
  end
end
