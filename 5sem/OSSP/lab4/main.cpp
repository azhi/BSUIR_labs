#include <stdio.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>

#include "DB.h"

typedef void (*hello_func)(const char*);
typedef int (*double_func)(int);
typedef void (*cpp_func)(void);

typedef BOOL (*init_func)();
typedef void (*finalize_func)();

typedef DWORD (*find_by_name_func)(DWORD *ids, DWORD max_num, LPCTSTR name);
typedef DWORD (*find_by_phone_no_func)(DWORD *ids, DWORD max_num, LPCTSTR phone_no);
typedef DWORD (*find_by_address_func)(DWORD *ids, DWORD max_num, LPCTSTR address);

typedef BOOL (*get_by_id_func)(DWORD id, Abonent *abonent);

typedef BOOL (*update_abonent_func)(Abonent *abonent);

typedef void (*insert_abonent_func)(Abonent *new_abonent);

typedef BOOL (*remove_abonent_func)(DWORD id);

int main(void)
{
  _setmode( _fileno( stdin ), _O_BINARY );
  _setmode( _fileno( stdout ), _O_BINARY );
  HINSTANCE dll_handle = LoadLibrary(TEXT("DB.dll"));
  init_func init = (init_func) GetProcAddress(dll_handle, "init");
  finalize_func finalize = (finalize_func) GetProcAddress(dll_handle, "finalize");
  find_by_name_func find_by_name = (find_by_name_func) GetProcAddress(dll_handle, "find_by_name");
  find_by_phone_no_func find_by_phone_no = (find_by_phone_no_func) GetProcAddress(dll_handle, "find_by_phone_no");
  find_by_address_func find_by_address = (find_by_address_func) GetProcAddress(dll_handle, "find_by_address");
  get_by_id_func get_by_id = (get_by_id_func) GetProcAddress(dll_handle, "get_by_id");
  update_abonent_func update_abonent = (update_abonent_func) GetProcAddress(dll_handle, "update_abonent");
  insert_abonent_func insert_abonent = (insert_abonent_func) GetProcAddress(dll_handle, "insert_abonent");
  remove_abonent_func remove_abonent = (remove_abonent_func) GetProcAddress(dll_handle, "remove_abonent");

  init();
  DWORD *ids = (DWORD*) malloc(sizeof(int) * 10);
  Abonent* ab = (Abonent*) malloc(sizeof(Abonent));

  ab->id = 0; ab->name = TEXT("Ivanov");
  ab->phone_no = TEXT("7658799"); ab->address = TEXT("On the street");
  insert_abonent(ab);
  ab->id = 0; ab->name = TEXT("Petrov");
  ab->phone_no = TEXT("5432189"); ab->address = TEXT("somewhere");
  insert_abonent(ab);
  ab->id = 0; ab->name = TEXT("Sidorov");
  ab->phone_no = TEXT("1298765"); ab->address = TEXT("somewhere");
  insert_abonent(ab);
  DWORD count = find_by_name(ids, 10, TEXT("Ivanov"));
  for (int i=0; i<count; i++)
  {
    ab->name = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->phone_no = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->address = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    get_by_id(ids[i], ab);
    _tprintf(TEXT("%d %s %s %s\n"), ab->id, ab->name, ab->phone_no, ab->address);
    free(ab->name);
    free(ab->phone_no);
    free(ab->address);
  }
  count = find_by_phone_no(ids, 10, TEXT("5432189"));
  for (int i=0; i<count; i++)
  {
    ab->name = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->phone_no = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->address = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    get_by_id(ids[i], ab);
    _tprintf(TEXT("%d %s %s %s\n"), ab->id, ab->name, ab->phone_no, ab->address);
    free(ab->name);
    free(ab->phone_no);
    free(ab->address);
  }
  remove_abonent(ids[0]);
  count = find_by_address(ids, 10, TEXT("somewhere"));
  if ( count > 1 )
  {
    ab->name = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->phone_no = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->address = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    get_by_id(ids[1], ab);
    ab->address = TEXT("not somewhere");
    update_abonent(ab);
    free(ab->name);
    free(ab->phone_no);
    free(ab->address);
  }
  for (int i=0; i<count; i++)
  {
    ab->name = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->phone_no = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    ab->address = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    get_by_id(ids[i], ab);
    _tprintf(TEXT("%d %s %s %s\n"), ab->id, ab->name, ab->phone_no, ab->address);
    free(ab->name);
    free(ab->phone_no);
    free(ab->address);
  }
  finalize();

  CloseHandle(dll_handle);
  return 0;
}
