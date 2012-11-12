#ifndef _DB_FUNS_H_
#define _DB_FUNS_H_

#include "DB.h"

typedef BOOL (*init_func)();
typedef void (*finalize_func)();

typedef DWORD (*find_by_name_func)(DWORD *ids, DWORD max_num, LPCTSTR name);
typedef DWORD (*find_by_phone_no_func)(DWORD *ids, DWORD max_num,
                                       LPCTSTR phone_no);
typedef DWORD (*find_by_address_func)(DWORD *ids, DWORD max_num,
                                      LPCTSTR address);

typedef BOOL (*get_by_id_func)(DWORD id, Abonent *abonent);

typedef BOOL (*update_abonent_func)(Abonent *abonent);

typedef DWORD (*insert_abonent_func)(Abonent *new_abonent);

typedef BOOL (*remove_abonent_func)(DWORD id);

#endif /* _DB_FUNS_H_ */
