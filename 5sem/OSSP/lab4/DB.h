#ifndef DLL_H
#define DLL_H

#include <tchar.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILDING_DLL
#define FUNC_DECLARE __declspec(dllexport)
#else
#define FUNC_DECLARE __declspec(dllimport)
#endif

typedef struct _Abonent
{
  DWORD id;
  LPTSTR name;
  LPTSTR phone_no;
  LPTSTR address;
} Abonent;

void FUNC_DECLARE init();
void FUNC_DECLARE finalize();

DWORD FUNC_DECLARE find_by_name(DWORD *ids, DWORD max_num, LPCTSTR name);
DWORD FUNC_DECLARE find_by_phone_no(DWORD *ids, DWORD max_num, LPCTSTR phone_no);
DWORD FUNC_DECLARE find_by_address(DWORD *ids, DWORD max_num, LPCTSTR address);

BOOL FUNC_DECLARE get_by_id(DWORD id, Abonent *abonent);

BOOL FUNC_DECLARE update_abonent(Abonent *abonent);

DWORD FUNC_DECLARE insert_abonent(Abonent *new_abonent);

BOOL FUNC_DECLARE remove_abonent(DWORD id);


typedef struct _List_element
{
  Abonent* abonent;
  _List_element* prev;
  _List_element* next;
} List_element;

typedef BOOL (*cmp_func)(const Abonent* a1, const Abonent* a2);

Abonent* create_abonent(DWORD id, LPCTSTR name, LPCTSTR phone, LPCTSTR address);
void clear_abonent(Abonent* abonent);
void add_to_list(Abonent* abonent);
List_element* find_element(List_element* begin, cmp_func cmp, Abonent* cmp_ab);
void remove_element(List_element* rem_el);

#ifdef __cplusplus
}
#endif

#endif  // DLL_H
