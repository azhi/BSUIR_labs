#include "DB.h"

#include <stdio.h>

DWORD cmp_id(const Abonent* a1, const Abonent* a2)
{
  return a1->id == a2->id;
}

#define str_equal(s1, s2)\
  (_tcscmp(s1, s2) == 0)

#define str_similar(s1, s2)\
  (_tcsstr(s1, s2) != NULL)

#define abonent_all_equal(ab1, ab2)\
  str_equal(ab1->phone_no, ab2->phone_no) &&\
  str_equal(ab1->family_name, ab2->family_name) &&\
  str_equal(ab1->name, ab2->name) &&\
  str_equal(ab1->middle_name, ab2->middle_name) &&\
  str_equal(ab1->street, ab2->street) &&\
  str_equal(ab1->house, ab2->house) &&\
  str_equal(ab1->building, ab2->building) &&\
  str_equal(ab1->flat, ab2->flat)

#define abonent_some_equal(ab1, ab2)\
  str_equal(ab1->phone_no, ab2->phone_no) ||\
  str_equal(ab1->family_name, ab2->family_name) ||\
  str_equal(ab1->name, ab2->name) ||\
  str_equal(ab1->middle_name, ab2->middle_name) ||\
  str_equal(ab1->street, ab2->street) ||\
  str_equal(ab1->house, ab2->house) ||\
  str_equal(ab1->building, ab2->building) ||\
  str_equal(ab1->flat, ab2->flat)

#define abonent_all_similar(ab1, ab2)\
  ( ab2->phone_no[0] == '\0' || ( ab2->phone_no[0] != '\0' && str_similar(ab1->phone_no, ab2->phone_no) ) ) &&\
  ( ab2->family_name[0] == '\0' || ( ab2->family_name[0] != '\0' && str_similar(ab1->family_name, ab2->family_name) ) ) &&\
  ( ab2->name[0] == '\0' || ( ab2->name[0] != '\0' && str_similar(ab1->name, ab2->name) ) ) &&\
  ( ab2->middle_name[0] == '\0' || ( ab2->middle_name[0] != '\0' && str_similar(ab1->middle_name, ab2->middle_name) ) ) &&\
  ( ab2->street[0] == '\0' || ( ab2->street[0] != '\0' && str_similar(ab1->street, ab2->street) ) ) &&\
  ( ab2->house[0] == '\0' || ( ab2->house[0] != '\0' && str_similar(ab1->house, ab2->house) ) ) &&\
  ( ab2->building[0] == '\0' || ( ab2->building[0] != '\0' && str_similar(ab1->building, ab2->building) ) ) &&\
  ( ab2->flat[0] == '\0' || ( ab2->flat[0] != '\0' && str_similar(ab1->flat, ab2->flat) ) )

#define abonent_some_similar(ab1, ab2)\
  ( ab2->phone_no[0] != '\0' && str_similar(ab1->phone_no, ab2->phone_no) ) ||\
  ( ab2->family_name[0] != '\0' && str_similar(ab1->family_name, ab2->family_name) ) ||\
  ( ab2->name[0] != '\0' && str_similar(ab1->name, ab2->name) ) ||\
  ( ab2->middle_name[0] != '\0' && str_similar(ab1->middle_name, ab2->middle_name) ) ||\
  ( ab2->street[0] != '\0' && str_similar(ab1->street, ab2->street) ) ||\
  ( ab2->house[0] != '\0' && str_similar(ab1->house, ab2->house) ) ||\
  ( ab2->building[0] != '\0' && str_similar(ab1->building, ab2->building) ) ||\
  ( ab2->flat[0] != '\0' && str_similar(ab1->flat, ab2->flat) )


DWORD cmp(const Abonent* a1, const Abonent* a2)
{
  if ( abonent_all_equal(a1, a2) )
    return 4;
  if ( abonent_some_equal(a1, a2) )
    return 3;
  if ( abonent_all_similar(a1, a2) )
    return 2;
  if ( abonent_some_similar(a1, a2) )
    return 1;
  return 0;
}

#undef str_equal
#undef str_similar
#undef abonent_all_equal
#undef abonent_some_equal
#undef abonent_all_similar
#undef abonent_some_similar

void init()
{
  error = false;
  HANDLE hFile=CreateFile(TEXT("phones.db"), GENERIC_READ | GENERIC_WRITE,
                          FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 
                          FILE_ATTRIBUTE_NORMAL, NULL);
  if ( hFile == INVALID_HANDLE_VALUE )
  {
    error = true;
    finalize();
    return;
  }

  mapSize = GetFileSize(hFile, 0);
  hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
  CloseHandle(hFile);
  if ( !hMap )
  {
    error = true;
    finalize();
    return;
  }

  lpMapView = MapViewOfFile(hMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
  if ( !lpMapView )
  {
    error = true;
    finalize();
    return;
  }
}

void finalize()
{
  if ( !error )
    FlushViewOfFile(lpMapView, mapSize);
  if ( hMap )
    CloseHandle(hMap);
  if ( lpMapView )
    UnmapViewOfFile(lpMapView);
}

void get_next_el(TCHAR** buf, LPTSTR res, TCHAR* max_pos)
{
  int count = 0;
  while ( (*buf) < max_pos && (**buf) != '\n' && (**buf) != ',')
  {
    res[count++] = **buf;
    (*buf)++;
  }
  res[count] = '\0';
}

void skip_delimiters(TCHAR** buf, TCHAR* max_pos)
{
  while ( (*buf) < max_pos && (**buf) != '\n' && ( (**buf) == ',' || (**buf) == ' ' ) )
    (*buf)++;
}

#define read_field(field)\
  skip_delimiters(pos, max_pos);\
  get_next_el(pos, tmp, max_pos);\
  _tcscpy(field, tmp);

Abonent* parse_abonent(TCHAR** pos, TCHAR* max_pos)
{
  DWORD id;
  TCHAR phone_no[256];
  TCHAR family_name[256];
  TCHAR name[256];
  TCHAR middle_name[256];
  TCHAR street[256];
  TCHAR house[256];
  TCHAR building[256];
  TCHAR flat[256];
  TCHAR tmp[256];

  get_next_el(pos, tmp, max_pos);
  id = _ttoi(tmp);

  read_field(phone_no);
  read_field(family_name);
  read_field(name);
  read_field(middle_name);
  read_field(street);
  read_field(house);
  read_field(building);
  read_field(flat);

  skip_delimiters(pos, max_pos);
  (*pos)++;

  Abonent* abonent = create_abonent(id, phone_no, family_name, name,
                                    middle_name, street, house, building, flat);
  return abonent;
}

#undef read_field

#define copy_field(field)\
  if ( src->field != NULL )\
    _tcscpy(dest->field, src->field);

void copy_abonent(Abonent* dest, Abonent* src)
{
  dest->id = src->id;
  copy_field(phone_no);
  copy_field(family_name);
  copy_field(name);
  copy_field(middle_name);
  copy_field(street);
  copy_field(house);
  copy_field(building);
  copy_field(flat);
}

#undef copy_field

DWORD FUNC_DECLARE find_abonents(Abonent** abonents, DWORD max_num, Abonent* ab)
{
  DWORD count = -1;

  Thread_resources resources[MAX_THREADS];
  Thread_params params[MAX_THREADS];
  HANDLE handles[MAX_THREADS];
  DWORD thread_chunk_size = mapSize / MAX_THREADS;
  if ( thread_chunk_size % 2 == 1 )
    thread_chunk_size--;
  TCHAR* from = (TCHAR*) lpMapView;

  for ( int j = 0; j < MAX_THREADS; j++ )
  {
    resources[j].level_abonents = (Abonent***) malloc(sizeof(Abonent**) * 4);
    for ( int i = 0; i < 4; ++i )
      resources[j].level_abonents[i] = (Abonent**) malloc(sizeof(Abonent*) * max_num);
    resources[j].counts = (DWORD*) calloc(sizeof(DWORD), 4);
    while ( from >= lpMapView && (*from) != '\n' )
      --from;
    ++from;
    TCHAR* to = (TCHAR*) ((BYTE*) from + thread_chunk_size - 2);
    while ( to >= from && (*to) != '\n' )
      --to;
    ++to;
    if ( j == MAX_THREADS - 1 )
      to = (TCHAR*) ((BYTE*) lpMapView + mapSize - 2);
    if ( (*to) == '\n' )
      --to;

    params[j].from = from; params[j].to = to;
    params[j].cmp = &cmp; params[j].cmp_ab = ab;
    params[j].level_abonents = resources[j].level_abonents;
    params[j].counts = resources[j].counts;
    params[j].max_num = max_num;
    params[j].count = &count;
    params[j].res_ab = NULL;
    params[j].finish = NULL;

    DWORD thread_id;
    handles[j] = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size
        find_records,           // thread function name
        &params[j],             // argument to thread function
        0,                      // use default creation flags
        &thread_id);            // returns the thread identifier
    from = to;
  }

  DWORD fill = 0;
  DWORD dw_wait_result = WaitForMultipleObjects(MAX_THREADS, handles, TRUE, INFINITE);
  if ( dw_wait_result >= WAIT_OBJECT_0 &&
       dw_wait_result < WAIT_OBJECT_0 + MAX_THREADS ) {

    for ( int i = 3; i > -1; --i )
      for ( int j = 0; j < MAX_THREADS; ++j )
      {
        for ( int c = 0; c < resources[j].counts[i]; ++c )
        {
          if ( fill < max_num )
            copy_abonent(abonents[fill++], resources[j].level_abonents[i][c]);
          clear_abonent(resources[j].level_abonents[i][c]);
        }
      }

    for ( int j = 0; j < MAX_THREADS; ++j )
    {
      for ( int i = 0; i < 4; ++i )
        free(resources[j].level_abonents[i]);
      free(resources[j].level_abonents);
      free(resources[j].counts);
    }
  }

  return fill;
}

BOOL FUNC_DECLARE get_by_id(DWORD id, Abonent *abonent)
{
  Abonent* cmp_ab = create_abonent(id);

  Thread_params params[MAX_THREADS];
  HANDLE handles[MAX_THREADS];
  DWORD thread_chunk_size = mapSize / MAX_THREADS;
  if ( thread_chunk_size % 2 == 1 )
    thread_chunk_size--;
  Abonent* res_ab = NULL;
  BOOL finish = false;

  TCHAR* from = (TCHAR*) lpMapView;
  for ( int j = 0; j < MAX_THREADS; j++ )
  {
    while ( from >= lpMapView && (*from) != '\n' )
      from--;
    from++;
    TCHAR* to = (TCHAR*) ((BYTE*) from + thread_chunk_size - 2);
    while ( to >= from && (*to) != '\n' )
      to--;
    to++;
    if ( j == MAX_THREADS - 1 )
      to = (TCHAR*) ((BYTE*) lpMapView + mapSize - 2);
    if ( (*to) == '\n' )
      --to;

    params[j].from = from; params[j].to = to;
    params[j].cmp = &cmp_id; params[j].cmp_ab = cmp_ab;
    params[j].level_abonents = NULL; params[j].counts = NULL;
    params[j].max_num = 0; params[j].count = 0;
    params[j].res_ab = &res_ab;
    params[j].finish = &finish;

    DWORD thread_id;
    handles[j] = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size
        find_single_record,     // thread function name
        &params[j],             // argument to thread function
        0,                      // use default creation flags
        &thread_id);            // returns the thread identifier
    from = to;
  }

  DWORD dw_wait_result = WaitForMultipleObjects(MAX_THREADS, handles, TRUE, INFINITE);
  if ( dw_wait_result >= WAIT_OBJECT_0 &&
       dw_wait_result < WAIT_OBJECT_0 + MAX_THREADS ) {
    if (res_ab == NULL)
      return false;

    copy_abonent(abonent, res_ab);
    clear_abonent(res_ab);
    clear_abonent(cmp_ab);
    return true;
  }
  return false;
}

DWORD WINAPI find_records(void* data)
{
  Thread_params* tp = (Thread_params*) data;
  TCHAR* from = tp->from; TCHAR* to = tp->to;
  cmp_func cmp = tp->cmp; Abonent* cmp_ab = tp->cmp_ab;
  Abonent*** level_abonents = tp->level_abonents;
  DWORD* counts = tp->counts;
  DWORD max_num = tp->max_num;
  DWORD* count = tp->count;

  // _tprintf(TEXT("Thread started from=%c%c%c%c%c%c:%d to=%c%c%c%c%c%c:%d"), *from, *(from+1), *(from+2), *(from+3), *(from+4), *(from+5), from,
  //     *to, *(to+1), *(to+2), *(to+3), *(to+4), *(to+5), to);
  while ( from < to )
  {
    Abonent* abonent = parse_abonent(&from, to);
    int similar_level = cmp(abonent, cmp_ab);

    if ( similar_level > 0 )
    {
      (*count)++;
      if ( (*count) >= max_num )
      {
        clear_abonent(abonent);
        break;
      }
      level_abonents[similar_level - 1][ counts[similar_level - 1]++ ] = abonent;
    }
    else
      clear_abonent(abonent);
  }

  return 0;
}

DWORD WINAPI find_single_record(void* data)
{
  Thread_params* tp = (Thread_params*) data;
  TCHAR* from = tp->from; TCHAR* to = tp->to;
  cmp_func cmp = tp->cmp; Abonent* cmp_ab = tp->cmp_ab;
  Abonent** res_ab = tp->res_ab;
  BOOL* finish = tp->finish;

  TCHAR* back_from = from;
  while ( from < to )
  {
    if ( (*finish) )
      break;

    Abonent* abonent = parse_abonent(&from, to);
    int similar_level = cmp(abonent, cmp_ab);

    if ( similar_level == 4 )
    {
      (*finish) = true;
      (*res_ab) = abonent;
      break;
    }
    clear_abonent(abonent);
  }
  return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  switch (fdwReason)
  {
    case DLL_PROCESS_ATTACH:
      init();
      break;
    case DLL_PROCESS_DETACH:
      finalize();
      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
  }
  return TRUE;
}
