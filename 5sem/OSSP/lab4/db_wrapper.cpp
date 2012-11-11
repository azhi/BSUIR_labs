#include <tchar.h>

#include "db_wrapper.h"

DB_Wrapper::DB_Wrapper(LPCTSTR path)
{
  dll_handle = LoadLibrary(path);
  //_tprintf(TEXT("%s: 0x%0.8x\n"), path, dll_handle);
  init = (init_func) GetProcAddress(dll_handle, "init");
  finalize = (finalize_func) GetProcAddress(dll_handle, "finalize");

  find_by_name = (find_by_name_func)
    GetProcAddress(dll_handle, "find_by_name");

  find_by_phone_no = (find_by_phone_no_func)
    GetProcAddress(dll_handle, "find_by_phone_no");

  find_by_address = (find_by_address_func)
    GetProcAddress(dll_handle, "find_by_address");

  get_by_id = (get_by_id_func)
    GetProcAddress(dll_handle, "get_by_id");

  update_abonent = (update_abonent_func)
    GetProcAddress(dll_handle, "update_abonent");

  insert_abonent = (insert_abonent_func)
    GetProcAddress(dll_handle, "insert_abonent");

  remove_abonent = (remove_abonent_func)
    GetProcAddress(dll_handle, "remove_abonent");

  init();
}

DB_Wrapper::~DB_Wrapper()
{
  finalize();
  //_tprintf(TEXT("Closing handle 0x%0.8x\n"), dll_handle);
  CloseHandle(dll_handle);
};
