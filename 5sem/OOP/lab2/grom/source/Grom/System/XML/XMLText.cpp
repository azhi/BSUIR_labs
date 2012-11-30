// Free open-source Grom library

#include "../PrecompiledHeader.h"

//#include <memory.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "../Utils.h"
#include "XMLText.h"

namespace Sys
{

// XMLText

void XMLText::Initialize(Object* owner)
{
  XMLNode::Initialize(owner); // inherited
  SetType(XMLNode::XML_TEXT);
}

void XMLText::Finalize()
{
  //Clear(); // Can lead to the "object being destroyed cannot become owner" exception
  XMLNode::Finalize(); // inherited
}

void XMLText::CopyFrom(XMLText* from)
{
  SetValue(from->Value());
}

} // namespace Sys
