// Free open-source Grom library

#ifndef __Grom_System_XMLNode_h_
#define __Grom_System_XMLNode_h_

#include "../Config.h"

#include <string>
#include <stdio.h>
#include "../Object.h"
#include "../ValueArray.h"

namespace Sys
{

// The XMLNode class represents a basic XML element in memory

// XMLNode
class XMLNode : public Object
{
public:
  void CopyFrom(XMLNode* from);
  short Type();
  void SetType(short value);
  const char* Name();
  void SetName(const char* value);
  const char* Value();
  void SetValue(const char* value);
  void SetValue(const char* value, int valueLength);
  void Clear();
  // Hierarchy
  XMLNode* Parent();
  void SetParent(XMLNode* value);
  int ChildCount();
  XMLNode* Child(int index);
  XMLNode* FindChild(const char* path); // names in path are delimited by '/'
  XMLNode* CreateChild(const char* name);

  // Type constant definitions
  static const short XML_ATTRIBUTE = 1;
  static const short XML_TEXT = 2;
  static const short XML_ELEMENT = 3;
  static const short XML_COMMENT = 4;
  static const short XML_INSTRUCTION = 5;
  static const short XML_CDATA = 6;

protected:
  virtual void Initialize(Object* owner); // override
  virtual void Finalize(); // override

private:
  // Fields
  short m_Type;
  std::string m_Name;
  std::string m_Value;
  Collection* m_Children;
};

class XMLNodeEnumerator
{
public:
  XMLNodeEnumerator();
  XMLNodeEnumerator(XMLNode* root);
  ~XMLNodeEnumerator();

  XMLNode* Root();
  void SetRoot(XMLNode* value);

  void Reset();
  bool MoveNext();
  XMLNode* Current();

private:
  void DeleteNonRecursiveEnumerator();

  XMLNode* m_Root;
  class NonRecursiveXMLNodeEnumerator* m_ChildrenEnumerator;
};

inline
XMLNode* XMLNodeEnumerator::Root()
{
  return m_Root;
}

} // namespace Sys

#endif // __Grom_System_XMLNode_h__
