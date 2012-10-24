require File.join(File.dirname(__FILE__), 'java_baseobject.rb')

class JavaHasVariables < JavaBaseObject
  attr_reader :variables
  attr_reader :ext_variables

  def initialize source, id = nil, ext_variables = []
    @ext_variables = ext_variables
    super(source, id)
  end

  def add_var name
    var = (@variables + @ext_variables).find{ |var| var.identifier == name }
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
