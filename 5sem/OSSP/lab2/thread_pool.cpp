#include "thread_pool.h"

Thread_pool::Thread_pool(int workers_count)
{
  task_queue = new queue<Func_descriptor>;
  workers = new vector<Worker*>;
  for (int i = 0; i < workers_count; i++)
    workers->push_back( new Worker(i, task_queue) );
  last_task_id = 0;
}

Thread_pool::~Thread_pool()
{
  delete workers;
  delete task_queue;
}

void Thread_pool::add_task(FUNC_PTR func)
{
  Func_descriptor fd = { last_task_id++, func };

  // mutex_lock here
  task_queue->push(fd);
  // mutex_unlock here
}
