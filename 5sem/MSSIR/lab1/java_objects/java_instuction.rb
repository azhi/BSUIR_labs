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
    try_size = ->(w){ m[w] ? m[w].size : 0 }
    sz = [ try_size.call(:for_form),
           try_size.call(:if_form),
           try_size.call(:while_form),
           try_size.call(:var_def),
           try_size.call(:expr) ].max


    @variables = []
    @blocks = []

    if try_size.call(:for_form) == sz
      ini, con, it = m[:for_form][RegexpBuilder.surrounded_by].split ?;
      ini = ini[1..-1].strip + ?;
      con = con + ?;
      it = it[0..-2].strip + ?;
      variables = []
      vars = RegexpBuilder.parse_variable_init(ini)
      variables += vars[:i]
      if RegexpBuilder.operation.match(ini)[:var_def]
        @variables += vars[:o].map{ |v| JavaVariable.new v }
      end
      variables += RegexpBuilder.get_vars(con)
      variables += RegexpBuilder.get_vars(it)
      variables.each do |var_name|
        var = add_var var_name
        var.setType :control, true
      end unless variables.nil?
      @type = :cycle

      @expr_count = con.split(RegexpBuilder.bool_b_ops).size
      @blocks << JavaBlock.new("{#{m[:block]}}", nil,
                               @variables + @ext_variables) if m[:block]
    elsif try_size.call(:if_form) == sz
      ctrl = m[:if_form][RegexpBuilder.surrounded_by]
      variables = RegexpBuilder.get_vars(ctrl)
      variables.each do |var_name|
        var = add_var var_name
        var.setType :control, true
      end unless variables.nil?
      @type = :control
      @expr_count = ctrl.split(RegexpBuilder.bool_b_ops).size
      @blocks << JavaBlock.new("{#{m[:then_block]}}", nil,
                               @variables + @ext_variables) if m[:then_block]
      @blocks << JavaBlock.new("{#{m[:else_block]}}", nil,
                               @variables + @ext_variables) if m[:else_block]
    elsif try_size.call(:while_form) == sz
      ctrl = m[:while_form][RegexpBuilder.surrounded_by]
      variables = RegexpBuilder.get_vars(ctrl)
      variables.each do |var_name|
        var = add_var var_name
        var.setType :control, true
      end unless variables.nil?
      @type = :cycle
      @expr_count = ctrl.split(RegexpBuilder.bool_b_ops).size

      @blocks << JavaBlock.new("{#{m[:block]}}", nil,
                               @ext_variables + @variables) if m[:block]
    elsif try_size.call(:var_def) == sz
      variables = RegexpBuilder.parse_variable_init(m[:defs])
      variables[:o].each do |var_name|
        @variables << JavaVariable.new(var_name)
      end unless variables[:o].nil?
      variables[:i].each do |var_name|
        var = add_var var_name
        var.setType :calc, true
      end unless variables[:i].nil?
    elsif m[:method_call]
      @type = :call
      @call_name = m[:call_name].split(?.).last
      io_type = false
      unless (@call_name =~ /( print | println | readLine | Reader | Writer )+/ix).nil?
        io_type = true
      end
      variables = RegexpBuilder.get_vars(m[:method_call])
      variables.each do |var_name|
        var = add_var var_name
        var.setType :io, io_type unless var.types[:io]
        var.setType :calc, true
      end unless variables.nil?
    elsif try_size.call(:expr) == sz
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
      @type = :control_inc if source =~ /break|continue/
    end unless sz.zero?
  end
end
