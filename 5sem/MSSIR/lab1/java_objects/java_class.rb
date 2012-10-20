require File.join(File.dirname(__FILE__), 'java_baseobject.rb')
require File.join(File.dirname(__FILE__), 'java_method.rb')
require File.join(File.dirname(__FILE__), 'java_variable.rb')

class JavaClass < JavaBaseObject
  attr_reader :methods
  attr_reader :fields

  def initialize source, id
    @source = source.gsub(RegexpBuilder.class_def).to_a.first
    raise unless @source

    @methods = []
    @source.gsub(RegexpBuilder.method_def) do |jms|
      @methods << JavaMethod.new(jms)
    end

    @fields = []
    @source.gsub(RegexpBuilder.field_def) do |jfs|
      @fields << JavaVariable.new(jfs)
    end

    super source
  end
end
