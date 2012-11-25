#ifndef _COMMON_H_
#define _COMMON_H_

typedef struct init_info_t {
  DWORD thread_id;
  CHAR pipe_name[22];
} init_info_t;

const size_t init_info_size = sizeof(init_info_t);

#endif /* _COMMON_H_ */
