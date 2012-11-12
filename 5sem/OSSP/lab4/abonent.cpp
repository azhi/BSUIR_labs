#include <tchar.h>

#include "abonent.h"

Abonent* create_abonent(DWORD id, LPCTSTR name, LPCTSTR phone, LPCTSTR address)
{
  Abonent *abonent = (Abonent*) malloc(sizeof(Abonent));
  abonent->id = id;
  abonent->name = (LPTSTR) calloc(sizeof(TCHAR), 256);
  if (name != NULL)
    _tcscpy(abonent->name, name);
  abonent->phone_no = (LPTSTR) calloc(sizeof(TCHAR), 256);
  if (phone != NULL)
    _tcscpy(abonent->phone_no, phone);
  abonent->address = (LPTSTR) calloc(sizeof(TCHAR), 256);
  if (address != NULL)
    _tcscpy(abonent->address, address);
  return abonent;
}

void clear_abonent(Abonent* abonent)
{
  if (abonent != NULL)
  {
    free(abonent->name);
    free(abonent->phone_no);
    free(abonent->address);
    free(abonent);
  }
}
