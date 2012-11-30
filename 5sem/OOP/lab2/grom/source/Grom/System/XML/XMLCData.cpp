// Free open-source Grom library

#include "../PrecompiledHeader.h"

//#include <memory.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "../Utils.h"
#include "XMLCData.h"

namespace Sys
{

// XMLText

void XMLCData::Initialize(Object* owner)
{
  XMLNode::Initialize(owner); // inherited
  SetType(XMLNode::XML_CDATA);
}

void XMLCData::Finalize()
{
  //Clear(); // Can lead to the "object being destroyed cannot become owner" exception
  XMLNode::Finalize(); // inherited
}

void XMLCData::CopyFrom(XMLCData* from)
{
  SetValue(from->Value());
}

} // namespace Sys
