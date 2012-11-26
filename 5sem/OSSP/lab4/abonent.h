#ifndef _ABONENT_H_
#define _ABONENT_H_

#include <windows.h>

typedef struct _Abonent
{
  DWORD id;
  LPTSTR phone_no;
  LPTSTR family_name;
  LPTSTR name;
  LPTSTR middle_name;
  LPTSTR street;
  LPTSTR house;
  LPTSTR building;
  LPTSTR flat;
} Abonent;

Abonent* create_abonent(DWORD id=0, LPCTSTR phone_no=NULL,
    LPCTSTR family_name=NULL,
    LPCTSTR name=NULL, LPCTSTR middle_name=NULL,
    LPCTSTR street=NULL, LPCTSTR house=NULL,
    LPCTSTR building=NULL, LPCTSTR flat=NULL);
void clear_abonent(Abonent* abonent);

Abonent **waste_my_memory(size_t num=1024);
void i_want_my_memory_back1(Abonent **abonents, size_t num=1024);

#endif /* _ABONENT_H_ */
