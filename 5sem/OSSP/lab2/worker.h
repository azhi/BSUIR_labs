#ifndef WORKER_H
#define WORKER_H

#include <queue>
#include <vector>

#include <windows.h>

using namespace std;

typedef void (*FUNC_PTR)(void* data);

enum CMD
{
  CM_NOTHING = 0,
  CM_RUN,
  CM_EXIT
};

struct Func_descriptor
{
  int id;
  CMD cmd;
  FUNC_PTR func;
};

struct Thread_params
{
  int id;
  HANDLE mutex;
  queue<Func_descriptor>* task_queue;
};

class Worker
{
public:
  Worker(int id, queue<Func_descriptor>* task_queue, HANDLE mutex);
  virtual ~Worker();

  int get_id();
  HANDLE get_thread_handle();
  static DWORD WINAPI main_loop(void* data);

  void suspend();
  void resume();

private:
  int id;
  DWORD thread_id;
  HANDLE thread_handle;
  queue<Func_descriptor>* task_queue;
};

#endif // WORKER_H
