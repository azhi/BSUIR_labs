#ifndef CONSOLE_IO_H
#define CONSOLE_IO_H

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>

#define BUF_SIZE 1024

class ConsoleIO
{
public:
  ConsoleIO();
  virtual ~ConsoleIO();

  static void cprint(LPTSTR msg);
  static LPTSTR cgets();

private:
  /* data */
};

#endif // CONSOLE_IO_H
