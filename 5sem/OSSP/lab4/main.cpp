#include <windef.h>
#include <tchar.h>
#include <windows.h>
//#include <io.h>

#include "app.h"
#include "abonent.h"
#include "db_wrapper.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE _hPrevInst,
                   PSTR _lpCmdLine, int nCmdShow)
{
  //_setmode( _fileno( stdin ), _O_BINARY );
  //_setmode( _fileno( stdout ), _O_BINARY );

  try {
    DB_Wrapper db_w = DB_Wrapper(TEXT("DB.dll"));
    if ( db_w.last_error() )
      throw;

    App(hInst).run(nCmdShow);

    Abonent* ab = create_abonent(0, TEXT("1234"), NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    Abonent** abonents = (Abonent**) malloc(sizeof(Abonent*) * 1000);
    for ( int i = 0; i < 1000; ++i )
      abonents[i] = create_abonent();
    DWORD count = db_w.find_abonents(abonents, 1000, ab);
    _tprintf(TEXT("Count=%d\n"), count);
    for ( int i = 0; i < (count > 1000 ? 1000 : count); ++i )
    {
      _tprintf(TEXT("%d %s %s %s %s %s %s %s %s\n"), abonents[i]->id,
               abonents[i]->phone_no, abonents[i]->family_name,
               abonents[i]->name, abonents[i]->middle_name,
               abonents[i]->street, abonents[i]->house,
               abonents[i]->building, abonents[i]->flat);
    }
    for ( int i = 0; i < 1000; ++i )
      clear_abonent(abonents[i]);
    free(abonents);
    clear_abonent(ab);
  } catch (...) {
    throw;
  }


  return 0;
}
