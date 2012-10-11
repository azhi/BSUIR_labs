#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "worker.h"

using namespace std;

class Thread_pool
{
public:
  Thread_pool(int workers_count);
  virtual ~Thread_pool();

  void add_task(FUNC_PTR func);
  void suspend_all();
  void resume_all();

private:
  queue<Func_descriptor>* task_queue;
  vector<Worker*>* workers;
  int last_task_id;

};

#endif // THREAD_POOL_H
