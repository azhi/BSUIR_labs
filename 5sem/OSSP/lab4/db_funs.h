#ifndef _DB_FUNS_H_
#define _DB_FUNS_H_

#include "abonent.h"

typedef DWORD (*find_abonents_func)(Abonent** abonents, DWORD max_num,
                                       Abonent* ab);
typedef BOOL (*get_by_id_func)(DWORD id, Abonent *abonent);

typedef BOOL (*last_error_func)();

#endif /* _DB_FUNS_H_ */
