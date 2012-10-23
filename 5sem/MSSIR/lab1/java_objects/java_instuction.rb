require File.join(File.dirname(__FILE__), 'java_hasvariables.rb')

class JavaInstruction < JavaHasVariables
  attr_reader :type
  attr_reader :blocks

  def initialize source, id, ext_variables = []
    super(source, id, ext_variables)

    # defining type
    res = RegexpBuilder.io_operations source
    # NOTE 4 gerkola: discuss the interface here, cause i didn't get what are u returning
    @type = :control # :io, :calc

    @variables = []
    unless RegexpBuilder.expr.match(source).nil?
      # NOTE: can't figure out how to do this, by now this is not working
      expr = RegexpBuilder.expr.match(source)
      puts "src: #{expr[:expr]}"
      puts "gsub vars: #{expr[:expr].gsub(RegexpBuilder.expr, '\k<var>')}"
    end

    @blocks = []
    # if needed, creating a blocks attached to this instruction
    # @block = JavaBlock.new source, id, @ext_variables + @variables
  end
end
