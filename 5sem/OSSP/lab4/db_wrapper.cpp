#include <tchar.h>

#include "db_wrapper.h"
#include <stdio.h>

DB_Wrapper::DB_Wrapper(LPCTSTR path)
{
  dll_handle = LoadLibrary(path);
  //_tprintf(TEXT("%s: 0x%0.8x\n"), path, dll_handle);

  find_abonents = (find_abonents_func)
    GetProcAddress(dll_handle, "find_abonents");

  get_by_id = (get_by_id_func)
    GetProcAddress(dll_handle, "get_by_id");

  last_error = (last_error_func)
    GetProcAddress(dll_handle, "last_error");
}

DB_Wrapper::~DB_Wrapper()
{
  CloseHandle(dll_handle);
};
