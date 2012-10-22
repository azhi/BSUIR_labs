require File.join(File.dirname(__FILE__), 'java_hasvariables.rb')

class JavaIntruction < JavaHasVariables
  attr_reader :type
  attr_reader :block

  def initialize source, id, ext_variables = []
    super(source, id, ext_variables)

    # defining type
    @type = :control # :io, :calc

    @variables = []
    # parsing for variables
    # note: here, we must create JavaVariable objects by add_var method

    @block = nil
    # if needed, creating a block attached to this instruction
    # @block = JavaBlock.new source, id, @ext_variables + @variables
  end
end
