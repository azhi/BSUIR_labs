#ifndef _DB_WRAPPER_H_
#define _DB_WRAPPER_H_

#include "db_funs.h"

class DB_Wrapper
{
public:
  DB_Wrapper(LPCTSTR path=TEXT("DB.dll"));
  virtual ~DB_Wrapper();

  find_by_name_func find_by_name;
  find_by_phone_no_func find_by_phone_no;
  find_by_address_func find_by_address;
  get_by_id_func get_by_id;
  update_abonent_func update_abonent;
  insert_abonent_func insert_abonent;
  remove_abonent_func remove_abonent;

private:
  init_func init;
  finalize_func finalize;

  HINSTANCE dll_handle;
};

#endif /* _DB_WRAPPER_H_ */
