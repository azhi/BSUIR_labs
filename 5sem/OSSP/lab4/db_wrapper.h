#ifndef _DB_WRAPPER_H_
#define _DB_WRAPPER_H_

#include "db_funs.h"

class DB_Wrapper
{
public:
  DB_Wrapper(LPCTSTR path=TEXT("DB.dll"));
  virtual ~DB_Wrapper();

  find_abonents_func find_abonents;
  get_by_id_func get_by_id;
  update_abonent_func update_abonent;
  insert_abonent_func insert_abonent;
  remove_abonent_func remove_abonent;
  last_error_func last_error;

private:
  HINSTANCE dll_handle;
};

#endif /* _DB_WRAPPER_H_ */
