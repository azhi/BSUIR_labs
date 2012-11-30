// Free open-source Grom library

#ifndef __Grom_System_XMLAttribute_h_
#define __Grom_System_XMLAttribute_h_

#include "../Config.h"

#include <string>
#include <stdio.h>
#include "../Object.h"
#include "../ValueArray.h"
#include "XMLNode.h"

namespace Sys
{

// The XMLAttribute class represents XML Attributes

// XMLAttribute
class XMLAttribute : public XMLNode
{
public:
  void CopyFrom(XMLAttribute* from);

protected:
  virtual void Initialize(Object* owner); // override
  virtual void Finalize(); // override

private:
  // overriding hierarchy structure to forbid it
  void Clear();
  int ChildCount();
  XMLNode* Child(int index);
  XMLNode* FindChild(const char* path);
  XMLNode* CreateChild(const char* name);
};

} // namespace Sys

#endif // __Grom_System_XMLAttribute_h__
