require File.join(File.dirname(__FILE__), 'id_helper.rb')
require File.join(File.dirname(__FILE__), '../regexp_builder.rb')

class JavaBaseObject
  attr_reader :raw_text
  attr_reader :id

  def initialize text, id = nil
    @raw_text = text
    @id = id || IDHelper.get_next
  end
end
