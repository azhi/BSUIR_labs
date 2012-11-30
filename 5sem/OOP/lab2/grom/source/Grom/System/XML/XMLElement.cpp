// Free open-source Grom library

#include "../PrecompiledHeader.h"

//#include <memory.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "../Utils.h"
#include "XMLElement.h"

namespace Sys
{

// XMLElement

void XMLElement::Initialize(Object* owner)
{
  XMLNode::Initialize(owner); // inherited
  SetType(XMLNode::XML_ELEMENT);
}

void XMLElement::Finalize()
{
  //Clear(); // Can lead to the "object being destroyed cannot become owner" exception
  XMLNode::Finalize(); // inherited
}

void XMLElement::CopyFrom(XMLElement* from)
{
  XMLNode::CopyFrom(as<XMLNode*>(from));
}

} // namespace Sys
