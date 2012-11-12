#include "DB.h"

#include <stdio.h>

List_element *head, *tail;
int last_id;

BOOL cmp_id(const Abonent* a1, const Abonent* a2)
{
  return a1->id == a2->id;
}

BOOL cmp(const Abonent* a1, const Abonent* a2)
{
  return a1->id == a2->id;
}

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

#define read_field(LPTSTR field)
  skip_delimiters(buf, &i);\
  get_next_el(buf, &i, tmp);\
  _tcscpy(field, tmp);

void FUNC_DECLARE init()
{
  _tprintf(TEXT("initing\n"));
  head = NULL; tail = NULL;
  last_id = 0;
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

void FUNC_DECLARE finalize()
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

DWORD FUNC_DECLARE find_abonents(DWORD *ids, DWORD max_num, Abonent* ab)
{
  DWORD count = -1;
  List_element* begin = head;
  List_element* res = NULL;
  while ( (res = find_element(begin, &cmp, ab)) != NULL )
  {
    count++;
    if ( count < max_num )
      ids[count] = res->abonent->id;
    begin = res->next;
  }
  return count + 1;
}

BOOL FUNC_DECLARE get_by_id(DWORD id, Abonent *abonent)
{
  Abonent* cmp_ab = create_abonent(id);
  List_element* res = find_element(head, &cmp_id, cmp_ab);
  if (res == NULL)
    return false;
  abonent->id = res->abonent->id;
  _tcscpy(abonent->name, res->abonent->name);
  _tcscpy(abonent->phone_no, res->abonent->phone_no);
  _tcscpy(abonent->address, res->abonent->address);
  clear_abonent(cmp_ab);
  return true;
}

BOOL FUNC_DECLARE update_abonent(Abonent *abonent)
{
  Abonent* cmp_ab = create_abonent(abonent->id);
  List_element* res = find_element(head, &cmp_id, cmp_ab);
  if (res == NULL)
    return false;
  _tcscpy(res->abonent->name, abonent->name);
  _tcscpy(res->abonent->phone_no, abonent->phone_no);
  _tcscpy(res->abonent->address, abonent->address);
  clear_abonent(cmp_ab);
  return true;
}

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
  List_element* res = find_element(head, &cmp_id, cmp_ab);
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

List_element* find_element(List_element* begin, cmp_func cmp, Abonent* cmp_ab)
{
  List_element* iterator = begin;
  while (iterator != NULL && !cmp(iterator->abonent, cmp_ab))
  {
    iterator = iterator->next;
  }
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
