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

  try {
    DB_Wrapper db_w = DB_Wrapper(TEXT("DB.dll"));
    App(hInst).run(nCmdShow);
  } catch (...) {
    throw;
  }

  return 0;
}
