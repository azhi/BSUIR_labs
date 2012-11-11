#include "DB.h"

#include <stdlib.h>

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

void FUNC_DECLARE init()
{
  head = NULL; tail = NULL;
  last_id = 0;
  FILE *db_file = _tfopen(TEXT("phones.db"), TEXT("rb"));
  if ( db_file == NULL )
    return;

  TCHAR buf[1024];
  while ( _fgetts(buf, 1024, db_file) != NULL)
  {
    DWORD id;
    TCHAR name[256];
    TCHAR phone_no[256];
    TCHAR address[256];
    TCHAR tmp[256];

    int i = 0, count = 0;
    get_next_el(buf, &i, tmp);
    id = _ttoi(tmp);
    
    skip_delimiters(buf, &i);
    get_next_el(buf, &i, tmp);
    _tcscpy(name, tmp);

    skip_delimiters(buf, &i);
    get_next_el(buf, &i, tmp);
    _tcscpy(phone_no, tmp);

    skip_delimiters(buf, &i);
    get_next_el(buf, &i, tmp);
    _tcscpy(address, tmp);

    Abonent* new_abonent = create_abonent(id, name, phone_no, address);
    add_to_list(new_abonent);
    if ( id > last_id )
      last_id = id;
  }
  last_id++;
  fclose(db_file);
}

void FUNC_DECLARE finalize()
{
  FILE *db_file = _tfopen(TEXT("phones.db"), TEXT("wb"));
  if ( db_file == NULL )
    return;
  List_element* iterator = head;
  while (iterator != NULL)
  {
    Abonent* abonent = iterator->abonent;
    _ftprintf(db_file, TEXT("%d, %s, %s, %s\n"), abonent->id, abonent->name, abonent->phone_no, abonent->address);
    iterator = iterator->next;
  }
  fclose(db_file);
}

DWORD FUNC_DECLARE find_by_name(DWORD *ids, DWORD max_num, LPCTSTR name)
{
  DWORD count = -1;
  Abonent* cmp_ab = create_abonent(0, name, NULL, NULL);
  List_element* begin = head;
  List_element* res = NULL;
  while ( (res = find_element(begin, &cmp_name, cmp_ab)) != NULL )
  {
    count++;
    if ( count < max_num )
      ids[count] = res->abonent->id;
    begin = res->next;
  }
  clear_abonent(cmp_ab);
  return count + 1;
}

DWORD FUNC_DECLARE find_by_phone_no(DWORD *ids, DWORD max_num, LPCTSTR phone_no)
{
  DWORD count = -1;
  Abonent* cmp_ab = create_abonent(0, NULL, phone_no, NULL);
  List_element* begin = head;
  List_element* res = NULL;
  while ( (res = find_element(begin, &cmp_phone, cmp_ab)) != NULL )
  {
    count++;
    if ( count < max_num )
      ids[count] = res->abonent->id;
    begin = res->next;
  }
  clear_abonent(cmp_ab);
  return count + 1;
}

DWORD FUNC_DECLARE find_by_address(DWORD *ids, DWORD max_num, LPCTSTR address)
{
  DWORD count = -1;
  Abonent* cmp_ab = create_abonent(0, NULL, NULL, address);
  List_element* begin = head;
  List_element* res = NULL;
  while ( (res = find_element(begin, &cmp_address, cmp_ab)) != NULL )
  {
    count++;
    if ( count < max_num )
      ids[count] = res->abonent->id;
    begin = res->next;
  }
  clear_abonent(cmp_ab);
  return count + 1;
}

BOOL FUNC_DECLARE get_by_id(DWORD id, Abonent *abonent)
{
  Abonent* cmp_ab = create_abonent(id, NULL, NULL, NULL);
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
  Abonent* cmp_ab = create_abonent(abonent->id, NULL, NULL, NULL);
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
  Abonent* abonent = create_abonent(last_id++, new_abonent->name, new_abonent->phone_no, new_abonent->address);
  add_to_list(abonent);
  return abonent->id;
}

BOOL FUNC_DECLARE remove_abonent(DWORD id)
{
  Abonent* cmp_ab = create_abonent(id, NULL, NULL, NULL);
  List_element* res = find_element(head, &cmp_id, cmp_ab);
  if (res == NULL)
    return false;
  remove_element(res);
  return true;
}

Abonent* create_abonent(DWORD id, LPCTSTR name, LPCTSTR phone, LPCTSTR address)
{
  Abonent *abonent = (Abonent*) malloc(sizeof(Abonent));
  abonent->id = id;
  abonent->name = (LPTSTR) calloc(sizeof(TCHAR), 256);
  if (name != NULL)
    _tcscpy(abonent->name, name);
  abonent->phone_no = (LPTSTR) calloc(sizeof(TCHAR), 256);
  if (phone != NULL)
    _tcscpy(abonent->phone_no, phone);
  abonent->address = (LPTSTR) calloc(sizeof(TCHAR), 256);
  if (address != NULL)
    _tcscpy(abonent->address, address);
  return abonent;
}

void clear_abonent(Abonent* abonent)
{
  if (abonent != NULL)
  {
    free(abonent->name);
    free(abonent->phone_no);
    free(abonent->address);
    free(abonent);
  }
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

