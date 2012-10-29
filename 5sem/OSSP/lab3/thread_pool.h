#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "critical_section.h"

#include <windows.h>
#include <vector>

using namespace std;

struct Thread_params
{
  HANDLE closeEvent;
};

class ThreadPool
{
public:
  ThreadPool(int count);
  virtual ~ThreadPool();

  void kill_all();

private:
  HANDLE closeEvent;
  vector<HANDLE> thread_handles;

  static DWORD WINAPI main_loop(void* data);
};

#endif // THREAD_POOL_H
