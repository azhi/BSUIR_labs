#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <windows.h>
#include <wincrypt.h>

const size_t buf_size = 128;
const size_t block_size = 8; // 64-bit blocks

enum Mode {
  ENCODE,
  DECODE
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

HCRYPTKEY generate_key(HCRYPTPROV provider, ALG_ID algid, LPTSTR password) {
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

DWORD do_crypt(ALG_ID algid, char* password, enum Mode mode,
              PBYTE in_data, PBYTE out_data,
              DWORD in_size, DWORD out_size)
{
  BOOL res;
  HCRYPTPROV prov;
  DWORD actual_out_size;

  if (!CryptAcquireContext(&prov, 0, MS_ENHANCED_PROV, PROV_RSA_FULL,
                           CRYPT_VERIFYCONTEXT)) {
    fprintf(stderr, "Cannot acquire crypt context.");
    exit(-1);
  }

  HCRYPTKEY key = generate_key(prov, algid, password);
  if (key == 0) {
    fprintf(stderr, "Cannot generate key.");
    exit(-1);
  }

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

  CryptDestroyKey(key);
  CryptReleaseContext(prov,0);

  return actual_out_size;
}

void strip_out_file(HANDLE out_file, DWORD actual_out_size)
{
  if (out_file) {
    SetFilePointer(out_file, actual_out_size, 0, FILE_BEGIN);
    SetEndOfFile(out_file);
  }
}


int main (int argc, char *argv[])
{
  if (argc != 5) {
    fprintf(stderr, "Usage: %s --encode|--decode <password> <input file> <output file>", argv[0]);
    return -1;
  }

  enum Mode mode;
  if (strncmp(argv[1], "--encode", 9) == 0) {
    mode = ENCODE;
  } else if (strncmp(argv[1], "--decode", 9) == 0) {
    mode = DECODE;
  } else {
    fprintf(stderr, "Usage: %s --encode|--decode <password> <input file> <output file>", argv[0]);
    return -1;
  }

  HANDLE in_file = 0;
  HANDLE in_file_mapping = 0;
  PBYTE in_data = NULL;
  DWORD in_size = 0;
  map_file_into_memory(argv[3], GENERIC_READ, OPEN_EXISTING, PAGE_READONLY, FILE_MAP_READ, 0,
                       &in_file, &in_file_mapping, &in_data, &in_size);

  HANDLE out_file = 0;
  HANDLE out_file_mapping = 0;
  PBYTE out_data = NULL;
  DWORD out_size = (in_size / 8 + 2) * 8;
  map_file_into_memory(argv[4], GENERIC_READ | GENERIC_WRITE, CREATE_ALWAYS, PAGE_READWRITE, FILE_MAP_WRITE, out_size,
                       &out_file, &out_file_mapping, &out_data, NULL);

  DWORD actual_out_size = do_crypt(CALG_3DES, argv[2], mode, in_data, out_data, in_size, out_size);
  strip_out_file(out_file, actual_out_size);

  UnmapViewOfFile(in_data);
  CloseHandle(in_file_mapping);
  CloseHandle(in_file);

  UnmapViewOfFile(out_data);
  CloseHandle(out_file_mapping);
  CloseHandle(out_file);

  return 0;
}
