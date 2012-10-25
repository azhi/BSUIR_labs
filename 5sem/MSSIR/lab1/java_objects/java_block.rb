require File.join(File.dirname(__FILE__), 'java_hasvariables.rb')
require File.join(File.dirname(__FILE__), 'java_instuction.rb')
require File.join(File.dirname(__FILE__), 'java_class.rb')

class JavaBlock < JavaHasVariables
  attr_reader :instructions

  def initialize source, id = nil, ext_variables = []
    super(source, id, ext_variables)

    @instructions = []
    @instr_variables = []
    source.gsub(RegexpBuilder.single_operation) do |operation|
      @instructions << JavaInstruction.new(operation, nil, @instr_variables + @ext_variables)
      @instr_variables += @instructions.last.variables
    end
  end

  def calc_mayers classes, level
    puts '   ' * level + "Block id=#{@id}, inside control operators:"
    e = @instructions.size;
    v = @instructions.size - 1;
    h = 0;
    @instructions.each do |instr|
      case instr.type
      when :control
        h += instr.expr_count - 1
        e += instr.blocks.size + 1
        v += instr.blocks.size + 2
        puts '   ' * level + "Control operator id=#{instr.id}, has #{instr.blocks.size} blocks, processing ..."
        instr.blocks.each do |block|
          ie, iv, ih = block.calc_mayers classes, level + 1
          e += ie; v += iv; h += ih;
        end
      when :control_inc
        e += 1
        v += 3
      when :cycle
        h += instr.expr_count - 1
        e += 2
        v += 3
        puts '   ' * level + "Control operator id=#{instr.id}, has #{instr.blocks.size} blocks, processing ..."
        instr.blocks.each do |block|
          ie, iv, ih = block.calc_mayers classes, level + 1
          e += ie; v += iv; h += ih;
        end
      when :call
        method = classes.map{ |java_class| java_class.methods.find{ |method| method.definition[:name] == instr.call_name } }.compact.first
        if method.nil?
          # probably method or function from some library, do nothing
          puts "Can't find method #{instr.call_name}, moving forward ... "
        else
          puts '   ' * level + "Call operator id=#{instr.id} for function #{instr.call_name}, processing ..."
          ie, iv, ih = method.calc_mayers classes, level + 1
          e += ie; v += iv; h += ih;
        end
      end
    end
    puts '   ' * level + "End block id=#{@id}"
    return e, v, h
  end
end
