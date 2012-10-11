#ifndef WORKER_H
#define WORKER_H

#include <queue>
#include <vector>

#include <windows.h>

using namespace std;

typedef void (*FUNC_PTR)(void* data);

struct Func_descriptor
{
  int id;
  FUNC_PTR func;
};

struct Thread_params
{
  int id;
  queue<Func_descriptor>* task_queue;
};

class Worker
{
public:
  Worker(int id, queue<Func_descriptor>* task_queue);
  virtual ~Worker();

  int get_id();
  int get_thread_id();
  static DWORD WINAPI main_loop(void* data);

private:
  int id;
  DWORD thread_id;
  HANDLE thread_handle;
  queue<Func_descriptor>* task_queue;
};

#endif // WORKER_H
