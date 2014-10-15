#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "crypt.h"

enum HmacMode {
  GEN, CHECK
};

int main(int argc, char *argv[])
{
  if (argc != 5) {
    fprintf(stderr, "Usage: %s --generate|--check <password> <input file> <hmac file>", argv[0]);
    return -1;
  }

  enum Mode mode;
  if (strncmp(argv[1], "--generate", 11) == 0) {
    mode = GEN;
  } else if (strncmp(argv[1], "--check", 8) == 0) {
    mode = CHECK;
  } else {
    fprintf(stderr, "Usage: %s --generate|--check <password> <input file> <hmac file>", argv[0]);
    return -1;
  }

  struct AlgDescriptor alg_descriptor = find_alg_descriptor_by_name("RC4");

  HANDLE in_file = 0;
  HANDLE in_file_mapping = 0;
  PBYTE in_data = NULL;
  DWORD in_size = 0;
  map_file_into_memory(argv[3], GENERIC_READ, OPEN_EXISTING, PAGE_READONLY, FILE_MAP_READ, 0,
                       &in_file, &in_file_mapping, &in_data, &in_size);

  HCRYPTPROV prov;
  HCRYPTKEY key;
  prepare_password_crypt(alg_descriptor, argv[2], &prov, &key);
  DWORD res = 0;
  if (mode == GEN) {
    save_hmac(prov, key, argv[4], in_data, in_size);
  } else {
    BOOL ok = check_hmac(prov, key, argv[4], in_data, in_size);
    if (!ok)
      res = -1;
  }
  finalize_crypt(prov, key);

  UnmapViewOfFile(in_data);
  CloseHandle(in_file_mapping);
  CloseHandle(in_file);

  return res;
}
