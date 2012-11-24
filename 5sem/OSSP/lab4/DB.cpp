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
    return 3;
  if ( abonent_some_equal(a1, a2) )
    return 2;
  if ( abonent_some_similar(a1, a2) )
    return 1;
  return 0;
}

#undef str_equal
#undef str_similar
#undef abonent_all_equal
#undef abonent_some_equal
#undef abonent_some_similar

void init()
{
  printf("initing\n");
  last_id = -1;
  mapAddingCapacity = ADDING_CAPACITY;

  error = false;
  HANDLE hFile=CreateFile(TEXT("phones.db"), GENERIC_READ | GENERIC_WRITE,
                          0, NULL, OPEN_ALWAYS, 
                          FILE_ATTRIBUTE_NORMAL, NULL);
  if ( hFile == INVALID_HANDLE_VALUE )
  {
    error = true;
    finalize();
    return;
  }

  mapSize = GetFileSize(hFile, 0);
  printf("Size: %d\n", mapSize);
  hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, mapSize + mapAddingCapacity, NULL);
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

  printf("getting last id\n");
  TCHAR* last_record = (TCHAR*) ((BYTE*) (lpMapView) + mapSize - 2);
  if ( (*last_record) == '\n' )
    --last_record;
  while ( last_record > lpMapView && (*last_record) != '\n' )
    --last_record;
  ++last_record;
  Abonent* last_abonent = parse_abonent(&last_record, (TCHAR*) ((BYTE*) lpMapView + mapSize - 2));
  last_id = last_abonent->id;
  clear_abonent(last_abonent);
  ++last_id;
  printf("last id=%d\n", last_id);
}

void finalize()
{
  printf("finalizing\n");
  if ( !error )
    FlushViewOfFile(lpMapView, mapSize);
  if ( hMap )
    CloseHandle(hMap);
  if ( lpMapView )
    UnmapViewOfFile(lpMapView);

  HANDLE hFile=CreateFile(TEXT("phones.db"), GENERIC_READ | GENERIC_WRITE,
                          0, NULL, OPEN_ALWAYS, 
                          FILE_ATTRIBUTE_NORMAL, NULL);
  if ( hFile == INVALID_HANDLE_VALUE )
    return;
  printf("truncating to %d size\n", mapSize);
  SetFilePointer(hFile, mapSize, 0, FILE_BEGIN);
  SetEndOfFile(hFile);
  CloseHandle(hFile);
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

#define copy_res_array(sub_ar_index)\
  for ( int j = 0; j < counts[sub_ar_index]; ++j )\
    ids[fill++] = level_ids[sub_ar_index][j];

DWORD FUNC_DECLARE find_abonents(DWORD *ids, DWORD max_num, Abonent* ab)
{
  DWORD count = -1;
  DWORD** level_ids = (DWORD**) malloc(sizeof(DWORD*) * 3);
  for ( int i = 0; i < 3; ++i )
    level_ids[i] = (DWORD*) malloc(sizeof(DWORD) * max_num);
  DWORD* counts = (DWORD*) calloc(sizeof(DWORD), 3);
  TCHAR* from = (TCHAR*) lpMapView;
  TCHAR* to = (TCHAR*) ((BYTE*) lpMapView + mapSize - 1);
  if ( (*to) == '\n' )
    --to;
  find_records(from, to, &cmp, ab, level_ids, counts, max_num, &count);

  DWORD fill = 0;
  copy_res_array(2);
  copy_res_array(1);
  copy_res_array(0);

  for ( int i = 0; i < 3; ++i )
    free(level_ids[i]);
  free(level_ids);
  free(counts);
  return count + 1;
}

#define copy_abonent_fields(ab1, ab2)\
  _tcscpy(ab1->phone_no, ab2->phone_no);\
  _tcscpy(ab1->family_name, ab2->family_name);\
  _tcscpy(ab1->name, ab2->name);\
  _tcscpy(ab1->middle_name, ab2->middle_name);\
  _tcscpy(ab1->street, ab2->street);\
  _tcscpy(ab1->house, ab2->house);\
  _tcscpy(ab1->building, ab2->building);\
  _tcscpy(ab1->flat, ab2->flat);


BOOL FUNC_DECLARE get_by_id(DWORD id, Abonent *abonent)
{
  Abonent* cmp_ab = create_abonent(id);
  TCHAR* from = (TCHAR*) lpMapView;
  TCHAR* to = (TCHAR*) ((BYTE*) lpMapView + mapSize - 2);
  if ( (*to) != '\n' )
    ++to;
  TCHAR* pos = find_single_record(from, to, &cmp_id, cmp_ab);
  if (pos == NULL)
    return false;
  Abonent* res_abonent = parse_abonent(&pos, to);
  abonent->id = res_abonent->id;
  copy_abonent_fields(abonent, res_abonent);
  clear_abonent(res_abonent);
  clear_abonent(cmp_ab);
  return true;
}

BOOL FUNC_DECLARE update_abonent(Abonent *abonent)
{
  // Abonent* cmp_ab = create_abonent(abonent->id);
  // List_element* res = find_element(head, &cmp_id, cmp_ab, NULL);
  // if (res == NULL)
  //   return false;
  // copy_abonent_fields(res->abonent, abonent);
  // clear_abonent(cmp_ab);
  return true;
}

#undef copy_abonent_fields

DWORD FUNC_DECLARE insert_abonent(Abonent *new_abonent)
{
  new_abonent->id = last_id++;
  TCHAR* end = (TCHAR*) ((BYTE*) lpMapView + mapSize - 2);
  if ( (*end) != '\n' )
  {
    end++;
    (*end) = '\n';
  }
  end++;

  TCHAR buf[2500];
  _stprintf(buf, TEXT("%d, %s, %s, %s, %s, %s, %s, %s, %s\n\0"), new_abonent->id, new_abonent->phone_no, new_abonent->family_name,
            new_abonent->name, new_abonent->middle_name, new_abonent->street,
            new_abonent->house, new_abonent->building, new_abonent->flat);

  DWORD dSize = _tcslen(buf) * 2;
  _tcscpy(end, buf);
  if ( dSize <= mapAddingCapacity )
  {
    mapSize += dSize;
    mapAddingCapacity -= dSize;
  }
  else
  {
    mapSize += dSize;
    FlushViewOfFile(lpMapView, mapSize);
    UnmapViewOfFile(lpMapView);
    CloseHandle(hMap);
    init();
  }

  return new_abonent->id;
}

BOOL FUNC_DECLARE remove_abonent(DWORD id)
{
  // Abonent* cmp_ab = create_abonent(id);
  // List_element* res = find_element(head, &cmp_id, cmp_ab, NULL);
  // if (res == NULL)
  //   return false;
  // remove_element(res);
  return true;
}

void find_records(TCHAR* from, TCHAR* to, cmp_func cmp, Abonent* cmp_ab, DWORD** level_ids, DWORD* counts, DWORD max_num, DWORD* count)
{
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
      level_ids[similar_level - 1][ counts[similar_level - 1]++ ] = abonent->id;
    }
    clear_abonent(abonent);
  }
}

TCHAR* find_single_record(TCHAR* from, TCHAR* to, cmp_func cmp, Abonent* cmp_ab)
{
  TCHAR* back_from = from;
  while ( from < to )
  {
    Abonent* abonent = parse_abonent(&from, to);
    int similar_level = cmp(abonent, cmp_ab);
    clear_abonent(abonent);

    if ( similar_level > 0 )
    {
      from -= 2;
      while ( from >= back_from && (*from) != '\n' )
        from--;
      from++;
      return from;
    }
  }
  return NULL;
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  switch (fdwReason)
  {
    case DLL_PROCESS_ATTACH:
      if ( reference_count == 0 )
        init();
      reference_count++;
      break;
    case DLL_PROCESS_DETACH:
      reference_count--;
      if ( reference_count == 0 )
        finalize();
      break;
    case DLL_THREAD_ATTACH:
      reference_count++;
      break;
    case DLL_THREAD_DETACH:
      reference_count--;
      break;
  }
  return TRUE;
}
