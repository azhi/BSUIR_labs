#include "crypt.h"

void load_binary_key_from_file(const char* path, HCRYPTPROV prov, HCRYPTKEY* key)
{
  DWORD key_size = 10240;
  BYTE key_blob[key_size];

  FILE* key_file = fopen(path, "rb");
  key_size = fread(key_blob, 1, key_size, key_file);
  fclose(key_file);

  get_key_from_binary(key_blob, key_size, prov, key);
}

void get_key_from_binary(BYTE* binary, DWORD binary_len, HCRYPTPROV prov, HCRYPTKEY* key)
{
  HCRYPTHASH hash;
  if (!CryptCreateHash(prov, CALG_SHA, 0, 0, &hash)) {
    fprintf(stderr, "Cannot create hash. Error code: %lu\n",
            GetLastError());
    exit(-1);
  }
  if (!CryptHashData(hash, (BYTE *)binary, binary_len, 0)) {
    fprintf(stderr, "Cannot hash data. Error code: %lu\n",
            GetLastError());
    exit(-1);
  }
  if (!CryptDeriveKey(prov, CALG_AES_256, hash, CRYPT_EXPORTABLE, key)) {
    fprintf(stderr, "Cannot derive key. Error code: %lu\n",
            GetLastError());
    exit(-1);
  }
  CryptDestroyHash(hash);
}

void generate_session_key(HCRYPTPROV prov, HCRYPTKEY export_key, DWORD *key_size, BYTE *key_blob)
{
  HCRYPTKEY key;

  if (!CryptGenKey(prov, CALG_AES_256, CRYPT_EXPORTABLE, &key)) {
    fprintf(stderr, "Cannot generate key. Error code: %lu\n",
            GetLastError());
    exit(-1);
  }

  if (!CryptExportKey(key, export_key, SIMPLEBLOB, 0, key_blob, key_size)) {
    fprintf(stderr, "Cannot export session key. Error code: %lu\n",
            GetLastError());
    exit(-1);
  }
}

DWORD create_mandate(BYTE *res, struct Mandate mandate, HCRYPTKEY server_key)
{
  DWORD res_len = 0;

  res[res_len++] = (BYTE) strlen(mandate.server_name);
  memcpy(res + res_len, mandate.server_name, strlen(mandate.server_name));
  res_len += strlen(mandate.server_name);

  res[res_len++] = (BYTE) strlen(mandate.user_name);
  memcpy(res + res_len, mandate.user_name, strlen(mandate.user_name));
  res_len += strlen(mandate.user_name);

  memcpy(res + res_len, &mandate.expires, sizeof(mandate.expires));
  res_len += sizeof(mandate.expires);

  memcpy(res + res_len, mandate.session_key_size, sizeof(mandate.session_key_size));
  res_len += sizeof(mandate.session_key_size);

  memcpy(res + res_len, mandate.session_key_blob, mandate.session_key_size);
  res_len += mandate.session_key_size;

  res_len = do_crypt(server_key, ENCODE, res, res, res_len, res_len);
  return res_len;
}

struct Mandate parse_mandate(BYTE *src, DWORD src_len, HCRYPTKEY server_key)
{
  struct Mandate res;
  src_len = do_crypt(server_key, DECODE, src, src, src_len, src_len);

  DWORD i = 0;

  BYTE server_name_length = src[i];
  i += 1;
  res.server_name = malloc(server_name_length + 1);
  memcpy(res.server_name, src + i, server_name_length);
  res.server_name[server_name_length] = '\0';
  i += server_name_length;

  BYTE user_name_length = src[i];
  i += 1;
  res.user_name = malloc(user_name_length + 1);
  memcpy(res.user_name, src + i, user_name_length);
  res.user_name[user_name_length] = '\0';
  i += user_name_length;

  memcpy((BYTE*) res.expires, src + i, sizeof(time_t));
  i += sizeof(time_t);

  memcpy((BYTE*) res.session_key_size, src + i, sizeof(DWORD));
  i += sizeof(DWORD);

  memcpy(res.session_key_blob, src + i, res.session_key_size);
  i += res.session_key_size;

  return res;
}

void print_hex_binary(BYTE* binary, DWORD binary_size)
{
  for (int i = 0; i < binary_size; i++)
    printf("%02x", (unsigned) binary[i]);
  putchar('\n');
}

DWORD do_crypt(HCRYPTKEY key, enum Mode mode,
              PBYTE in_data, PBYTE out_data,
              DWORD in_size, DWORD out_size)
{
  size_t block_size = 16;
  size_t buf_size = 16;

  BOOL res;
  DWORD actual_out_size;

  for (DWORD i = 0; i < in_size; i += buf_size) {

    unsigned char buf[buf_size + block_size];
    DWORD len = buf_size;
    void *in_chunk = in_data + i,
         *out_chunk = out_data + i;
    BOOL final = in_size - i <= buf_size;

    if (final) {
      len = in_size - i;
    }

    memcpy(buf, in_chunk, len);
    if (mode == ENCODE) {
      res = CryptEncrypt(key, 0, final, 0, buf, &len, buf_size + block_size);
    } else {
      res = CryptDecrypt(key, 0, final, 0, buf, &len);
    }

    if (res) {
      memcpy(out_chunk, buf, len);
      if (final) {
        actual_out_size = i + len;
      }
    } else {
      fprintf(stderr, "Cant crypt chunk %d. Error code: %lu\n",
              i, GetLastError());
      exit(-1);
    }
  }

  return actual_out_size;
}

void create_prov(HCRYPTPROV* prov)
{
  if (!CryptAcquireContext(prov, 0, MS_ENHANCED_PROV, PROV_RSA_AES,
                           CRYPT_VERIFYCONTEXT)) {
    fprintf(stderr, "Cannot acquire crypt context. Error code: %lu\n",
            GetLastError());
    exit(-1);
  }
}

void finalize_prov(HCRYPTPROV prov)
{
  CryptReleaseContext(prov,0);
}
