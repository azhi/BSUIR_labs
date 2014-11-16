#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <windows.h>
#include <wincrypt.h>
#include <time.h>

enum Mode {
  ENCODE,
  DECODE
};

struct User {
  LPTSTR name;
  DWORD key_size;
  LPTSTR key;
};

struct Mandate {
  char *server_name;
  char *user_name;
  time_t expires;
  BYTE* session_key_blob;
  DWORD session_key_size;
};

void load_binary_key_from_file(const char* path, HCRYPTPROV prov, HCRYPTKEY* key);
void get_key_from_binary(BYTE* binary, DWORD binary_len, HCRYPTPROV prov, HCRYPTKEY* key);
void generate_session_key(HCRYPTPROV prov, HCRYPTKEY export_key, DWORD *key_size, BYTE *key_blob);

DWORD create_mandate(BYTE *res, struct Mandate mandate, HCRYPTKEY server_key);
struct Mandate parse_mandate(BYTE *src, DWORD src_len, HCRYPTKEY server_key);

void print_hex_binary(BYTE* binary, DWORD binary_size);

DWORD do_crypt(HCRYPTKEY key, enum Mode mode, PBYTE in_data, PBYTE out_data, DWORD in_size, DWORD out_size);

void create_prov(HCRYPTPROV* prov);
void finalize_prov(HCRYPTPROV prov);
