#include "Grom/System/Object.h"
#include "Grom/System/XML/XMLNode.h"
#include "Grom/XmlUtils/XmlUtils.h"

#include <rice/Data_Type.hpp>
#include <rice/Constructor.hpp>

using namespace Rice;

typedef void (Sys::XMLNode::*SetValue1Arg)(const char* value);

Sys::XMLNode* CreateXMLNodeWithNullOwnerWrapper()
{
  return Sys::Create<Sys::XMLNode>(NULL);
}

Sys::XMLNode* CreateXMLNodeWrapper(Sys::XMLNode* owner)
{
  return Sys::Create<Sys::XMLNode>(owner);
}

template<>
short from_ruby<short>(Object x)
{
  int i_x = from_ruby<int>(x);
  return (short) i_x;
}

template<>
Object to_ruby<short>(short const & x)
{
  int i_x = x;
  return to_ruby<int>(i_x);
}

extern "C"
void Init_grom_wrapper()
{
  Module rb_mGOC =
    define_module("GromXmlNodeCreator")
    .define_module_function("create", &CreateXMLNodeWrapper)
    .define_module_function("create_without_owner", &CreateXMLNodeWithNullOwnerWrapper)
    ;

  Data_Type<Sys::Object> rb_cObject =
    define_class<Sys::Object>("GromObject")
    ;

  Data_Type<Sys::XMLNode> rb_cXMLNode =
    define_class<Sys::XMLNode, Sys::Object>("GromXMLNode")
    .define_method("type", &Sys::XMLNode::Type)
    .define_method("name", &Sys::XMLNode::Name)
    .define_method("value", &Sys::XMLNode::Value)
    .define_method("set_value", (SetValue1Arg) &Sys::XMLNode::SetValue, Arg("value"))
    .define_method("child_count", &Sys::XMLNode::ChildCount)
    .define_method("child", &Sys::XMLNode::Child)
    .define_method("destroy", &Sys::XMLNode::Destroy)
    ;

  Module rb_mUtl = 
    define_module("GromXmlUtl")
    .define_module_function("load_from_file", &Utl::LoadTreeFromXmlFile)
    .define_module_function("save_to_file", &Utl::SaveTreeToXmlFile)
    ;
}
