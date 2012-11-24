#include <tchar.h>

#include "db_wrapper.h"
#include <stdio.h>

DB_Wrapper::DB_Wrapper(LPCTSTR path)
{
  printf("Loading library\n");
  dll_handle = LoadLibrary(path);
  printf("Library loaded!\n");
  //_tprintf(TEXT("%s: 0x%0.8x\n"), path, dll_handle);

  find_abonents = (find_abonents_func)
    GetProcAddress(dll_handle, "find_abonents");

  get_by_id = (get_by_id_func)
    GetProcAddress(dll_handle, "get_by_id");

  update_abonent = (update_abonent_func)
    GetProcAddress(dll_handle, "update_abonent");

  insert_abonent = (insert_abonent_func)
    GetProcAddress(dll_handle, "insert_abonent");

  remove_abonent = (remove_abonent_func)
    GetProcAddress(dll_handle, "remove_abonent");

  last_error = (last_error_func)
    GetProcAddress(dll_handle, "last_error");
}

DB_Wrapper::~DB_Wrapper()
{
  CloseHandle(dll_handle);
};
