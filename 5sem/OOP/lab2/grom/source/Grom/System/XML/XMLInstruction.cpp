// Free open-source Grom library

#include "../PrecompiledHeader.h"

//#include <memory.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "../Utils.h"
#include "XMLInstruction.h"

namespace Sys
{

// XMLInstruction

void XMLInstruction::Initialize(Object* owner)
{
  XMLNode::Initialize(owner); // inherited
  SetType(XMLNode::XML_INSTRUCTION);
}

void XMLInstruction::Finalize()
{
  //Clear(); // Can lead to the "object being destroyed cannot become owner" exception
  XMLNode::Finalize(); // inherited
}

void XMLInstruction::CopyFrom(XMLInstruction* from)
{
  SetValue(from->Value());
}

} // namespace Sys
