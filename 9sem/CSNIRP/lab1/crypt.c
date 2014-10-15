#include "crypt.h"

const short ALG_COUNT = 8;
const struct AlgDescriptor alg_descriptors[] = {
  {"RC2", MS_ENHANCED_PROV, PROV_RSA_FULL, CALG_RC2, 64 / 8},
  {"RC4", MS_ENHANCED_PROV, PROV_RSA_FULL, CALG_RC4, 0 / 8},
  {"DES", MS_ENHANCED_PROV, PROV_RSA_FULL, CALG_DES, 64 / 8},
  {"2k-3-DES", MS_ENHANCED_PROV, PROV_RSA_FULL, CALG_3DES_112, 64 / 8},
  {"3k-3-DES", MS_ENHANCED_PROV, PROV_RSA_FULL, CALG_3DES, 64 / 8},
  {"AES-128", MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CALG_AES_128, 128 / 8},
  {"AES-192", MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CALG_AES_192, 128 / 8},
  {"AES-256", MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CALG_AES_256, 128 / 8}
};

struct AlgDescriptor find_alg_descriptor_by_name(const char* name)
{
  struct AlgDescriptor res;
  for(int i = 0; i < ALG_COUNT; i++)
    if (strcmp(name, alg_descriptors[i].name) == 0) {
      res = alg_descriptors[i];
      break;
    }
  return res;
};

void map_file_into_memory(char* path, DWORD file_access, DWORD file_create,
                          DWORD mapping_page_access, DWORD map_access, DWORD map_size,
                          HANDLE* file, HANDLE* file_mapping, PBYTE* data,
                          DWORD* size)
{
  *file = CreateFileA(path, file_access, 0, NULL, file_create,
                    FILE_ATTRIBUTE_NORMAL, NULL);
  *file_mapping = CreateFileMappingA(*file, NULL, mapping_page_access, 0, map_size, NULL);
  if (*file_mapping == NULL) {
    fprintf(stderr, "Can't open memory mapped file. Error code: %lu\n",
            GetLastError());
    exit(-1);
  }
  *data = (PBYTE)MapViewOfFile(*file_mapping, map_access, 0, 0, 0);
  if (*data == NULL) {
    fprintf(stderr, "Can't map view of file. Error code: %lu\n", GetLastError());
    exit(-1);
  }

  if (size != NULL)
    *size = GetFileSize(*file, NULL);
}

HCRYPTKEY get_user_key(HCRYPTPROV prov, BOOL allow_create)
{
  HCRYPTKEY user_key;
  if (!CryptGetUserKey(prov, AT_KEYEXCHANGE, &user_key)) {
    DWORD last_error = GetLastError();
    if (last_error == NTE_NO_KEY && allow_create) {
      fprintf(stderr, "No user key found. Trying to build one...");
      if (!CryptGenKey(prov, AT_KEYEXCHANGE, 0, &user_key)) {
        fprintf(stderr, "Cannot generate user key. Error code: %lu\n",
                GetLastError());
        exit(-1);
      } else {
        if (!CryptGetUserKey(prov, AT_KEYEXCHANGE, &user_key)) {
          fprintf(stderr, "Cannot get user key. Error code: %lu\n",
                  GetLastError());
          exit(-1);
        }
      }
    } else {
      fprintf(stderr, "Cannot get user key. Error code: %lu\n",
              last_error);
      exit(-1);
    }
  }
  return user_key;
}


HCRYPTKEY load_encrypted_session_key(const char* key_file_name, HCRYPTPROV prov)
{
  HCRYPTKEY user_key = get_user_key(prov, FALSE);

  DWORD key_size = 260;
  BYTE key_blob[key_size];

  FILE* key_file = fopen(key_file_name, "rb");
  key_size = fread(key_blob, 1, key_size, key_file);
  fclose(key_file);

  HCRYPTKEY key;
  if (!CryptImportKey(prov, key_blob, key_size, user_key, 0, &key)) {
    fprintf(stderr, "Cannot import session key. Error code: %lu\n",
            GetLastError());
    exit(-1);
  }

  return key;
}

void save_encrypted_session_key(const char* key_file_name, HCRYPTPROV prov, HCRYPTKEY key)
{
  HCRYPTKEY user_key = get_user_key(prov, TRUE);

  DWORD key_size = 260;
  BYTE key_blob[key_size];
  if (!CryptExportKey(key, user_key, SIMPLEBLOB, 0, key_blob, &key_size)) {
    fprintf(stderr, "Cannot export session key. Error code: %lu\n",
            GetLastError());
    exit(-1);
  }

  FILE* key_file = fopen(key_file_name, "wb");
  fwrite(key_blob, 1, key_size, key_file);
  fclose(key_file);
}


void prepare_gen_key_crypt(struct AlgDescriptor alg_descriptor, enum Mode mode, const char* key_file,
                             HCRYPTPROV* prov, HCRYPTKEY* key)
{
  // try create new key storage
  if (!CryptAcquireContext(prov, 0, alg_descriptor.provider_name, alg_descriptor.provider_type,
                           CRYPT_NEWKEYSET)) {
    DWORD last_error = GetLastError();
    if (last_error == NTE_EXISTS) {
      // open existing key storage
      if (!CryptAcquireContext(prov, 0, alg_descriptor.provider_name, alg_descriptor.provider_type,
                                 0)) {
        last_error = GetLastError();
      } else {
        last_error = 0;
      }
    }
    if (last_error != 0) {
      fprintf(stderr, "Cannot acquire crypt context. Error code: %lu\n",
              last_error);
      exit(-1);
    }
  }

  if (mode == ENCODE) {
    if (!CryptGenKey(*prov, alg_descriptor.algid, CRYPT_EXPORTABLE, key)) {
      fprintf(stderr, "Cannot generate key. Error code: %lu\n",
              GetLastError());
      exit(-1);
    }
  } else {
    *key = load_encrypted_session_key(key_file, *prov);
  }
}

HCRYPTKEY generate_hash_key(HCRYPTPROV provider, ALG_ID algid, LPTSTR password)
{
  HCRYPTHASH hash;

  if (!CryptCreateHash(provider, CALG_SHA, 0, 0, &hash))
    return 0;
  if (!CryptHashData(hash, (BYTE *)password, strlen(password), 0)) {
    CryptDestroyHash(hash);
    return 0;
  }

  BOOL res;
  HCRYPTKEY key;
  res = CryptDeriveKey(provider, algid, hash, CRYPT_EXPORTABLE, &key);
  CryptDestroyHash(hash);
  return (res ? key : 0);
}

void prepare_password_crypt(struct AlgDescriptor alg_descriptor, char* password,
                             HCRYPTPROV* prov, HCRYPTKEY* key)
{
  if (!CryptAcquireContext(prov, 0, alg_descriptor.provider_name, alg_descriptor.provider_type,
                           CRYPT_VERIFYCONTEXT)) {
    fprintf(stderr, "Cannot acquire crypt context. Error code: %lu\n",
            GetLastError());
    exit(-1);
  }

  *key = generate_hash_key(*prov, alg_descriptor.algid, password);
  if (key == 0) {
    fprintf(stderr, "Cannot generate key. Error code: %lu\n",
            GetLastError());
    exit(-1);
  }
}

void finalize_crypt(HCRYPTPROV prov, HCRYPTKEY key)
{
  CryptDestroyKey(key);
  CryptReleaseContext(prov,0);
}

void calc_hmac(HCRYPTPROV prov, HCRYPTKEY key, PBYTE in_data, DWORD in_size, BYTE** res, DWORD* res_len)
{
  HCRYPTHASH hash;
  if (!CryptCreateHash(prov, CALG_HMAC, key, 0, &hash)) {
    fprintf(stderr, "Error in CryptCreateHash 0x%08x \n",
      GetLastError());
    exit(-1);
  }

  HMAC_INFO   HmacInfo;
  ZeroMemory(&HmacInfo, sizeof(HmacInfo));
  HmacInfo.HashAlgid = CALG_MD5;

  if (!CryptSetHashParam(hash, HP_HMAC_INFO, (BYTE*)&HmacInfo, 0)) {
    fprintf(stderr, "Error in CryptSetHashParam 0x%08x \n",
      GetLastError());
    exit(-1);
  }

  if (!CryptHashData(hash, in_data, in_size, 0))
  {
    printf("Error in CryptHashData 0x%08x \n",
      GetLastError());
    exit(-1);
  }

  if (!CryptGetHashParam(hash, HP_HASHVAL, NULL, res_len, 0))
  {
    fprintf(stderr, "Error in CryptGetHashParam 0x%08x \n",
      GetLastError());
    exit(-1);
  }
  *res = (BYTE*) malloc(*res_len);

  if (!CryptGetHashParam(hash, HP_HASHVAL, *res, res_len, 0))
  {
    printf("Error in CryptGetHashParam 0x%08x \n", GetLastError());
    exit(-1);
  }
}

void save_hmac(HCRYPTPROV prov, HCRYPTKEY key, const char* hmac_file_name, PBYTE in_data, DWORD in_size)
{
  BYTE *hmac = NULL;
  DWORD hmac_len;
  calc_hmac(prov, key, in_data, in_size, &hmac, &hmac_len);

  FILE* hmac_file = fopen(hmac_file_name, "wb");
  fwrite(hmac, 1, hmac_len, hmac_file);
  fclose(hmac_file);
  free(hmac);
}

BOOL check_hmac(HCRYPTPROV prov, HCRYPTKEY key, const char* hmac_file_name, PBYTE in_data, DWORD in_size)
{
  BYTE *s_hmac = (BYTE*) malloc(1024);

  FILE* hmac_file = fopen(hmac_file_name, "rb");
  DWORD s_hmac_len = fread(s_hmac, 1, 1024, hmac_file);
  fclose(hmac_file);

  BYTE *hmac = NULL;
  DWORD hmac_len;
  calc_hmac(prov, key, in_data, in_size, &hmac, &hmac_len);
  return (s_hmac_len == hmac_len && memcmp(s_hmac, hmac, hmac_len) == 0);
}

DWORD do_crypt(struct AlgDescriptor alg_descriptor, HCRYPTKEY key, enum Mode mode,
              PBYTE in_data, PBYTE out_data,
              DWORD in_size, DWORD out_size)
{
  size_t block_size = alg_descriptor.block_size;
  size_t buf_size = block_size != 0 ? block_size : 128;

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

void strip_out_file(HANDLE out_file, DWORD actual_out_size)
{
  if (out_file) {
    SetFilePointer(out_file, actual_out_size, 0, FILE_BEGIN);
    SetEndOfFile(out_file);
  }
}

