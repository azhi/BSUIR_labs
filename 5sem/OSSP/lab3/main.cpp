#include <windows.h>

#include <stdio.h>
#include "thread_pool.h"

DWORD WINAPI threadProc(void *data);

int main(int argc, char **argv)
{
  // auto pCriticalSection(new CriticalSection(TEXT("mySection")));
  // //Sleep(1000);
  // pCriticalSection->enter();
  // //Sleep(6000);
  // delete pCriticalSection;

  ThreadPool* tp = new ThreadPool(2);
  Sleep(2000);
  tp->kill_all();
  return 0;
}
