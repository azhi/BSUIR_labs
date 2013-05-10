#ifndef _COMMON_H_
#define _COMMON_H_

#define die(...)                \
{                               \
  fprintf(stderr, __VA_ARGS__); \
  exit(-1);                     \
}

#endif /* _COMMON_H_ */
