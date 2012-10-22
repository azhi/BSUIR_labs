require File.join(File.dirname(__FILE__), 'java_hasvariables.rb')
require File.join(File.dirname(__FILE__), 'java_instuction.rb')

class JavaBlock < JavaHasVariables
  attr_reader :instructions

  def initialize source, id = nil, ext_variables = []
    super(source, id, ext_variables)

    @instructions = []
    # parse instructions
  end
end
