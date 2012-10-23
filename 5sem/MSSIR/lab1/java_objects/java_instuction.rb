require File.join(File.dirname(__FILE__), 'java_hasvariables.rb')

class JavaInstruction < JavaHasVariables
  attr_reader :type
  attr_reader :blocks

  def initialize source, id, ext_variables = []
    super(source, id, ext_variables)

    # defining type
    # res = RegexpBuilder.io_operations source
    # NOTE 4 gerkola: discuss the interface here, cause i didn't get what are u returning

    m = RegexpBuilder.single_operation.match(source)
    if m[:for_form] || m[:if_form]
      @type = :control
    elsif RegexpBuilder.io_operations(source)[:o].any?
      @type = :io
    else
      @type = :calc
    end

    case @type
    when :io
      inputs  = RegexpBuilder.io_operations(source)[:i]
      outputs = RegexpBuilder.io_operations(source)[:o]
    end

    @variables = []
    unless @type == :control || RegexpBuilder.expr.match(source).nil?
      # NOTE: can't figure out how to do this, by now this is not working
      #   it is easier to help you when one knows what you want (gekola)
      expr = RegexpBuilder.expr.match(source)
      puts "src: #{expr[:expr]}"
      puts "gsub vars: #{expr[:expr].gsub(RegexpBuilder.expr, '\k<var>')}"
    end

    @blocks = []
    # if needed, creating a blocks attached to this instruction
    # @block = JavaBlock.new source, id, @ext_variables + @variables

    if m[:for_form] && (!m[:if_form] ||
                        m[:if_form].size < m[:for_form].size)
      @blocks << JavaBlock.new(m[:block], nil, @ext_variables + @variables)
    else
      @blocks << JavaBlock.new("{#{m[:then_block]}}", nil,
                               @ext_variables + @variables) if m[:then_block]
      @blocks << JavaBlock.new("{#{m[:else_block]}}", nil,
                               @ext_variables + @variables) if m[:else_block]
    end if nil # @type == :control
    # TODO: Work around control operations, that are not in block
    #   Also analize 'for' control section
  end
end
