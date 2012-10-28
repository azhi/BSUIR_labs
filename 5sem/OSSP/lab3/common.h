#ifndef _COMMON_H_
#define _COMMON_H_

#include <windows.h>

#define asrt(err, msg)			      \
  if (err) {				      \
    MessageBox(NULL, TEXT("Error!"), msg, 0); \
    throw;				      \
  }


#endif /* _COMMON_H_ */
