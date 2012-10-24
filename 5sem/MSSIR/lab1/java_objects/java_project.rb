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

    each_variable{ |var| var.setType :unused, true if !var.types[:control] && !var.types[:io] && !var.types[:calc] }
  end

  def each_variable
    @classes.each do |java_class|
      java_class.fields.each do |field|
        yield field
      end

      java_class.methods.each do |method|
        unless method.definition[:params].empty?
          method.definition[:params].each do |param|
            yield param
          end
        end
        process_block method.block unless method.block.nil?
      end
    end
  end


  def process_block block
    unless block.variables.nil?
      block.variables.each do |var|
        yield var
      end
    end
    block.instructions.each do |instr|
      instr.variables.each{ |var| yield var }
      instr.blocks.each{ |block| process_block block }
    end
  end

  def chepo_spen
    puts "Variables:"
    each_variable{ |var| puts var } 
  end

  def chepin
    res = 0
    each_variable do |var|
      res += 3 if var.types[:control]
      res += 2 if var.types[:calc]
      res += 1 if var.types[:io]
      res += 0.5 if var.types[:unused]
    end
    puts "Chepin = #{res}"
  end

  def mayers
    main_method = @classes.map{ |java_class| java_class.methods.find{ |method| method.definition[:name] == "Main" } }.compact.first
    e, v, h = main_method.calc_mayers @classes, 0
    puts "Mayers = [#{e - v + 2}; #{e - v + 2 + h}]"
  end
end
