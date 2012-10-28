#include "critical_section.h"

#include "common.h"

#include <stdio.h>

CriticalSection::CriticalSection(LPCTSTR szName)
  : bOccupied(false)
{
  try{
    hMMFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szName);
    if (!hMMFile)
      CreateSection(szName);
    lpCriticalSection = (LPCRITICAL_SECTION)
      MapViewOfFile(hMMFile, FILE_MAP_ALL_ACCESS, 0, 0, iFileSize);

    asrt(!lpCriticalSection, TEXT("Can not map the shared file."));
  } catch (...) {
    CloseHandle(hMMFile);
    hMMFile = NULL;
    throw;
  }
  printf("CS: 0x%0.16llx%0.16llx%0.16llx\n", *lpCriticalSection, *(lpCriticalSection + 8), *(lpCriticalSection + 16));
}

CriticalSection::~CriticalSection()
{
  if(bOccupied)
    leave();
  UnmapViewOfFile(lpCriticalSection);
  CloseHandle(hMMFile);
  hMMFile = NULL;
}

void CriticalSection::CreateSection(LPCTSTR szName)
{
  printf("Creating a shared file\n");
  hMMFile =
    CreateFileMapping(INVALID_HANDLE_VALUE, // use paging file
		      NULL,                 // default security
		      PAGE_READWRITE,       // read/write access
		      0,                    // maximum object size hi
		      iFileSize,            // , lo
		      szName);              // name of mapping object
  asrt(!hMMFile, TEXT("Can not create a shared file.."));

  lpCriticalSection = (LPCRITICAL_SECTION)
    MapViewOfFile(hMMFile, FILE_MAP_ALL_ACCESS, 0, 0, iFileSize);

  asrt(!lpCriticalSection, TEXT("Can not map the shared file."));

  InitializeCriticalSection(lpCriticalSection);
}

void CriticalSection::enter(void)
{
  if (bOccupied)
    return;
  printf("Entering critical section (%0.8x) ... ", lpCriticalSection);
  EnterCriticalSection(lpCriticalSection);
  bOccupied = true;
  printf("done.\n");
}

void CriticalSection::leave(void)
{
  if (!bOccupied)
    return;
  printf("Leaving critical section (%0.8x) ... ", lpCriticalSection);
  LeaveCriticalSection(lpCriticalSection);
  bOccupied = false;
  printf("done.\n");
}

LPCRITICAL_SECTION CriticalSection::getCriticalSection(void)
{
  return lpCriticalSection;
}
