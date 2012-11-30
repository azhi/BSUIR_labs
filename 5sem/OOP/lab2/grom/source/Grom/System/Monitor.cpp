// Free open-source Grom library

#include "PrecompiledHeader.h"

#include "Exception.h"
#include "Synchronization.h"
#include "Array.h"
#include "Monitor.h"

namespace Sys
{

// Global variables

PointerArray g_SyncBlockPool;

// Monitor

ThreadMutex* Monitor::CreateThreadMutex(Object* obj)
{
  ThreadMutexLockScope lock(g_GlobalMutex);
  if (obj->m_SyncBlockIndex < 0) // check it once again when g_GlobalMutex is locked
  {
    int i = 0;
    while (i < g_SyncBlockPool.Count() && g_SyncBlockPool.Item(i) != NULL)
      ++i;
    if (i < g_SyncBlockPool.Count())
    {
      ThreadMutex* result = new ThreadMutex();
      g_SyncBlockPool.SetItem(i, result);
      obj->m_SyncBlockIndex = i;
      return result;
    }
    else
      throw CreateSystemResourceException();
  }
  else
    return (ThreadMutex*)g_SyncBlockPool.Item(obj->m_SyncBlockIndex);
}

void Monitor::DestroyThreadMutex(Object* obj)
{
  if (obj->m_SyncBlockIndex >= 0)
  {
    // DestroyThreadMutex is called from Object::FreeMemory, 
    // which runs within DestroyScope, i.e. g_GlobalMutex lock.
    //Lock lock(g_GlobalMutex);
    delete (ThreadMutex*)g_SyncBlockPool.Item(obj->m_SyncBlockIndex);
    g_SyncBlockPool.SetItem(obj->m_SyncBlockIndex, NULL);
  }
}

ThreadMutex* Monitor::GetThreadMutex(Object* obj)
{
  if (obj->m_SyncBlockIndex >= 0)
    return (ThreadMutex*)g_SyncBlockPool.Item(obj->m_SyncBlockIndex);
  else
    throw CreateSynchronizationLockException();
}

ThreadMutex* Monitor::AcquireThreadMutex(Object* obj)
{
  if (obj->m_SyncBlockIndex < 0)
    return Monitor::CreateThreadMutex(obj);
  else
    return Monitor::GetThreadMutex(obj);
}

void Monitor::Enter(Object* obj)
{
  if (obj != NULL)
    Monitor::AcquireThreadMutex(obj)->Lock();
  else
    throw CreateArgumentNullException();
}

void Monitor::Exit(Object* obj)
{
  if (obj != NULL)
    Monitor::GetThreadMutex(obj)->Unlock();
  else
    throw CreateArgumentNullException();
}

// SynchronizationLockException

SynchronizationLockException::SynchronizationLockException(const std::string& message) :
  SystemException(message)
{
}

SynchronizationLockException::SynchronizationLockException(const std::string& message, Exception* innerException) :
  SystemException(message, innerException)
{
}

SynchronizationLockException* CreateSynchronizationLockException()
{
  return new SynchronizationLockException("the current thread does not own the lock for the specified object");
}

// Global functions

int GetSyncBlockCount()
{
  ThreadMutexLockScope lock(g_GlobalMutex);
  int result = 0;
  for (int i = 0; i < g_SyncBlockPool.Count(); ++i)
  {
    if (Sys::g_SyncBlockPool.Item(i) != NULL)
      ++result;
  }
  return result;
}

} // namespace Sys
