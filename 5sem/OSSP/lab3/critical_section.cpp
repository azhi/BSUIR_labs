#include "critical_section.h"

#include "common.h"

CriticalSection::CriticalSection(LPTSTR szName)
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
  }
}

CriticalSection::~CriticalSection()
{
  if(bOccupied)
    leave();
  UnmapViewOfFile(lpCriticalSection);
  CloseHandle(hMMFile);
  hMMFile = NULL;
}

void CriticalSection::CreateSection(LPTSTR szName)
{
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

  lpCriticalSection = (LPCRITICAL_SECTION) malloc(iFileSize);
  InitializeCriticalSection(lpCriticalSection);
}

void CriticalSection::enter(void)
{
  if (bOccupied)
    return;
  EnterCriticalSection(lpCriticalSection);
  bOccupied = true;
}

void CriticalSection::leave(void)
{
  if (!bOccupied)
    return;
  LeaveCriticalSection(lpCriticalSection);
  bOccupied = false;
}

LPCRITICAL_SECTION CriticalSection::getCriticalSection(void)
{
  return lpCriticalSection;
}
