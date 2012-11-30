require 'grom_wrapper'

def doSomething node
  node.child_count.times do |i|
    case node.child(i).type
    when 1
      node.child(i).set_value node.child(i).value + "!!at"
    when 2
      node.child(i).set_value node.child(i).value + "@@tx"
    when 3
      node.child(i).set_value node.child(i).value + "##cm"
    when 4
      node.child(i).set_value node.child(i).value + "$$in"
    when 5
      node.child(i).set_value node.child(i).value + "%%cc"
    when 6
      doSomething node.child(i)
    end
  end
end

root = GromXmlNodeCreator.create
GromXmlUtl.load_from_file root, "test.xml"
doSomething root
GromXmlUtl.save_to_file root, "testcopy.xml"
GromXmlNodeCreator.destroy root
