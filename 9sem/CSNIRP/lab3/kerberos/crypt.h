#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <windows.h>
#include <wincrypt.h>
#include <time.h>

#define AS_PORT 5555
#define TGS_PORT 5556
#define R_PORT 5557

struct MKeyBlob {
  BLOBHEADER hdr;
  DWORD dwKeySize;
  BYTE rgbKeyData[];
};

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

struct Authenticator {
  char *user_name;
  time_t now;
};

void load_binary_key_from_file(const char* path, HCRYPTPROV prov, HCRYPTKEY* key);
void get_key_from_binary(BYTE* binary, DWORD binary_len, HCRYPTPROV prov, HCRYPTKEY* key);
void generate_session_key(HCRYPTPROV prov, DWORD *key_size, BYTE *key_blob);

DWORD create_mandate(BYTE *res, struct Mandate mandate, HCRYPTKEY server_key);
struct Mandate parse_mandate(BYTE *src, DWORD src_len, HCRYPTKEY server_key);

DWORD create_authenticator(BYTE *res, struct Authenticator auth, HCRYPTKEY session_key);
struct Authenticator parse_authenticator(BYTE *src, DWORD src_len, HCRYPTKEY session_key);

void print_hex_binary(BYTE* binary, DWORD binary_size);

DWORD do_crypt(HCRYPTKEY key, enum Mode mode, PBYTE in_data, PBYTE out_data, DWORD in_size, DWORD out_size);

void create_prov(HCRYPTPROV* prov);
void finalize_prov(HCRYPTPROV prov);
