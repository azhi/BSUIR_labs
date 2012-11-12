#ifndef _DB_FUNS_H_
#define _DB_FUNS_H_

#include "abonent.h"

typedef DWORD (*find_abonents_func)(DWORD *ids, DWORD max_num,
                                       Abonent* ab);
typedef BOOL (*get_by_id_func)(DWORD id, Abonent *abonent);

typedef BOOL (*update_abonent_func)(Abonent *abonent);

typedef DWORD (*insert_abonent_func)(Abonent *new_abonent);

typedef BOOL (*remove_abonent_func)(DWORD id);

#endif /* _DB_FUNS_H_ */
