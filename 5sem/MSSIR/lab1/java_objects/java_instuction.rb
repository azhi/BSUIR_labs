require File.join(File.dirname(__FILE__), 'java_hasvariables.rb')

class JavaInstruction < JavaHasVariables
  attr_reader :type
  attr_reader :blocks
  # NOTE expr_count should be defined for control instructions and should be
  # equal of expression count, used to define the decision
  attr_reader :expr_count
  attr_reader :call_name

  def initialize source, id, ext_variables = []
    super(source, id, ext_variables)

    m = RegexpBuilder.single_operation.match(source)

    @variables = []
    @blocks = []

    if m[:for_form] && (!m[:if_form] ||
                        m[:if_form].size < m[:for_form].size)
      # NOTE 4 gerkola: expecting here a list of used variables in for
      # definition
      variables = RegexpBuilder.io_operations(m[:for_form])[:variables]
      variables.each do |var_name|
        var = add_var var_name
        var.setType :control, true
      end unless variables.nil?
      @type = :control
      # TODO: expr_count calculation
      @expr_count = 1
      @blocks << JavaBlock.new("{#{m[:block]}}", nil, @ext_variables + @variables) if m[:block]
    elsif m[:if_form]
      # NOTE 4 gerkola: expecting here a list of used variables in if
      # definition
      variables = RegexpBuilder.io_operations(m[:if_form])[:variables]
      variables.each do |var_name|
        var = add_var var_name
        var.setType :control, true
      end unless variables.nil?
      @type = :control
      # TODO: expr_count calculation
      @expr_count = 1
      @blocks << JavaBlock.new("{#{m[:then_block]}}", nil,
                               @ext_variables + @variables) if m[:then_block]
      @blocks << JavaBlock.new("{#{m[:else_block]}}", nil,
                               @ext_variables + @variables) if m[:else_block]
    elsif m[:method_call]
      # NOTE 4 gerkola: expecting here a list of used variables in params of
      # calling
      @type = :call
      @call_name = m[:call_name]
      io_type = false
      unless (@call_name =~ /( print | println | readLine | Reader | Writer )+/ix).nil?
        io_type = true
      end
      variables = RegexpBuilder.io_operations(m[:method_call])[:variables]
      variables.each do |var_name|
        var = add_var var_name
        var.setType :io, io_type
      end unless variables.nil?
    elsif m[:expr]
      inputs = RegexpBuilder.io_operations(source)[:i]
      outputs = RegexpBuilder.io_operations(source)[:o]
      inputs.each do |var_name|
        var = add_var var_name
        var.setType :calc, true
      end unless inputs.nil?
      outputs.each do |var_name|
        var = add_var var_name
        var.setType :calc, true
      end unless outputs.nil?
    # elsif is_a_var_def
      # NOTE in var der, we shoudn't call add_var method, we should add
      # variable directly, cause even if's name matches external variable, we
      # should create a local one with the same name
    # elsif for_loop
    # elsif while_loop
    end
  end
end
