require File.join(File.dirname(__FILE__), 'id_helper.rb')
require File.join(File.dirname(__FILE__), 'java_class.rb')

class JavaProject
  attr_reader :classes

  def initialize dir_path
    files = []
    files << Dir["#{dir_path}/**/*.java"]
    files.flatten!.uniq!

    @classes = []
    files.each.with_index do |file_path, index|
      File.open(file_path) do |file|
        @classes << JavaClass.new(file.read, IDHelper.get_next)
      end
    end
  end

  def process_block block
    unless block.variables.nil?
      block.variables.each do |var|
        var.types[:unused] = true if !var.types[:control] && !var.types[:io] && !var.types[:calc]
      end
      block.variables.each{ |var| puts var }
    end
    block.instructions.each do |instr|
      instr.variables.each{ |var| puts var }
      unless instr.block.nil?
        process_block instr.block
      end
    end
  end

  def chepo_spen
    @classes.each do |java_class|
      java_class.fields.each do |field|
        field.types[:unused] = true if !field.types[:control] && !field.types[:io] && !field.types[:calc]
      end
      java_class.fields.each{ |field| puts field }

      java_class.methods.each do |method|
        unless method.definition[:params].empty?
          method.definition[:params].each do |param|
            param.types[:unused] = true if !param.types[:control] && !param.types[:io] && !param.types[:calc]
          end
          method.definition[:params].each{ |param| puts param }
        end
        process_block method.block
      end
    end
  end
end
