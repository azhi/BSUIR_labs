#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <windows.h>
#include <wincrypt.h>

struct User {
  LPTSTR name;
  DWORD n;
  LPTSTR key;
};

LPTSTR do_hash_round(HCRYPTPROV provider, LPTSTR data);
LPTSTR do_n_hash_round(HCRYPTPROV provider, LPTSTR data, DWORD n);
BOOL check_n_hash(HCRYPTPROV provider, LPTSTR data, LPTSTR template_data);

void create_prov(HCRYPTPROV* prov);
void finalize_prov(HCRYPTPROV prov);
