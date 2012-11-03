#include <stdio.h>
#include "DB.h"

BOOL FUNC_DECLARE init()
{
  printf("init\n");
  return 1;
}

DWORD FUNC_DECLARE find_by_name(DWORD *ids, DWORD max_num, LPCTSTR name)
{
  printf("fname\n");
  return 1;
}

DWORD FUNC_DECLARE find_by_phone_no(DWORD *ids, DWORD max_num, LPCTSTR phone_no)
{
  printf("fphone\n");
  return 1;
}

DWORD FUNC_DECLARE find_by_address(DWORD *ids, DWORD max_num, LPCTSTR adress)
{
  printf("faddr\n");
  return 1;
}

BOOL FUNC_DECLARE get_by_id(DWORD id, Abonent *abonent)
{
  printf("get_id\n");
  return 1;
}

BOOL FUNC_DECLARE update_abonent(Abonent *abonent)
{
  printf("update\n");
  return 1;
}

DWORD FUNC_DECLARE insert_abonent(Abonent *new_abonent)
{
  printf("insert\n");
  return 1;
}

BOOL FUNC_DECLARE remove_abonent(DWORD id)
{
  printf("remove\n");
  return 1;
}

