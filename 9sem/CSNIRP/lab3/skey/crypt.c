#include "crypt.h"

LPTSTR do_hash_round(HCRYPTPROV provider, LPTSTR data) {
  HCRYPTHASH hash;

  if (!CryptCreateHash(provider, CALG_SHA, 0, 0, &hash)) {
    fprintf(stderr, "Error in CryptCreateHash 0x%08x \n",
      GetLastError());
    exit(-1);
  }
  if (!CryptHashData(hash, (BYTE *)data, strlen(data), 0)) {
    fprintf(stderr, "Error in CryptHashData 0x%08x \n",
      GetLastError());
    exit(-1);
  }

  DWORD res_len;
  if (!CryptGetHashParam(hash, HP_HASHVAL, NULL, &res_len, 0)) {
    fprintf(stderr, "Error in CryptGetHashParam 0x%08x \n",
      GetLastError());
    exit(-1);
  }
  LPTSTR res = (LPTSTR) malloc(res_len + 1);

  if (!CryptGetHashParam(hash, HP_HASHVAL, res, &res_len, 0)) {
    fprintf(stderr, "Error in CryptGetHashParam 0x%08x \n",
      GetLastError());
    exit(-1);
  }
  res[res_len] = '\0';

  return res;
}

LPTSTR do_n_hash_round(HCRYPTPROV provider, LPTSTR data, DWORD n)
{
  LPTSTR current = (LPTSTR) malloc(strlen(data) + 1);
  strcpy(current, data);
  for(DWORD i = 0; i < n; i++) {
    LPTSTR step = do_hash_round(provider, current);
    free(current);
    current = step;
  }
  return current;
}

BOOL check_n_hash(HCRYPTPROV provider, LPTSTR data, LPTSTR template_data)
{
  LPTSTR n_hash = do_hash_round(provider, data);
  return (strcmp(n_hash, template_data) == 0);
}

void create_prov(HCRYPTPROV* prov)
{
  if (!CryptAcquireContext(prov, 0, 0, PROV_RSA_FULL, 0)) {
    fprintf(stderr, "Cannot acquire crypt context. Error code: %lu\n",
            GetLastError());
    exit(-1);
  }
}

void finalize_prov(HCRYPTPROV prov)
{
  CryptReleaseContext(prov,0);
}
