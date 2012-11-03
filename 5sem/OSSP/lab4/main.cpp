#include <stdio.h>
#include <windows.h>
#include "DB.h"

typedef void (*hello_func)(const char*);
typedef int (*double_func)(int);
typedef void (*cpp_func)(void);

typedef BOOL (*init_func)();

typedef DWORD (*find_by_name_func)(DWORD *ids, DWORD max_num, LPCTSTR name);
typedef DWORD (*find_by_phone_no_func)(DWORD *ids, DWORD max_num, LPCTSTR phone_no);
typedef DWORD (*find_by_address_func)(DWORD *ids, DWORD max_num, LPCTSTR adress);

typedef BOOL (*get_by_id_func)(DWORD id, Abonent *abonent);

typedef BOOL (*update_abonent_func)(Abonent *abonent);

typedef DWORD (*insert_abonent_func)(Abonent *new_abonent);

typedef BOOL (*remove_abonent_func)(DWORD id);

int main(void)
{
  HINSTANCE dll_handle = LoadLibrary("DB.dll");
  init_func init = (init_func) GetProcAddress(dll_handle, TEXT("init"));
  find_by_name_func find_by_name = (find_by_name_func) GetProcAddress(dll_handle, TEXT("find_by_name"));
  find_by_phone_no_func find_by_phone_no = (find_by_phone_no_func) GetProcAddress(dll_handle, TEXT("find_by_phone_no"));
  find_by_address_func find_by_address = (find_by_address_func) GetProcAddress(dll_handle, TEXT("find_by_address"));
  get_by_id_func get_by_id = (get_by_id_func) GetProcAddress(dll_handle, TEXT("get_by_id"));
  update_abonent_func update_abonent = (update_abonent_func) GetProcAddress(dll_handle, TEXT("update_abonent"));
  insert_abonent_func insert_abonent = (insert_abonent_func) GetProcAddress(dll_handle, TEXT("insert_abonent"));
  remove_abonent_func remove_abonent = (remove_abonent_func) GetProcAddress(dll_handle, TEXT("remove_abonent"));

  init();
  find_by_name(NULL, 10, "test");
  find_by_phone_no(NULL, 10, "test");
  find_by_address(NULL, 10, "test");
  get_by_id(5, NULL);
  update_abonent(NULL);
  insert_abonent(NULL);
  remove_abonent(2);

  CloseHandle(dll_handle);
  return 0;
}
