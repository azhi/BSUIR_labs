#ifndef DLL_H
#define DLL_H

#include <tchar.h>
#include <windows.h>

#include "abonent.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILDING_DLL
#define FUNC_DECLARE __declspec(dllexport)
#else
#define FUNC_DECLARE __declspec(dllimport)
#endif

#define ADDING_CAPACITY 10240

int reference_count = 0;

HANDLE hMap;
LPVOID lpMapView;
DWORD mapSize;
DWORD mapAddingCapacity;
BOOL error;

int last_id;

DWORD FUNC_DECLARE find_abonents(DWORD *ids, DWORD max_num, Abonent* ab);

BOOL FUNC_DECLARE get_by_id(DWORD id, Abonent *abonent);

BOOL FUNC_DECLARE update_abonent(Abonent *abonent);

DWORD FUNC_DECLARE insert_abonent(Abonent *new_abonent);

BOOL FUNC_DECLARE remove_abonent(DWORD id);

BOOL FUNC_DECLARE last_error() { return error; };

BOOL FUNC_DECLARE APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);


typedef DWORD (*cmp_func)(const Abonent* a1, const Abonent* a2);
void init();
void finalize();

Abonent* parse_abonent(TCHAR** pos, TCHAR* max_pos);
void find_records(TCHAR* from, TCHAR* to, cmp_func cmp, Abonent* cmp_ab, DWORD** level_ids, DWORD* counts, DWORD max_num, DWORD* count);
TCHAR* find_single_record(TCHAR* from, TCHAR* to, cmp_func cmp, Abonent* cmp_ab);

#ifdef __cplusplus
}
#endif

#endif  // DLL_H
