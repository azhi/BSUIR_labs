#include <Grom/XmlUtils/XmlUtils.h>
#include <Grom/System/XML/XMLNode.h>
#include <stdio.h>

void changeSomething(Sys::XMLNode* node)
{
  std::string buf;
  if (node->ChildCount() > 0)
  {
    for (int i = 0; i < node->ChildCount(); ++i)
    {
      Sys::XMLNode* child = node->Child(i);
      switch (child->Type())
      {
        case Sys::XMLNode::XML_ATTRIBUTE:
          {
            std::string attr = child->Value();
            attr += "!!at";
            child->SetValue(attr.c_str());
          }
          break;
        case Sys::XMLNode::XML_TEXT:
          {
            std::string tx = child->Value();
            tx += "@@tx";
            child->SetValue(tx.c_str());
          }
          break;
        case Sys::XMLNode::XML_COMMENT:
          {
            std::string cm = child->Value();
            cm += "##cm";
            child->SetValue(cm.c_str());
          }
          break;
        case Sys::XMLNode::XML_INSTRUCTION:
          {
            std::string in = child->Value();
            in += "$$in";
            child->SetValue(in.c_str());
          }
          break;
        case Sys::XMLNode::XML_CDATA:
          {
            std::string c = child->Value();
            c += "%%cc";
            child->SetValue(c.c_str());
          }
          break;
        case Sys::XMLNode::XML_ELEMENT:
          {
            changeSomething(child);
          }
          break;
      }
    }
  }
}

int main(int argc, const char *argv[])
{
  Sys::XMLNode* pt = Sys::Create<Sys::XMLNode>(NULL);
  printf("reading\n");
  Utl::LoadTreeFromXmlFile(pt, "test.xml");
  printf("applying some changes...\n");
  changeSomething(pt);
  printf("writing\n");
  Utl::SaveTreeToXmlFile(pt, "testcopy.xml");
  pt->Destroy();
  return 0;
}
