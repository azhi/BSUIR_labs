#include <windows.h>

#include <conio.h>
#include <ctype.h>
#include <stdlib.h>

#include "thread_pool.h"
#include "console_io.h"

void task1(void* data)
{
  ConsoleIO::cprint(TEXT("Task 1 running...\n"));
  Sleep(2000);
}

void task2(void* data)
{
  ConsoleIO::cprint(TEXT("Task 2 running...\n"));
  Sleep(4000);
}

void task3(void* data)
{
  ConsoleIO::cprint(TEXT("Task 3 running...\n"));
  Sleep(6000);
}

int main(int argc, const char *argv[])
{
  ThreadPool* tp = new ThreadPool(5);
  int ch;
  while ((ch = _getch()) != 'q')
  {
    printf("%d\n", ch);
    switch (ch)
    {
    case '1':
      tp->add_task(&task1);
      break;

    case '2':
      tp->add_task(&task2);
      break;

    case '3':
      tp->add_task(&task3);
      break;

    case 's':
      tp->suspend_all();
      break;

    case 'r':
      tp->resume_all();
      break;
    }
  }
  tp->dec_workers_num(5);
  return 0;
}
