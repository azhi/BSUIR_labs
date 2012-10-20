#include "console_io.h"
#include "worker.h"

Worker::Worker(int id, queue<Func_descriptor>* task_queue, HANDLE mutex)
  : id(id), task_queue(task_queue)
{
  Thread_params* tpp = (Thread_params*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Thread_params));
  tpp->id = id;
  tpp->mutex = mutex;
  tpp->task_queue = task_queue;

  thread_handle = CreateThread(
      NULL,                   // default security attributes
      0,                      // use default stack size
      main_loop,              // thread function name
      tpp,                    // argument to thread function
      0,                      // use default creation flags
      &thread_id);            // returns the thread identifier
}

Worker::~Worker()
{
  CloseHandle(thread_handle);
}

HANDLE Worker::get_thread_handle()
{
  return thread_handle;
}

void Worker::suspend()
{
  SuspendThread(get_thread_handle());
}

void Worker::resume()
{
  ResumeThread(get_thread_handle());
}

DWORD Worker::main_loop(void* data)
{
  Thread_params* tpp = (Thread_params*) data;
  int id = tpp->id;
  HANDLE gh_mutex = tpp->mutex;
  queue<Func_descriptor>* task_queue = tpp->task_queue;
  TCHAR msg[1024];

  while (1)
  {
    Func_descriptor work_descr = {0, CM_NOTHING, NULL};

    while ( work_descr.cmd == CM_NOTHING )
    {
      DWORD dw_wait_result = WaitForSingleObject(gh_mutex, INFINITE);
      if ( !task_queue->empty() )
      {
        work_descr = task_queue->front();
        task_queue->pop();
      }
      ReleaseMutex(gh_mutex);
      Sleep(10);
    }
    switch(work_descr.cmd)
    {
    case CM_RUN:
      StringCchPrintf(msg, BUF_SIZE, TEXT("%d worker beginning task %d!\n"), id, work_descr.id);
      ConsoleIO::cprint(msg);

      work_descr.func(NULL);
      break;
    case CM_EXIT:
      ExitThread(0);
      break;
    }
  }
  return 0;
}
