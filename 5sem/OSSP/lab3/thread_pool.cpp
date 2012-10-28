#include "thread_pool.h"

#include <stdio.h>

ThreadPool::ThreadPool(int count)
{
  cs = new CriticalSection(TEXT("consoleioCS"));
  closeEvent = CreateEvent( 
      NULL,               // default security attributes
      TRUE,               // manual-reset event
      FALSE,              // initial state is nonsignaled
      TEXT("CloseEvent")  // object name
      ); 

  Thread_params* tpp = (Thread_params*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Thread_params));
  tpp->closeEvent = closeEvent;
  tpp->cs = cs;

  for (int i = 0; i < count; i++)
  {
    HANDLE thread_handle = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size
        main_loop,              // thread function name
        tpp,                    // argument to thread function
        0,                      // use default creation flags
        NULL);                  // returns the thread identifier
    thread_handles.push_back(thread_handle);
  }
}

ThreadPool::~ThreadPool()
{
  delete cs;
}

void ThreadPool::kill_all()
{
  printf("Waiting for several threads to stop...\n");

  SetEvent(closeEvent);

  do
  {
    HANDLE* hth = (HANDLE*) malloc(thread_handles.size() * sizeof(HANDLE*));
    for(int i = 0; i < thread_handles.size(); i++)
      hth[i] = thread_handles[i];
    DWORD dw_wait_result = WaitForMultipleObjects(thread_handles.size(), hth, FALSE, INFINITE);
    free(hth);
    if ((dw_wait_result >= WAIT_OBJECT_0) &&
	(dw_wait_result < WAIT_OBJECT_0 + thread_handles.size())) {
      thread_handles.erase(thread_handles.begin() + dw_wait_result - WAIT_OBJECT_0);
    }
  } while(thread_handles.size());
}

DWORD ThreadPool::main_loop(void* data)
{
  Thread_params* tpp = (Thread_params*) data;
  HANDLE closeEvent = tpp->closeEvent;
  CriticalSection* cs = tpp->cs;
  TCHAR msg[1024];

  while (1)
  {
    cs->enter();
    for(int i = 0; i < 5; i++)
      printf("%d printing something to the console!\n", GetCurrentThreadId());
    cs->leave();

    DWORD wait_result = WaitForSingleObject(closeEvent, 0);
    if ( wait_result == WAIT_OBJECT_0 )
      ExitThread(0);
  }
  return 0;
}
