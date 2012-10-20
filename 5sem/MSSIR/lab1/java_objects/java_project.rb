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

  def chepo_spen
    @classes.each do |java_class|
      java_class.fields.each{ |field| puts field }

      java_class.methods.each do |method|
        method.block.variables.each{ |var| puts var }
      end
    end
  end
end
