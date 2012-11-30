// Free open-source Grom library

#include "PrecompiledHeader.h"

#include <errno.h>
#include "ErrorMessages.h"
#include "Utils.h"
#include "StringUtils.h"
#include "Synchronization.h"

#ifdef CFG_POSIX
long InterlockedIncrement(long volatile* addend)
{
  Sys::ThreadMutexLockScope lock(Sys::g_GlobalMutex);
  return ++(*addend);
}

long InterlockedDecrement(long volatile* addend)
{
  Sys::ThreadMutexLockScope lock(Sys::g_GlobalMutex);
  return --(*addend);
}

long InterlockedExchange(long volatile* target, long value)
{
  Sys::ThreadMutexLockScope lock(Sys::g_GlobalMutex);
  long prev = *target;
  *target = value;
  return prev;
}
#endif // CFG_POSIX

namespace Sys
{

ThreadMutex g_GlobalMutex;

// ThreadMutex

ThreadMutex::ThreadMutex()
{
#ifdef CFG_WIN32
  try
  {
    ::InitializeCriticalSection(&m_Mutex);
  }
  catch (...)
  {
    throw CreateSystemResourceException();
  }
#endif // CFG_WIN32
#ifdef CFG_POSIX
  pthread_mutexattr_t attr;
  if (pthread_mutexattr_init(&attr) != 0)
    throw CreateSystemResourceException();
  try
  {
    if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0)
      RunTimeAssert(false);
    if (pthread_mutex_init(&m_Mutex, &attr) != 0)
      throw CreateSystemResourceException();
  }
  catch (...)
  {
    pthread_mutexattr_destroy(&attr);
    throw;
  }
  pthread_mutexattr_destroy(&attr);
#endif // CFG_POSIX
}

ThreadMutex::~ThreadMutex()
{
#ifdef CFG_WIN32
  ::DeleteCriticalSection(&m_Mutex);
#endif // CFG_WIN32
#ifdef CFG_POSIX
  pthread_mutex_destroy(&m_Mutex);
#endif // CFG_POSIX
}

void ThreadMutex::Lock()
{
#ifdef CFG_WIN32
  ::EnterCriticalSection(&m_Mutex);
#endif // CFG_WIN32
#ifdef CFG_POSIX
  pthread_mutex_lock(&m_Mutex);
#endif // CFG_POSIX
}

void ThreadMutex::Unlock()
{
#ifdef CFG_WIN32
  ::LeaveCriticalSection(&m_Mutex);
#endif // CFG_WIN32
#ifdef CFG_POSIX
  pthread_mutex_unlock(&m_Mutex);
#endif // CFG_POSIX
}

// ThreadMutexLockScope

ThreadMutexLockScope::ThreadMutexLockScope(ThreadMutex* mutex, bool lockNow) :
  m_Mutex(mutex), m_IsLocked(false)
{
  if (lockNow)
    LockNow();
}

ThreadMutexLockScope::ThreadMutexLockScope(ThreadMutex& mutex, bool lockNow) :
  m_Mutex(&mutex), m_IsLocked(false)
{
  if (lockNow)
    LockNow();
}

ThreadMutexLockScope::~ThreadMutexLockScope()
{
  if (m_IsLocked)
    UnlockNow();
}

void ThreadMutexLockScope::LockNow()
{
  if (!m_IsLocked)
  {
    m_Mutex->Lock();
    m_IsLocked = true;
  }
}

void ThreadMutexLockScope::UnlockNow()
{
  if (m_IsLocked)
  {
    m_Mutex->Unlock();
    m_IsLocked = false;
  }
}

// Global functions

SystemException* CreateSystemResourceException()
{
  return new SystemException(FormattedString(ERRMSG_SYSTEM_RESOURCE_UNAVAILABLE));
}

} // namespace Sys
