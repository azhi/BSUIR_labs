require File.join(File.dirname(__FILE__), 'java_hasvariables.rb')
require File.join(File.dirname(__FILE__), 'java_instuction.rb')

class JavaBlock < JavaHasVariables
  attr_reader :instructions

  def initialize source, id = nil, ext_variables = []
    super(source, id, ext_variables)

    @instructions = []
    source.gsub(RegexpBuilder.single_operation) do |operation|
      # NOTE 4 gerkola: get rid of this useless ';' operations somehow, or just
      # discuss it with me
      @instructions << JavaInstruction.new(operation, nil, @ext_variables)
    end
    # parse instructions
  end
end
