// Free open-source Grom library

#include "PrecompiledHeader.h"

#include "Array.h"
#include "Synchronization.h"
#include "Monitor.h"
#include "Object.h"

namespace Sys
{

// Global variables

static int g_DestroySectionCount = 0;
static bool g_ExceptionDetectedDuringDestroy = false;
static PointerArray g_DestroyedObjects; // should be created on heap?
static Object* g_GlobalOwner = NULL;
int g_MaxSyncBlockCount = 10 * 1024;
extern PointerArray g_SyncBlockPool; // not published, see Monitor.cpp

// Global functions

void Initialize()
{
  g_SyncBlockPool.SetCount(g_MaxSyncBlockCount);
  GlobalOwner();
}

void Finalize()
{
  ThreadMutexLockScope lock(g_GlobalMutex);
  {
    g_GlobalOwner->Destroy();
    g_GlobalOwner = NULL;
    g_SyncBlockPool.Clear();
  }
}

Object* GlobalOwner()
{
  if (g_GlobalOwner == NULL)
  {
    ThreadMutexLockScope lock(g_GlobalMutex);
    {
      if (g_GlobalOwner == NULL)
        g_GlobalOwner = Create<Object>(NULL);
    }
  }
  return g_GlobalOwner;
}

// Object

Object::Object() : m_SyncBlockIndex(-1)
{
}

Object::~Object()
{
  delete m_OwnedObjects; // do not touch this line
}

void Object::Initialize(Object* owner)
{
  SetOwner(owner);
}

void Object::Finalize()
{
  if (m_OwnedObjects != NULL)
  {
    for (int i = m_OwnedObjects->Count() - 1; i >= 0; --i)
      ((Object*)m_OwnedObjects->Item(i))->Destroy();
  }
}

void Object::Destroy()
{
  if (this != NULL)
  {
    DestroyScope ds;
    if (!m_IsDestroying)
    {
      m_IsDestroying = true;
      try
      {
        Finalize();
        Object* owner = m_Owner;
        if (owner != NULL && !owner->m_IsDestroying)
          owner->DropOwnership(this);
        if (owner == NULL || !owner->m_IsDestroying)
          g_DestroyedObjects.Add(this);
      }
      catch (...)
      {
        m_IsDestroying = false;
        g_ExceptionDetectedDuringDestroy = true;
        throw;
      }
    }
  }
}

void Object::SetOwner(Object* value)
{
  if (m_Owner != value)
  {
    if (value != NULL)
      value->TakeOwnership(this);
    else
      m_Owner->DropOwnership(this);
  }
}

PointerArray* Object::AcquireObjectArray()
{
  if (m_OwnedObjects == NULL)
    m_OwnedObjects = new PointerArray();
  return m_OwnedObjects;
}

bool Object::CanBeOwnerOf(Object* object)
{
  return true;
}

void Object::ValidateOwner(Object* owner)
{
  if (owner == NULL)
   throw new ArgumentException("cannot validate NULL owner");
  if (m_Owner == owner)
    throw new ArgumentException("object is already owned by the given object");
  if (owner == this)
    throw new ArgumentException("object cannot become owner of itself");
  if (m_IsDestroying || owner->m_IsDestroying)
    throw new ArgumentException("object being destroyed cannot become owner");
  if (!owner->CanBeOwnerOf(this))
    throw new ArgumentException("object cannot become owner of the given object");
}

void Object::TakeOwnership(Object* object)
{
  object->ValidateOwner(this);
  // Drop ownership
  if (object->m_Owner != NULL)
    object->m_Owner->DropOwnership(object);
  // Never insert statements between the following two lines
  AcquireObjectArray()->Add(object);
  object->m_Owner = this;
}

void Object::DropOwnership(Object* object)
{
  int i = CyclicSearch(object);
  DropOwnershipAt(i);
}

Object* Object::DropOwnershipAt(int index)
{
  if (m_OwnedObjects == NULL)
    throw CreateIndexOutOfBoundsException(index);
  // Never insert statements between the following two lines
  Object* result = (Object*)m_OwnedObjects->RemoveAt(index);
  result->m_Owner = NULL;
  return result;
}

int Object::CyclicSearch(Object* object)
{
  int result = -1;
  if (m_OwnedObjects != NULL)
  {
    result = m_OwnedObjects->Count() - 1;
    if (result >= 0 && m_OwnedObjects->Item(result) != object)
      result = m_OwnedObjects->IndexOf(object, 0, result);
  }
  return result;
}

void Object::FreeMemory()
{
  if (m_OwnedObjects != NULL)
    for (int i = m_OwnedObjects->Count() - 1; i >= 0; --i)
      ((Object*)m_OwnedObjects->Item(i))->FreeMemory();
  Monitor::DestroyThreadMutex(this);
  this->~Object(); // finalize v-table and aggregated non-Sys:: objects (std::string, etc)
  free(this);
}

void Object::FreeAllDestroyedObjects()
{
  for (int i = 0; i < g_DestroyedObjects.Count(); ++i)
  {
    if (g_DestroyedObjects.Item(i) != NULL)
      ((Object*)g_DestroyedObjects.Item(i))->FreeMemory();
    g_DestroyedObjects.SetItem(i, NULL);
  }
  g_DestroyedObjects.Clear();
}

// Collection

int Collection::Capacity()
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->Capacity();
  else
    return 0;
}

void Collection::SetCapacity(int capacity)
{
  if (Capacity() != capacity)
  {
    if (capacity >= Count())
    {
      if (capacity > 0)
        AcquireObjectArray()->SetCapacity(capacity);
      else
      {
        delete m_OwnedObjects;
        m_OwnedObjects = NULL;
      }
    }
    else
      throw new ArgumentException("capacity cannot be less than count");
  }
}

int Collection::Count()
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->Count();
  else
    return 0;
}

Object* Collection::Item(int index)
{
  if (m_OwnedObjects == NULL)
    throw CreateIndexOutOfBoundsException(index);
  return (Object*)m_OwnedObjects->Item(index);
}

void Collection::Add(Object* item)
{
  TakeOwnership(item);
}

void Collection::Insert(int index, Object* item)
{
  TakeOwnership(item); // rely on the fact that duplicate item causes exception
  m_OwnedObjects->Move(CyclicSearch(item), index);
}

void Collection::Remove(Object* item)
{
  if (item->Owner() == this)
    DropOwnership(item);
  // Attempt to remove already removed object is ignored.
  else
   throw new ArgumentException("item is not a member of the given collection");
}

Object* Collection::RemoveAt(int index)
{
  return DropOwnershipAt(index);
}

void Collection::Move(int sourceIndex, int targetIndex)
{
  if (m_OwnedObjects == NULL)
    throw CreateIndexOutOfBoundsException(sourceIndex);
  m_OwnedObjects->Move(sourceIndex, targetIndex);
}

void Collection::MoveRange(int sourceIndex, int count, int targetIndex)
{
  if (m_OwnedObjects == NULL)
    throw CreateRangeOutOfBoundsException(sourceIndex, count);
  m_OwnedObjects->MoveRange(sourceIndex, count, targetIndex);
}

void Collection::Clear(Collection*& collection)
{
  if (collection->Count() > 0)
  {
    Sys::Collection* newCollection = Sys::Create<Sys::Collection>(collection->Owner());
    try
    {
      DestroyAndNull(collection);
      collection = newCollection;
    }
    catch (...)
    {
      newCollection->Destroy();
      throw;
    }
  }
}

int Collection::IndexOf(Object* item)
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->IndexOf(item);
  else
    return -1;
}

int Collection::IndexOf(Object* item, int startIndex)
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->IndexOf(item, startIndex);
  else
    return -1;
}

int Collection::IndexOf(Object* item, int startIndex, int count)
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->IndexOf(item, startIndex, count);
  else
    return -1;
}

int Collection::LastIndexOf(Object* item)
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->LastIndexOf(item);
  else
    return -1;
}

int Collection::LastIndexOf(Object* item, int startIndex)
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->LastIndexOf(item, startIndex);
  else
    return -1;
}

int Collection::LastIndexOf(Object* item, int startIndex, int count)
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->LastIndexOf(item, startIndex, count);
  else
    return -1;
}

bool Collection::Contains(Object* item)
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->Contains(item);
  else
    return false;
}

void Collection::Sort()
{
  if (m_OwnedObjects != NULL)
    m_OwnedObjects->Sort();
}

void Collection::Sort(CompareProc* comparer, void* userData)
{
  if (m_OwnedObjects != NULL)
    m_OwnedObjects->Sort(CastToPointerComparer(comparer), userData);
}

void Collection::Sort(int startIndex, int count, CompareProc* comparer, void* userData)
{
  if (m_OwnedObjects != NULL)
    m_OwnedObjects->Sort(startIndex, count, CastToPointerComparer(comparer), userData);
}

int Collection::BinarySearch(Object* item)
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->BinarySearch(item);
  else
    return ~0;
}

int Collection::BinarySearch(Object* item, CompareProc* comparer, void* userData)
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->BinarySearch(item, CastToPointerComparer(comparer), userData);
  else
    return ~0;
}

int Collection::BinarySearch(Object* item, int startIndex, int count)
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->BinarySearch(item, startIndex, count);
  else
    return ~0;
}

int Collection::BinarySearch(Object* item, int startIndex, int count, CompareProc* comparer, void* userData)
{
  if (m_OwnedObjects != NULL)
    return m_OwnedObjects->BinarySearch(item, startIndex, count, CastToPointerComparer(comparer), userData);
  else
    return ~0;
}

void Collection::Reverse()
{
  if (m_OwnedObjects != NULL)
    m_OwnedObjects->Reverse();
}

void Collection::Reverse(int startIndex, int count)
{
  if (m_OwnedObjects == NULL)
    throw CreateRangeOutOfBoundsException(startIndex, count);
  m_OwnedObjects->Reverse(startIndex, count);
}

void Collection::TrimToSize()
{
  SetCapacity(Count());
}

// DestroyScope

DestroyScope::DestroyScope()
{
  g_GlobalMutex.Lock();
  ++g_DestroySectionCount;
}

DestroyScope::~DestroyScope()
{
  if (g_DestroySectionCount == 1)
  {
    if (!g_ExceptionDetectedDuringDestroy)
      Object::FreeAllDestroyedObjects();
    g_ExceptionDetectedDuringDestroy = false;
  }
  --g_DestroySectionCount;
  g_GlobalMutex.Unlock();
}

// LockScope

LockScope::LockScope(Object* obj, bool lockNow) :
  m_Object(obj), m_IsLocked(false)
{
  if (lockNow)
    LockNow();
}

LockScope::~LockScope()
{
  if (m_IsLocked)
    UnlockNow();
}

void LockScope::LockNow()
{
  if (!m_IsLocked)
  {
    Monitor::Enter(m_Object);
    m_IsLocked = true;
  }
}

void LockScope::UnlockNow()
{
  if (m_IsLocked)
  {
    Monitor::Exit(m_Object);
    m_IsLocked = false;
  }
}

} // namespace Sys
