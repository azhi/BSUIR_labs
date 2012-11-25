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

#define MAX_THREADS 15

int reference_count;

HANDLE hMap;
LPVOID lpMapView;
DWORD mapSize;
BOOL error;

DWORD FUNC_DECLARE find_abonents(Abonent** abonents, DWORD max_num, Abonent* ab);

BOOL FUNC_DECLARE get_by_id(DWORD id, Abonent *abonent);

BOOL FUNC_DECLARE last_error() { return error; };

BOOL FUNC_DECLARE APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

typedef DWORD (*cmp_func)(const Abonent* a1, const Abonent* a2);

struct Thread_resources
{
  Abonent*** level_abonents;
  DWORD* counts;
};

struct Thread_params
{
  TCHAR* from;
  TCHAR* to;
  cmp_func cmp;
  Abonent* cmp_ab;
  Abonent*** level_abonents;
  DWORD* counts;
  DWORD max_num;
  DWORD* count;
  Abonent** res_ab;
  BOOL* finish;
};


void init();
void finalize();

Abonent* parse_abonent(TCHAR** pos, TCHAR* max_pos);
void copy_abonent(Abonent* dest, Abonent* src);
DWORD WINAPI find_records(void* data);
DWORD WINAPI find_single_record(void* data);

#ifdef __cplusplus
}
#endif

#endif  // DLL_H
