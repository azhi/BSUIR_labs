require File.join(File.dirname(__FILE__), 'java_identifier.rb')

class JavaVariable < JavaIdentifier
  attr_accessor :types

  def initialize name
    @types = { :io => false, :calc => false, :control => false, :unused => false }
    super name
  end

  def to_s
    types_str = @types.map{ |key, val| val == true ? key.to_s : nil }.compact.join('/')
    "Variable: id=#{@id}, name=#{@identifier}, spen=#{@count-1}, types=#{types_str}"
  end
end
