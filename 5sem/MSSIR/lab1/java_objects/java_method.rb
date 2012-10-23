require File.join(File.dirname(__FILE__), 'java_baseobject.rb')
require File.join(File.dirname(__FILE__), 'java_block.rb')
require File.join(File.dirname(__FILE__), 'java_variable.rb')

class JavaMethod < JavaBaseObject
  attr_reader :definition
  attr_reader :fields
  attr_reader :block

  def initialize source, fields
    @fields = fields
    @source = source
    def_m = RegexpBuilder.method_def.match(@source)
    name = def_m[:ex_name].split.last
    params = def_m[:params].split(?,).map(&:split).map(&:last)
    @definition = {name: name, params: []}
    params.each do |p|
      @definition[:params] << JavaVariable.new(p)
    end
    # p @definition
    @block = JavaBlock.new def_m[:block], nil, @fields + @definition[:params]
    # p @block
    super source
  end
end
