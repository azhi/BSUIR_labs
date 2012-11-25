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
  last_error_func last_error;

private:
  HINSTANCE dll_handle;
};

#endif /* _DB_WRAPPER_H_ */
