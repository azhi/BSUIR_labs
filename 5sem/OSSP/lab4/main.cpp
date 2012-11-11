#include <tchar.h>
#include <windows.h>
//#include <io.h>

#include "app.h"
#include "DB.h"
#include "db_wrapper.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE _hPrevInst,
		   PSTR _lpCmdLine, int nCmdShow)
{
  //_setmode( _fileno( stdin ), _O_BINARY );
  //_setmode( _fileno( stdout ), _O_BINARY );

  DB_Wrapper db_w = DB_Wrapper(TEXT("DB.dll"));
  App(hInst).run(nCmdShow);

  DWORD *ids = (DWORD*) malloc(sizeof(int) * 10);
  Abonent* ab = (Abonent*) malloc(sizeof(Abonent));

  ab->id = 0; ab->name = TEXT("Ivanov");
  ab->phone_no = TEXT("7658799"); ab->address = TEXT("On the street");
  db_w.insert_abonent(ab);
  ab->id = 0; ab->name = TEXT("Petrov");
  ab->phone_no = TEXT("5432189"); ab->address = TEXT("somewhere");
  db_w.insert_abonent(ab);
  ab->id = 0; ab->name = TEXT("Sidorov");
  ab->phone_no = TEXT("1298765"); ab->address = TEXT("somewhere");
  db_w.insert_abonent(ab);
  _tprintf(TEXT("Finding by name:\n"));
  DWORD count = db_w.find_by_name(ids, 10, TEXT("Ivanov"));
  if ( count > 10 )
  {
    _tprintf(TEXT("Overflow, too much elements, taking first 10\n"));
    count = 10;
  }
  for (int i=0; i<count; i++)
  {
    ab->name = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->phone_no = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->address = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    db_w.get_by_id(ids[i], ab);
    _tprintf(TEXT("%d %s %s %s\n"), ab->id, ab->name, ab->phone_no, ab->address);
    free(ab->name);
    free(ab->phone_no);
    free(ab->address);
  }
  _tprintf(TEXT("Finding by phone:\n"));
  count = db_w.find_by_phone_no(ids, 10, TEXT("5432189"));
  if ( count > 10 )
  {
    _tprintf(TEXT("Overflow, too much elements, taking first 10\n"));
    count = 10;
  }
  for (int i=0; i<count; i++)
  {
    ab->name = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->phone_no = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->address = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    db_w.get_by_id(ids[i], ab);
    _tprintf(TEXT("%d %s %s %s\n"), ab->id, ab->name, ab->phone_no, ab->address);
    free(ab->name);
    free(ab->phone_no);
    free(ab->address);
  }
  _tprintf(TEXT("Removing:\n"));
  db_w.remove_abonent(ids[0]);
  _tprintf(TEXT("Finding by address & updating:\n"));
  count = db_w.find_by_address(ids, 10, TEXT("somewhere"));
  if ( count > 10 )
  {
    _tprintf(TEXT("Overflow, too much elements, taking first 10\n"));
    count = 10;
  }
  if ( count > 1 )
  {
    ab->name = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->phone_no = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->address = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    db_w.get_by_id(ids[1], ab);
    ab->address = TEXT("not somewhere");
    db_w.update_abonent(ab);
    free(ab->name);
    free(ab->phone_no);
    free(ab->address);
  }
  for (int i=0; i<count; i++)
  {
    ab->name = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->phone_no = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->address = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    db_w.get_by_id(ids[i], ab);
    _tprintf(TEXT("%d %s %s %s\n"), ab->id, ab->name, ab->phone_no, ab->address);
    free(ab->name);
    free(ab->phone_no);
    free(ab->address);
  }

  return 0;
}
