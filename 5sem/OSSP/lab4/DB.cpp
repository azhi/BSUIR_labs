#include "DB.h"

#include <stdio.h>

List_element *head, *tail;
int last_id;

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

void get_next_el(LPCTSTR buf, int* pi, LPTSTR res)
{
  int count = 0;
  while ((*pi) < _tcslen(buf) && buf[*pi] != ',' && buf[*pi] != '\n')
  {
    res[count++] = buf[*pi];
    (*pi)++;
  }
  res[count] = '\0';
}

void skip_delimiters(LPCTSTR buf, int* pi)
{
  while ( buf[*pi] == ',' || buf[*pi] == ' ' )
    (*pi)++;
}

#define read_field(field)\
  skip_delimiters(buf, &i);\
  get_next_el(buf, &i, tmp);\
  _tcscpy(field, tmp);

void init()
{
  head = NULL; tail = NULL;
  last_id = -1;
  FILE *db_file = _tfopen(TEXT("phones.db"), TEXT("rb"));
  if ( db_file == NULL )
    return;

  TCHAR buf[1024];
  while ( _fgetts(buf, 1024, db_file) != NULL)
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

    int i = 0, count = 0;
    get_next_el(buf, &i, tmp);
    id = _ttoi(tmp);

    read_field(phone_no);
    read_field(family_name);
    read_field(name);
    read_field(middle_name);
    read_field(street);
    read_field(house);
    read_field(building);
    read_field(flat);

    Abonent* new_abonent = create_abonent(id, phone_no, family_name, name,
                                          middle_name, street, house, building, flat);
    add_to_list(new_abonent);
    if ( id > last_id )
      last_id = id;
  }
  last_id++;
  fclose(db_file);
}

#undef read_field

void finalize()
{
  FILE *db_file = _tfopen(TEXT("phones.db"), TEXT("wb"));
  if ( db_file == NULL )
    return;
  List_element* iterator = head;
  while (iterator != NULL)
  {
    Abonent* abonent = iterator->abonent;
    _ftprintf(db_file, TEXT("%d, %s, %s, %s, %s, %s, %s, %s, %s\n"),
              abonent->id, abonent->phone_no, abonent->family_name,
              abonent->name, abonent->middle_name, abonent->street,
              abonent->house, abonent->building, abonent->flat);
    iterator = iterator->next;
  }
  fclose(db_file);
}

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
  List_element* begin = head;
  List_element* res = NULL;
  int similar_level = 0;
  while ( (res = find_element(begin, &cmp, ab, &similar_level)) != NULL )
  {
    count++;
    if ( count >= max_num )
      break;
    level_ids[similar_level - 1][ counts[similar_level - 1]++ ] = res->abonent->id;
    begin = res->next;
  }

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
  List_element* res = find_element(head, &cmp_id, cmp_ab, NULL);
  if (res == NULL)
    return false;
  abonent->id = res->abonent->id;
  copy_abonent_fields(abonent, res->abonent);
  clear_abonent(cmp_ab);
  return true;
}

BOOL FUNC_DECLARE update_abonent(Abonent *abonent)
{
  Abonent* cmp_ab = create_abonent(abonent->id);
  List_element* res = find_element(head, &cmp_id, cmp_ab, NULL);
  if (res == NULL)
    return false;
  copy_abonent_fields(res->abonent, abonent);
  clear_abonent(cmp_ab);
  return true;
}

#undef copy_abonent_fields

DWORD FUNC_DECLARE insert_abonent(Abonent *new_abonent)
{
  Abonent* abonent = create_abonent(last_id++, new_abonent->phone_no, new_abonent->family_name,
                                    new_abonent->name, new_abonent->middle_name, new_abonent->street,
                                    new_abonent->house, new_abonent->building, new_abonent->flat);
  add_to_list(abonent);
  return abonent->id;
}

BOOL FUNC_DECLARE remove_abonent(DWORD id)
{
  Abonent* cmp_ab = create_abonent(id);
  List_element* res = find_element(head, &cmp_id, cmp_ab, NULL);
  if (res == NULL)
    return false;
  remove_element(res);
  return true;
}

void add_to_list(Abonent* abonent)
{
  List_element *el = (List_element*) malloc(sizeof(List_element));
  el->abonent = abonent;
  el->next = NULL;

  if (tail == NULL)
  {
    el->prev = NULL;
    head = el;
    tail = el;
  }
  else
  {
    el->prev = tail;
    tail->next = el;
    tail = el;
  }
}

List_element* find_element(List_element* begin, cmp_func cmp, Abonent* cmp_ab, int* sim_level)
{
  List_element* iterator = begin;
  int similar_level = 0;
  while (iterator != NULL && ( similar_level = cmp(iterator->abonent, cmp_ab) ) == 0)
    iterator = iterator->next;
  if ( sim_level != NULL )
    *sim_level = similar_level;
  return iterator;
}

void remove_element(List_element* rem_el)
{
  List_element* prev_el = rem_el->prev;
  List_element* next_el = rem_el->next;
  if (prev_el == NULL)
    head = next_el;
  else
    prev_el->next = next_el;
  if (next_el == NULL)
    tail = prev_el;
  else
    next_el->prev = prev_el;
  clear_abonent(rem_el->abonent);
  free(rem_el);
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if ( reference_count == 0 )
    init();

  switch (fdwReason)
  {
    case DLL_PROCESS_ATTACH:
      reference_count++;
      break;
    case DLL_PROCESS_DETACH:
      reference_count--;
      break;
    case DLL_THREAD_ATTACH:
      reference_count++;
      break;
    case DLL_THREAD_DETACH:
      reference_count--;
      break;
  }

  if ( reference_count == 0 )
    finalize();

  return TRUE;
}
