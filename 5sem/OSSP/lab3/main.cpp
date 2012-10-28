#include "critical_section.h"

#include <stdio.h>

DWORD WINAPI threadProc(void *data);

int main(int argc, char **argv)
{
  auto pCriticalSection(new CriticalSection(TEXT("mySection")));
  //Sleep(1000);
  pCriticalSection->enter();
  //Sleep(6000);
  delete pCriticalSection;
  return 0;
}
