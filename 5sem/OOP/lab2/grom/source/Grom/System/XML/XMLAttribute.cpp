// Free open-source Grom library

#include "../PrecompiledHeader.h"

//#include <memory.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "../Utils.h"
#include "XMLAttribute.h"

namespace Sys
{

// XMLAttribute

void XMLAttribute::Initialize(Object* owner)
{
  XMLNode::Initialize(owner); // inherited
  SetType(XMLNode::XML_ATTRIBUTE);
}

void XMLAttribute::Finalize()
{
  //Clear(); // Can lead to the "object being destroyed cannot become owner" exception
  XMLNode::Finalize(); // inherited
}

void XMLAttribute::CopyFrom(XMLAttribute* from)
{
  SetName(from->Name());
  SetValue(from->Value());
}

} // namespace Sys
