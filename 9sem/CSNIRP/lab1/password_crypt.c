#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "crypt.h"

int main(int argc, char *argv[])
{
  if (argc != 6) {
    fprintf(stderr, "Usage: %s --encode|--decode <alg_name> <password> <input file> <output file>", argv[0]);
    return -1;
  }

  enum Mode mode;
  if (strncmp(argv[1], "--encode", 9) == 0) {
    mode = ENCODE;
  } else if (strncmp(argv[1], "--decode", 9) == 0) {
    mode = DECODE;
  } else {
    fprintf(stderr, "Usage: %s --encode|--decode <alg_name> <password> <input file> <output file>", argv[0]);
    return -1;
  }

  struct AlgDescriptor alg_descriptor = find_alg_descriptor_by_name(argv[2]);

  HANDLE in_file = 0;
  HANDLE in_file_mapping = 0;
  PBYTE in_data = NULL;
  DWORD in_size = 0;
  map_file_into_memory(argv[4], GENERIC_READ, OPEN_EXISTING, PAGE_READONLY, FILE_MAP_READ, 0,
                       &in_file, &in_file_mapping, &in_data, &in_size);

  HANDLE out_file = 0;
  HANDLE out_file_mapping = 0;
  PBYTE out_data = NULL;
  DWORD out_size = in_size + alg_descriptor.block_size;
  map_file_into_memory(argv[5], GENERIC_READ | GENERIC_WRITE, CREATE_ALWAYS, PAGE_READWRITE, FILE_MAP_WRITE, out_size,
                       &out_file, &out_file_mapping, &out_data, NULL);

  HCRYPTPROV prov;
  HCRYPTKEY key;
  prepare_password_crypt(alg_descriptor, argv[3], &prov, &key);
  DWORD actual_out_size = do_crypt(alg_descriptor, key, mode, in_data, out_data, in_size, out_size);
  finalize_password_crypt(prov, key);

  strip_out_file(out_file, actual_out_size);

  UnmapViewOfFile(in_data);
  CloseHandle(in_file_mapping);
  CloseHandle(in_file);

  UnmapViewOfFile(out_data);
  CloseHandle(out_file_mapping);
  CloseHandle(out_file);

  return 0;
}
