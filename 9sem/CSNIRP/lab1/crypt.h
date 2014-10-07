#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <windows.h>
#include <wincrypt.h>

enum Mode {
  ENCODE,
  DECODE
};

struct AlgDescriptor {
  const char* name;
  LPCTSTR provider_name;
  DWORD provider_type;
  ALG_ID algid;
  DWORD block_size;
};

struct AlgDescriptor find_alg_descriptor_by_name(const char* name);

void map_file_into_memory(char* path, DWORD file_access, DWORD file_create,
                          DWORD mapping_page_access, DWORD map_access, DWORD map_size,
                          HANDLE* file, HANDLE* file_mapping, PBYTE* data,
                          DWORD* size);

HCRYPTKEY generate_key(HCRYPTPROV provider, ALG_ID algid, LPTSTR password);

void prepare_password_crypt(struct AlgDescriptor alg_descriptor, char* password,
                             HCRYPTPROV* prov, HCRYPTKEY* key);
void finalize_password_crypt(HCRYPTPROV prov, HCRYPTKEY key);

DWORD do_crypt(struct AlgDescriptor alg_descriptor, HCRYPTKEY key, enum Mode mode,
              PBYTE in_data, PBYTE out_data,
              DWORD in_size, DWORD out_size);

void strip_out_file(HANDLE out_file, DWORD actual_out_size);
