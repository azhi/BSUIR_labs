require 'grom_wrapper'

GC.disable
def doSomething node
  node.child_count.times do |i|
    case node.child(i).type
    when 1
      node.child(i).set_value node.child(i).value + "!!at"
    when 2
      node.child(i).set_value node.child(i).value + "@@tx"
    when 3
      doSomething node.child(i)
    when 4
      node.child(i).set_value node.child(i).value + "##cm"
    when 5
      node.child(i).set_value node.child(i).value + "$$in"
    when 6
      node.child(i).set_value node.child(i).value + "%%cc"
    end
  end
end

root = GromXmlNodeCreator.create_without_owner
GromXmlUtl.load_from_file root, "test.xml"
doSomething root
GromXmlUtl.save_to_file root, "testcopy.xml"
root.destroy
puts "Done!"
