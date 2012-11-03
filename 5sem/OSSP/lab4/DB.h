#ifndef DLL_H
#define DLL_H

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

BOOL FUNC_DECLARE init();

DWORD FUNC_DECLARE find_by_name(DWORD *ids, DWORD max_num, LPCTSTR name);
DWORD FUNC_DECLARE find_by_phone_no(DWORD *ids, DWORD max_num, LPCTSTR phone_no);
DWORD FUNC_DECLARE find_by_address(DWORD *ids, DWORD max_num, LPCTSTR adress);

BOOL FUNC_DECLARE get_by_id(DWORD id, Abonent *abonent);

BOOL FUNC_DECLARE update_abonent(Abonent *abonent);

DWORD FUNC_DECLARE insert_abonent(Abonent *new_abonent);

BOOL FUNC_DECLARE remove_abonent(DWORD id);

#ifdef __cplusplus
}
#endif

#endif  // DLL_H
