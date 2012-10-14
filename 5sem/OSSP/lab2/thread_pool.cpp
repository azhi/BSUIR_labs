#include "thread_pool.h"

ThreadPool::ThreadPool(int workers_count)
{
  mutex = CreateMutex(NULL,  // default security attributes
		      FALSE, // initially not owned
		      NULL); // unnamed
  if (!mutex) throw;
  task_queue = new queue<Func_descriptor>;
  workers = new vector<Worker*>;
  for (int i = 0; i < workers_count; i++)
    workers->push_back( new Worker(i, task_queue, mutex) );
  last_task_id = 0;
}

ThreadPool::~ThreadPool()
{
  delete workers;
  delete task_queue;
  CloseHandle(mutex);
}

void ThreadPool::add_task(FUNC_PTR func)
{
  Func_descriptor fd = { last_task_id++, CM_RUN, func };

  DWORD dw_wait_result = WaitForSingleObject(mutex, INFINITE);
  task_queue->push(fd);
  ReleaseMutex(mutex);
}

void ThreadPool::dec_workers_num(int workers_count)
{
  if (workers_count < 1)
    return;

  Func_descriptor fd = { last_task_id++, CM_EXIT, NULL };

  DWORD dw_wait_result = WaitForSingleObject(mutex, INFINITE);
  for (int i = 0; i < workers_count; ++i)
    task_queue->push(fd);
  ReleaseMutex(mutex);

  do
  {
    HANDLE* hth = worker_thread_handles();
    dw_wait_result = WaitForMultipleObjects(workers->size(), hth, FALSE, INFINITE);
    free(hth);
    if ((dw_wait_result >= WAIT_OBJECT_0) &&
	(dw_wait_result < WAIT_OBJECT_0 + workers->size())) {
      --workers_count;
      workers->erase(workers->begin() + dw_wait_result - WAIT_OBJECT_0);
    }
  } while(workers_count);

}

HANDLE* ThreadPool::worker_thread_handles()
{
  HANDLE* res = (HANDLE*) malloc(workers->size() * sizeof(HANDLE));
  for (int i = 0; i < workers->size(); ++i)
    res[i] = (*workers)[i]->get_thread_handle();
  return res;
}
