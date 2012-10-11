#include "worker.h"
#include "console_io.h"

Worker::Worker(int id, queue<Func_descriptor>* task_queue) : id(id), task_queue(task_queue)
{
  Thread_params* tpp = (Thread_params*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Thread_params));
  tpp->id = id;
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
  TerminateThread(thread_handle, NULL);
}

DWORD Worker::main_loop(void* data)
{
  Thread_params* tpp = (Thread_params*) data;
  int id = tpp->id;
  queue<Func_descriptor>* task_queue = tpp->task_queue;
  TCHAR msg[1024];

  while (1)
  {
    Func_descriptor work_descr = {0, NULL};

    while ( work_descr.func == NULL )
    {
      // mutex_lock here
      if ( !task_queue->empty() )
      {
        work_descr = task_queue->front();
        task_queue->pop();
      }
      // mutex unlock here
      // sleep here
    }

    StringCchPrintf(msg, BUF_SIZE, TEXT("%d worker beginnig task %d!\n"), id, work_descr.id); 
    ConsoleIO::cprint(msg);

    work_descr.func(NULL);
  }
  return 0;
}
