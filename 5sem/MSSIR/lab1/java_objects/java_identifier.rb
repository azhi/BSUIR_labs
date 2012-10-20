require File.join(File.dirname(__FILE__), 'java_baseobject.rb')

class JavaIdentifier < JavaBaseObject
  attr_reader :identifier
  attr_reader :count

  def initialize name
    @identifier = name
    @count = 1
    super name
  end

  def inc_count
    @count += 1
  end
end
