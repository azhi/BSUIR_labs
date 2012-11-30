// Free open-source Grom library

#include "../PrecompiledHeader.h"

//#include <memory.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "../Utils.h"
#include "XMLComment.h"

namespace Sys
{

// XMLComment

void XMLComment::Initialize(Object* owner)
{
  XMLNode::Initialize(owner); // inherited
  SetType(XMLNode::XML_COMMENT);
}

void XMLComment::Finalize()
{
  //Clear(); // Can lead to the "object being destroyed cannot become owner" exception
  XMLNode::Finalize(); // inherited
}

void XMLComment::CopyFrom(XMLComment* from)
{
  SetValue(from->Value());
}

} // namespace Sys
