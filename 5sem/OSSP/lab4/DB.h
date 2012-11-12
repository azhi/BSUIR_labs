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

DWORD FUNC_DECLARE find_abonents(DWORD *ids, DWORD max_num, Abonent* ab);

BOOL FUNC_DECLARE get_by_id(DWORD id, Abonent *abonent);

BOOL FUNC_DECLARE update_abonent(Abonent *abonent);

DWORD FUNC_DECLARE insert_abonent(Abonent *new_abonent);

BOOL FUNC_DECLARE remove_abonent(DWORD id);

BOOL FUNC_DECLARE APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

void init();
void finalize();

int reference_count = 0;

#ifdef BUILDING_DLL

typedef struct _List_element
{
  Abonent* abonent;
  _List_element* prev;
  _List_element* next;
} List_element;

typedef BOOL (*cmp_func)(const Abonent* a1, const Abonent* a2);

void add_to_list(Abonent* abonent);
List_element* find_element(List_element* begin, cmp_func cmp, Abonent* cmp_ab);
void remove_element(List_element* rem_el);

#endif

#ifdef __cplusplus
}
#endif

#endif  // DLL_H
