#include <windows.h>
#include <stdlib.h>

#include "thread_pool.h"
#include "console_io.h"

void task1(void* data)
{
  ConsoleIO::cprint(TEXT("Task 1 running...\n"));
}

void task2(void* data)
{
  ConsoleIO::cprint(TEXT("Task 2 running...\n"));
}

void task3(void* data)
{
  ConsoleIO::cprint(TEXT("Task 3 running...\n"));
}

int main(int argc, const char *argv[])
{
  ThreadPool* tp = new ThreadPool(5);
  tp->add_task(&task2);
  tp->add_task(&task1);
  tp->add_task(&task1);
  tp->add_task(&task3);
  for ( int i = 0; i < 1000; i++ )
    tp->add_task(&task2);
  tp->dec_workers_num(5);
  return 0;
}
