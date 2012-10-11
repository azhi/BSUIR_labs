#include <windows.h>
#include <stdlib.h>

#include "thread_pool.h"
#include "console_io.h"

void task1(void* data)
{
  ConsoleIO::cprint("Task 1 running...\n");
}

void task2(void* data)
{
  ConsoleIO::cprint("Task 2 running...\n");
}

void task3(void* data)
{
  ConsoleIO::cprint("Task 3 running...\n");
}

int main(int argc, const char *argv[])
{
  Thread_pool* tp = new Thread_pool(5);
  tp->add_task(&task2);
  tp->add_task(&task1);
  tp->add_task(&task1);
  tp->add_task(&task3);
  for ( int i = 0; i < 10000; i++ )
    tp->add_task(&task2);
  while (1)
    ;
  return 0;
}
