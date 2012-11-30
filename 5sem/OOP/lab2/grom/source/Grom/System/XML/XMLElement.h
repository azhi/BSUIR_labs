// Free open-source Grom library

#ifndef __Grom_System_XMLElement_h_
#define __Grom_System_XMLElement_h_

#include "../Config.h"

#include <string>
#include <stdio.h>
#include "../Object.h"
#include "../ValueArray.h"
#include "XMLNode.h"

namespace Sys
{

// The XMLElement class represents basic XML element

// XMLElement
class XMLElement : public XMLNode
{
public:
  void CopyFrom(XMLElement* from);

protected:
  virtual void Initialize(Object* owner); // override
  virtual void Finalize(); // override

private:

};

} // namespace Sys

#endif // __Grom_System_XMLElement_h__
