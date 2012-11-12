#include <tchar.h>

#include "abonent.h"

Abonent* create_abonent(DWORD id=0, LPCTSTR phone_no=NULL, LPCTSTR family_name=NULL,
                        LPCTSTR name=NULL, LPCTSTR middle_name=NULL,
                        LPCTSTR street=NULL, LPCTSTR house=NULL,
                        LPCTSTR building=NULL, LPCTSTR flat=NULL)
{
  Abonent *abonent = (Abonent*) malloc(sizeof(Abonent));
  abonent->id = id;
  init_field(abonent->phone_no, phone_no);
  init_field(abonent->family_name, family_name);
  init_field(abonent->name, name);
  init_field(abonent->middle_name, middle_name);
  init_field(abonent->street, street);
  init_field(abonent->house, house);
  init_field(abonent->building, building);
  init_field(abonent->flat, flat);
  return abonent;
}

LPTSTR init_field(LPTSTR field, LPCTSTR value)
{
  field = (LPTSTR) calloc(sizeof(TCHAR), 256);
  if (value != NULL)
    _tcscpy(field, value);
}

void clear_abonent(Abonent* abonent)
{
  if (abonent != NULL)
  {
    free(abonent->phone_no);
    free(abonent->family_name);
    free(abonent->name);
    free(abonent->middle_name);
    free(abonent->street);
    free(abonent->house);
    free(abonent->building);
    free(abonent->flat);
    free(abonent);
  }
}
