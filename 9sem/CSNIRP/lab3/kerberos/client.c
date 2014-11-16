#include <winsock2.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "crypt.h"

const int AS_PORT = 5555;
const int TGS_PORT = 5556;
const int R_PORT = 5557;

void append_self_to_users_file(const char *path, struct User user)
{
  FILE* users_file = fopen(path, "ab");

  DWORD name_length = strlen(user.name);
  fwrite(&name_length, 2, 1, users_file);
  fwrite(user.name, 1, name_length, users_file);

  fwrite(&user.key_size, 2, 1, users_file);
  fwrite(user.key, 1, user.key_size, users_file);

  fclose(users_file);
}

void get_key_blob(HCRYPTPROV prov, char* password, LPTSTR *res, DWORD *res_len)
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

  if (!CryptGetHashParam(hash, HP_HASHVAL, NULL, res_len, 0))
  {
    fprintf(stderr, "Error in CryptGetHashParam 0x%08x \n",
      GetLastError());
    exit(-1);
  }
  *res = (LPTSTR) malloc(*res_len + 1);

  if (!CryptGetHashParam(hash, HP_HASHVAL, *res, res_len, 0))
  {
    fprintf(stderr, "Error in CryptGetHashParam 0x%08x \n",
      GetLastError());
    exit(-1);
  }
}

SOCKET connect_to_server(const char *server_host, int server_port)
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
  serverInfo.sin_port = htons(server_port);

  retVal=connect(clientSock,(LPSOCKADDR)&serverInfo, sizeof(serverInfo));
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to connect\n");
    WSACleanup();
    exit(-1);
  }

  return clientSock;
}

void close_socket(SOCKET sock)
{
  closesocket(sock);
  WSACleanup();
}

void communicate_with_as(const char *host, const char *name, HCRYPTPROV prov, HCRYPTKEY user_key,
                         HCRYPTKEY* session_key, LPTSTR *encrypted_mandate, DWORD *encrypted_mandate_size)
{
  SOCKET s = connect_to_server(host, AS_PORT);

  int retVal = 0;

  retVal = send(s, name, strlen(name), 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to send\n");
    WSACleanup();
    exit(-1);
  }

  const int buf_len = 10240;
  char buf[10240];
  size_t msg_len = 0;
  msg_len = retVal = recv(s, buf, buf_len, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to recv\n");
    WSACleanup();
    exit(-1);
  }

  printf("Got 1st part from AS (session key). Encrypted: ");
  print_hex_binary(buf, msg_len);
  do_crypt(user_key, DECODE, buf, buf, msg_len, msg_len);
  printf("Decrypted: ");
  print_hex_binary(buf, msg_len);
  get_key_from_binary(buf, msg_len, prov, session_key);

  msg_len = retVal = recv(s, buf, buf_len, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to recv\n");
    WSACleanup();
    exit(-1);
  }

  printf("Got 1st part from AS (encrypted mandate): ");
  print_hex_binary(buf, msg_len);
  *encrypted_mandate_size = msg_len;
  *encrypted_mandate = (LPTSTR) malloc(msg_len);
  memcmp(*encrypted_mandate, buf, msg_len);

  close_socket(s);
}

int main(int argc, char *argv[])
{
  if (argc != 5 && argc != 6) {
    fprintf(stderr, "Usage: %s [--append-self <users_file> | --authenticate <server_host>] <name> <password>\n", argv[0]);
    return -1;
  }

  HCRYPTPROV prov;
  create_prov(&prov);

  if (strcmp(argv[1], "--append-self") == 0) {
    struct User user;
    user.name = argv[3];
    LPTSTR key = NULL;
    DWORD key_size = 0;
    get_key_blob(prov, argv[4], &key, &key_size);
    user.key = key;
    user.key_size = key_size;

    append_self_to_users_file(argv[2], user);
  } else if (strcmp(argv[1], "--authenticate") == 0) {
    LPTSTR key_blob = NULL;
    DWORD key_size = 0;
    get_key_blob(prov, argv[4], &key_blob, &key_size);

    HCRYPTKEY user_key;
    get_key_from_binary(key_blob, key_size, prov, &user_key);

    HCRYPTKEY session_key;
    LPTSTR encrypted_mandate = NULL;
    DWORD encrypted_mandate_size = 0;
    communicate_with_as(argv[2], argv[3], prov, user_key, &session_key, &encrypted_mandate, &encrypted_mandate_size);

  } else {
    fprintf(stderr, "Usage: %s [--append-self <users_file> | --authenticate <server_host> <server_port>] <name> <password>\n", argv[0]);
    return -1;
  }

  return 0;
}
