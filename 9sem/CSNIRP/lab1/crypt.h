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


void prepare_gen_key_crypt(struct AlgDescriptor alg_descriptor, enum Mode mode, const char* key_file,
                             HCRYPTPROV* prov, HCRYPTKEY* key);

void save_encrypted_session_key(const char* out_key_file_name, HCRYPTPROV prov, HCRYPTKEY key);
void prepare_password_crypt(struct AlgDescriptor alg_descriptor, char* password,
                             HCRYPTPROV* prov, HCRYPTKEY* key);
void finalize_crypt(HCRYPTPROV prov, HCRYPTKEY key);

void save_hmac(HCRYPTPROV prov, HCRYPTKEY key, const char* hmac_file_name, PBYTE in_data, DWORD in_size);
BOOL check_hmac(HCRYPTPROV prov, HCRYPTKEY key, const char* hmac_file_name, PBYTE in_data, DWORD in_size);

DWORD do_crypt(struct AlgDescriptor alg_descriptor, HCRYPTKEY key, enum Mode mode,
              PBYTE in_data, PBYTE out_data,
              DWORD in_size, DWORD out_size);

void strip_out_file(HANDLE out_file, DWORD actual_out_size);
