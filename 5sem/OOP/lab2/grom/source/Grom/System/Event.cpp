// Free open-source Grom library

#include "PrecompiledHeader.h"

#include "Event.h"

namespace Sys
{

// Delegate

struct Delegate
{
  Delegate(DelegateProcedure* procedure, void* userData, Delegate* previous);

  DelegateProcedure* Procedure;
  void* UserData;
  Delegate* Previous;
};

Delegate::Delegate(DelegateProcedure* procedure, void* userData, Delegate* previous) : 
  Procedure(procedure), UserData(userData), Previous(previous)
{
}

void DeleteChainedDelegate(Delegate** d)
{
  Delegate* c = *d;
  *d = (*d)->Previous;
  delete c;
}

// EventImpl

EventImpl::EventImpl() : m_Handlers(NULL), m_InvokeInProgress(false)
{
}

void EventImpl::Attach(DelegateProcedure* procedure, void* userData)
{
  Delegate** d = Find(procedure, userData);
  if ((*d) == NULL)
    m_Handlers = new Delegate(procedure, userData, m_Handlers);
}

void EventImpl::Detach(DelegateProcedure* procedure, void* userData)
{
  Delegate** d = Find(procedure, userData);
  if ((*d) != NULL)
  {
    if (!m_InvokeInProgress)
      DeleteChainedDelegate(d);
    else
      (*d)->Procedure = NULL;
  }
}

void EventImpl::Invoke(void* args)
{
  if (args != NULL)
  {
    if (m_Handlers != NULL)
    {
      bool b = m_InvokeInProgress;
      m_InvokeInProgress = true;
      try
      {
        Sys::InvokeDefaultHandler(m_Handlers, args); // this is more effective, than this->InvokeDefaultHandler();
      }
      catch (...)
      {
        m_InvokeInProgress = b;
        if (!m_InvokeInProgress)
          DeleteDetachedDelegates();
        throw;
      }
      m_InvokeInProgress = b;
      if (!m_InvokeInProgress)
        DeleteDetachedDelegates();
    }
  }
  else
    throw CreateArgumentNullException();
}

Delegate** EventImpl::Find(DelegateProcedure* procedure, void* userData)
{
  Delegate** d = &m_Handlers;
  while (*d != NULL)
  {
    if ((*d)->Procedure == procedure && (*d)->UserData == userData)
      break;
    d = &(*d)->Previous;
  }
  return d;
}

void EventImpl::DeleteDetachedDelegates()
{
  Delegate** d = &m_Handlers;
  while (*d != NULL)
  {
    if ((*d)->Procedure == NULL)
      DeleteChainedDelegate(d);
    else
      d = &(*d)->Previous;
  }
}

void InvokeDefaultHandler(Delegate* delegateChain, void* args)
{
  // Ignore removed handlers
  Delegate* d = delegateChain;
  while (d != NULL && d->Procedure == NULL)
    d = d->Previous;
  // Invoke
  if (d != NULL)
    d->Procedure(args, d->UserData, d->Previous);
}

} // namespace Sys
