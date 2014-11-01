#include <winsock2.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "crypt.h"

const int PASSWORDS_COUNT = 255;

void append_self_to_users_file(const char *path, struct User user)
{
  FILE* users_file = fopen(path, "ab");

  fwrite(&user.n, 2, 1, users_file);

  DWORD name_length = strlen(user.name);
  fwrite(&name_length, 2, 1, users_file);
  fwrite(user.name, 1, name_length, users_file);

  DWORD key_length = strlen(user.key);
  fwrite(&key_length, 2, 1, users_file);
  fwrite(user.key, 1, key_length, users_file);

  fclose(users_file);
}

LPTSTR get_key_blob(HCRYPTPROV prov, char* password)
{
  HCRYPTHASH hash;

  if (!CryptCreateHash(prov, CALG_SHA, 0, 0, &hash)) {
    fprintf(stderr, "Error in CryptCreateHash 0x%08x \n",
      GetLastError());
    exit(-1);
  }
  if (!CryptHashData(hash, (BYTE *)password, strlen(password), 0)) {
    printf("Error in CryptHashData 0x%08x \n",
      GetLastError());
    exit(-1);
  }

  DWORD res_len = 0;
  if (!CryptGetHashParam(hash, HP_HASHVAL, NULL, &res_len, 0))
  {
    fprintf(stderr, "Error in CryptGetHashParam 0x%08x \n",
      GetLastError());
    exit(-1);
  }
  LPTSTR res = (LPTSTR) malloc(res_len + 1);

  if (!CryptGetHashParam(hash, HP_HASHVAL, res, &res_len, 0))
  {
    fprintf(stderr, "Error in CryptGetHashParam 0x%08x \n",
      GetLastError());
    exit(-1);
  }
  res[res_len] = '\0';

  return res;
}

SOCKET connect_to_server(const char *server_host, const char *server_port)
{
  WORD ver = MAKEWORD(2,2);
  WSADATA wsaData;
  int retVal = 0;

  WSAStartup(ver,(LPWSADATA)&wsaData);

  LPHOSTENT hostEnt;

  hostEnt = gethostbyname(server_host);

  if (!hostEnt) {
    fprintf(stderr, "Unable to collect gethostbyname\n");
    WSACleanup();
    exit(-1);
  }

  SOCKET clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (clientSock == SOCKET_ERROR) {
    fprintf(stderr, "Unable to create socket\n");
    WSACleanup();
    exit(-1);
  }

  SOCKADDR_IN serverInfo;

  serverInfo.sin_family = PF_INET;
  serverInfo.sin_addr = *((LPIN_ADDR)*hostEnt->h_addr_list);
  serverInfo.sin_port = htons(atoi(server_port));

  retVal=connect(clientSock,(LPSOCKADDR)&serverInfo, sizeof(serverInfo));
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to connect\n");
    WSACleanup();
    exit(-1);
  }

  return clientSock;
}

DWORD request_n(SOCKET sock, const char *name)
{
  int retVal = 0;

  retVal = send(sock, name, strlen(name), 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to send\n");
    WSACleanup();
    exit(-1);
  }

  DWORD nResponse = 0;
  retVal = recv(sock, (char*) &nResponse, 2, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to recv\n");
    WSACleanup();
    exit(-1);
  }

  return nResponse;
}

BOOL do_register(SOCKET sock, const char *n_key)
{
  int retVal = 0;

  retVal = send(sock, n_key, strlen(n_key), 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to send\n");
    WSACleanup();
    exit(-1);
  }

  DWORD okResponse = 0;
  retVal = recv(sock, (char*) &okResponse, 1, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to recv\n");
    WSACleanup();
    exit(-1);
  }

  return okResponse == 1;
}

void close_socket(SOCKET sock)
{
  closesocket(sock);
  WSACleanup();
}

int main(int argc, char *argv[])
{
  if (argc != 5 && argc != 6) {
    fprintf(stderr, "Usage: %s [--append-self <users_file> | --authenticate <server_host> <server_port>] <name> <password>\n", argv[0]);
    return -1;
  }

  HCRYPTPROV prov;
  create_prov(&prov);

  if (strcmp(argv[1], "--append-self") == 0) {
    struct User user;
    user.n = PASSWORDS_COUNT;
    user.name = argv[3];
    LPTSTR key = get_key_blob(prov, argv[4]);
    LPTSTR n_key = do_n_hash_round(prov, key, PASSWORDS_COUNT);
    user.key = n_key;

    append_self_to_users_file(argv[2], user);
  } else if (strcmp(argv[1], "--authenticate") == 0) {
    SOCKET sock = connect_to_server(argv[2], argv[3]);
    printf("Connected to server, requesting N-1\n");
    DWORD n = request_n(sock, argv[4]);
    printf("Got N-1: %d. Doing N-1 hash rounds\n", n);
    LPTSTR key = get_key_blob(prov, argv[5]);
    LPTSTR n_key = do_n_hash_round(prov, key, n);
    printf("Sending N-1 hashed key to server\n");
    BOOL res = do_register(sock, n_key);
    close_socket(sock);
    if (res) {
      printf("Auth OK\n");
      return 1;
    } else {
      printf("Auth FAIL\n");
      return 0;
    }
  } else {
    fprintf(stderr, "Usage: %s [--append-self <users_file> | --authenticate <server_host> <server_port>] <name> <password>\n", argv[0]);
    return -1;
  }

  return 0;
}
