#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "worker.h"

using namespace std;

class ThreadPool
{
public:
  ThreadPool(int workers_count);
  virtual ~ThreadPool();

  void add_task(FUNC_PTR func);
  void dec_workers_num(int workers_count);
  void suspend_all();
  void resume_all();

private:
  HANDLE* worker_thread_handles();

  queue<Func_descriptor>* task_queue;
  vector<Worker*>* workers;
  int last_task_id;
  HANDLE mutex;
};

#endif // THREAD_POOL_H
