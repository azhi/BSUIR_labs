// Free open-source Grom library

#include "../PrecompiledHeader.h"

//#include <memory.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "../Utils.h"
#include "XMLNode.h"

namespace Sys
{

// XMLNodeChildren

class XMLNodeChildren : public Collection
{
};

// XMLNode

void XMLNode::Initialize(Object* owner)
{
  m_Children = Create<XMLNodeChildren>(this);
  Object::Initialize(owner); // inherited
}

void XMLNode::Finalize()
{
  //Clear(); // Can lead to the "object being destroyed cannot become owner" exception
  Object::Finalize(); // inherited
}

void XMLNode::CopyFrom(XMLNode* from)
{
  Clear();
  SetType(from->Type());
  for (int i = 0; i < from->ChildCount(); ++i)
    CreateChild("")->CopyFrom(from->Child(i));
}

short XMLNode::Type()
{
  return m_Type;
}

void XMLNode::SetType(short value)
{
  if (value > 0 && value < 7)
    m_Type = value;
  else
    throw new Sys::ArgumentException("argument must be in [1, 6] range");
}

const char* XMLNode::Name()
{
  return m_Name.c_str();
}

void XMLNode::SetName(const char* value)
{
  if (value != NULL)
    m_Name = value;
  else
    m_Name.clear();
}

const char* XMLNode::Value()
{
  return m_Value.c_str();
}

void XMLNode::SetValue(const char* value)
{
  if (value != NULL)
    m_Value = value;
  else
    m_Value.clear();
}

void XMLNode::SetValue(const char* value, int valueLength)
{
  if (valueLength != 0)
    if (value != NULL)
      m_Value.assign(value, valueLength);
    else
      throw Sys::CreateArgumentNullException();
  else
    m_Value.clear();
}

void XMLNode::Clear()
{
  if (m_Children->Count() > 0)
  {
    DestroyAndNull(m_Children);
    m_Children = Create<XMLNodeChildren>(this);
  }
}

XMLNode* XMLNode::Parent()
{
  XMLNodeChildren* collection = as<XMLNodeChildren*>(Owner());
  if (collection != NULL)
    return cast<XMLNode*>(collection->Owner());
  else
    return NULL;
}

void XMLNode::SetParent(XMLNode* value)
{
  if (value == NULL)
    throw Sys::CreateArgumentNullException();
  SetOwner(value->m_Children);
}

int XMLNode::ChildCount()
{
  return m_Children->Count();
}

XMLNode* XMLNode::Child(int index)
{
  return cast<XMLNode*>(m_Children->Item(index));
}

XMLNode* XMLNode::FindChild(const char* path)
{
  XMLNode* result = this;
  if (path != NULL && path[0] != '\0')
  {
    MultiString names(path, "/", NULL);
    if (names.word_count() > 0)
    {
      // Go through hierarchy
      int k = 0;
      int m = names.word_count() - 1;
      while (k < m && result != NULL)
      {
        const char* name = names.word(k);
        int i = 0;
        int n = result->ChildCount();
        while (i < n && strcmp(result->Child(i)->Name(), name) != 0)
          ++i; // next child
        if (i < n)
          result = result->Child(i);
        else
          result = NULL; // not found
        ++k; // next level
      }
    }
  }
  return result;
}

XMLNode* XMLNode::CreateChild(const char* name)
{
  XMLNode* p = Create<XMLNode>(m_Children);
  if (name != NULL)
  {
    try
    {
      p->SetName(name);
    }
    catch (...)
    {
      p->Destroy();
      throw;
    }
  }
  return p;
}

// NonRecursiveXMLNodeEnumerator

class NonRecursiveXMLNodeEnumerator
{
public:
  NonRecursiveXMLNodeEnumerator(
    NonRecursiveXMLNodeEnumerator* previousEnumerator, XMLNode* parent);
  ~NonRecursiveXMLNodeEnumerator();

  NonRecursiveXMLNodeEnumerator* PreviousEnumerator();
  XMLNode* Parent();
  bool IsBeforeFirst();
  bool IsAfterLast();
  bool MoveNext();
  XMLNode* Current();

private:
  NonRecursiveXMLNodeEnumerator* m_PreviousEnumerator;
  XMLNode* m_Parent;
  int m_CurrentChild;
};

NonRecursiveXMLNodeEnumerator::NonRecursiveXMLNodeEnumerator(
  NonRecursiveXMLNodeEnumerator* previousEnumerator, XMLNode* parent) :
  m_PreviousEnumerator(previousEnumerator), m_Parent(parent), m_CurrentChild(-1)
{
}

NonRecursiveXMLNodeEnumerator::~NonRecursiveXMLNodeEnumerator()
{
}

inline
NonRecursiveXMLNodeEnumerator* NonRecursiveXMLNodeEnumerator::PreviousEnumerator()
{
  return m_PreviousEnumerator;
}

inline
XMLNode* NonRecursiveXMLNodeEnumerator::Parent()
{
  return m_Parent;
}

inline
bool NonRecursiveXMLNodeEnumerator::IsBeforeFirst()
{
  return m_CurrentChild < 0;
}

inline
bool NonRecursiveXMLNodeEnumerator::IsAfterLast()
{
  return m_CurrentChild >= m_Parent->ChildCount();
}

bool NonRecursiveXMLNodeEnumerator::MoveNext()
{
  if (m_CurrentChild < m_Parent->ChildCount())
    ++m_CurrentChild;
  return m_CurrentChild < m_Parent->ChildCount();
}

inline
XMLNode* NonRecursiveXMLNodeEnumerator::Current()
{
  return m_Parent->Child(m_CurrentChild);
}

// XMLNodeEnumerator

XMLNodeEnumerator::XMLNodeEnumerator() :
  m_Root(NULL), m_ChildrenEnumerator(NULL)
{
}

XMLNodeEnumerator::XMLNodeEnumerator(XMLNode* root) :
  m_Root(root), m_ChildrenEnumerator(NULL)
{
}

XMLNodeEnumerator::~XMLNodeEnumerator()
{
  Reset();
}

void XMLNodeEnumerator::SetRoot(XMLNode* value)
{
  if (m_Root != value)
  {
    Reset();
    m_Root = value;
  }
}

void XMLNodeEnumerator::Reset()
{
  while (m_ChildrenEnumerator != NULL)
    DeleteNonRecursiveEnumerator();
}

bool XMLNodeEnumerator::MoveNext()
{
  bool result = false;
  if (m_Root != NULL)
  {
    if (m_ChildrenEnumerator == NULL)
      m_ChildrenEnumerator = new NonRecursiveXMLNodeEnumerator(NULL, m_Root);
    if (!m_ChildrenEnumerator->IsBeforeFirst() && 
      !m_ChildrenEnumerator->IsAfterLast() && 
      m_ChildrenEnumerator->Current()->ChildCount() > 0)
    {
      m_ChildrenEnumerator = new NonRecursiveXMLNodeEnumerator(
        m_ChildrenEnumerator, m_ChildrenEnumerator->Current());
    }
    result = m_ChildrenEnumerator->MoveNext();
    while (!result && m_ChildrenEnumerator->PreviousEnumerator() != NULL)
    {
      DeleteNonRecursiveEnumerator();
      result = m_ChildrenEnumerator->MoveNext();
    }
  }
  return result;
}

XMLNode* XMLNodeEnumerator::Current()
{
  Sys::RunTimeAssert(m_ChildrenEnumerator != NULL);
  return m_ChildrenEnumerator->Current();
}

void XMLNodeEnumerator::DeleteNonRecursiveEnumerator()
{
  NonRecursiveXMLNodeEnumerator* p = m_ChildrenEnumerator;
  m_ChildrenEnumerator = p->PreviousEnumerator();
  delete p;
}

} // namespace Sys
