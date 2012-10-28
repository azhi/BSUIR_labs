#ifndef _CRITICAL_SECTION_H_
#define _CRITICAL_SECTION_H_

#include <windows.h>


class CriticalSection
{
 public:
  CriticalSection(LPTSTR name);
  ~CriticalSection();

  LPCRITICAL_SECTION getCriticalSection(void);
  void enter(void);
  void leave(void);

 private:
  void CreateSection(LPTSTR szName);

  LPCRITICAL_SECTION lpCriticalSection;
  char bOccupied;
  HANDLE hMMFile;

  static const unsigned iFileSize = sizeof(CRITICAL_SECTION);
};

#endif /* _CRITICAL_SECTION_H_ */
