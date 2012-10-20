require File.join(File.dirname(__FILE__), 'java_baseobject.rb')

class JavaIntruction < JavaBaseObject
  attr_reader :type
  attr_reader :variables

  def initialize source, id
    # defining type
    @type = :control # :io, :calc

    @variables = []
    # parsing for variables, adding variables names
    # note: i think here we just add names, and from top level (java_block), we
    # will create objects for them
    super(source, IDHelper.get_next)
  end
end
