#ifndef _ABONENT_H_
#define _ABONENT_H_

#include <windows.h>

typedef struct _Abonent
{
  DWORD id;
  LPTSTR name;
  LPTSTR phone_no;
  LPTSTR address;
} Abonent;

Abonent* create_abonent(DWORD id=0, LPCTSTR name=NULL, LPCTSTR phone=NULL,
                        LPCTSTR address=NULL);
void clear_abonent(Abonent* abonent);

#endif /* _ABONENT_H_ */
