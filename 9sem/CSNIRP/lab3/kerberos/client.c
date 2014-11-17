#include <winsock2.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "crypt.h"

#define FAIL_IF_RESP_EMPTY(resp_len) if(resp_len == 0){printf("Got empty response from server, authentication failed.\n"); exit(-1);}

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

void get_key_blob(HCRYPTPROV prov, char* password, BYTE **res, DWORD *res_len)
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
  *res = (BYTE*) malloc(*res_len + 1);

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
                         HCRYPTKEY* session_key, BYTE **encrypted_tgs_mandate, DWORD *encrypted_tgs_mandate_size)
{
  printf("Initializing connection to AS\n");
  SOCKET s = connect_to_server(host, AS_PORT);

  int retVal = 0;

  printf("Sending name: %s\n", name);
  retVal = send(s, name, strlen(name), 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to send\n");
    WSACleanup();
    exit(-1);
  }

  const int buf_len = 10240;
  char buf[buf_len];
  size_t msg_len = 0;
  msg_len = retVal = recv(s, buf, buf_len, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to recv\n");
    WSACleanup();
    exit(-1);
  }
  FAIL_IF_RESP_EMPTY(retVal);

  printf("Got 1st part from AS (session key). Encrypted: ");
  print_hex_binary(buf, msg_len);
  DWORD actual_length = do_crypt(user_key, DECODE, buf, buf, msg_len, msg_len);
  printf("Decrypted: ");
  print_hex_binary(buf, actual_length);
  get_key_from_binary(buf, actual_length, prov, session_key);

  msg_len = retVal = recv(s, buf, buf_len, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to recv\n");
    WSACleanup();
    exit(-1);
  }
  FAIL_IF_RESP_EMPTY(retVal);

  printf("Got 2nd part from AS (encrypted TGS mandate): ");
  print_hex_binary(buf, msg_len);
  *encrypted_tgs_mandate_size = msg_len;
  *encrypted_tgs_mandate = (BYTE*) malloc(msg_len);
  memcpy(*encrypted_tgs_mandate, buf, msg_len);

  close_socket(s);
}

void communicate_with_tgs(const char *host, char *name, HCRYPTPROV prov, HCRYPTKEY session_key,
                          BYTE *encrypted_tgs_mandate, DWORD encrypted_tgs_mandate_size, HCRYPTKEY *r_key,
                          BYTE **encrypted_r_mandate, DWORD *encrypted_r_mandate_size)
{
  printf("Initializing connection to TGS\n");
  SOCKET s = connect_to_server(host, TGS_PORT);

  int retVal = 0;
  const int buf_len = 10240;
  char buf[buf_len];

  struct Authenticator auth = {name, time(NULL)};
  DWORD msg_len = create_authenticator(buf, auth, session_key);

  printf("Sending authenticator: ");
  print_hex_binary(buf, msg_len);
  retVal = send(s, buf, msg_len, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to send\n");
    WSACleanup();
    exit(-1);
  }

  printf("Sending TGS mandate: ");
  print_hex_binary(encrypted_tgs_mandate, encrypted_tgs_mandate_size);
  retVal = send(s, encrypted_tgs_mandate, encrypted_tgs_mandate_size, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to send\n");
    WSACleanup();
    exit(-1);
  }

  msg_len = retVal = recv(s, buf, buf_len, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to recv\n");
    WSACleanup();
    exit(-1);
  }
  FAIL_IF_RESP_EMPTY(retVal);

  printf("Got 1st part from TGS (session key). Encrypted: ");
  print_hex_binary(buf, msg_len);
  DWORD actual_length = do_crypt(session_key, DECODE, buf, buf, msg_len, msg_len);
  printf("Decrypted: ");
  print_hex_binary(buf, actual_length);
  get_key_from_binary(buf, actual_length, prov, r_key);

  msg_len = retVal = recv(s, buf, buf_len, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to recv\n");
    WSACleanup();
    exit(-1);
  }
  FAIL_IF_RESP_EMPTY(retVal);

  printf("Got 2nd part from TGS (encrypted RS mandate): ");
  print_hex_binary(buf, msg_len);
  *encrypted_r_mandate_size = msg_len;
  *encrypted_r_mandate = (BYTE*) malloc(msg_len);
  memcpy(*encrypted_r_mandate, buf, msg_len);

  close_socket(s);
}

void communicate_with_r(const char *host, char *name, HCRYPTPROV prov, HCRYPTKEY session_key,
                        BYTE *encrypted_r_mandate, DWORD encrypted_r_mandate_size)
{
  printf("Initializing connection to RS\n");
  SOCKET s = connect_to_server(host, R_PORT);

  int retVal = 0;
  const int buf_len = 10240;
  char buf[buf_len];

  struct Authenticator auth = {name, time(NULL)};
  DWORD msg_len = create_authenticator(buf, auth, session_key);

  printf("Sending authenticator: ");
  print_hex_binary(buf, msg_len);
  retVal = send(s, buf, msg_len, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to send\n");
    WSACleanup();
    exit(-1);
  }

  printf("Sending RS mandate: ");
  print_hex_binary(encrypted_r_mandate, encrypted_r_mandate_size);
  retVal = send(s, encrypted_r_mandate, encrypted_r_mandate_size, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to send\n");
    WSACleanup();
    exit(-1);
  }

  msg_len = retVal = recv(s, buf, buf_len, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to recv\n");
    WSACleanup();
    exit(-1);
  }
  FAIL_IF_RESP_EMPTY(retVal);

  printf("Got encrypted timestamp: ");
  print_hex_binary(buf, msg_len);
  msg_len = do_crypt(session_key, DECODE, buf, buf, msg_len, msg_len);
  printf("Decrypted binary: ");
  print_hex_binary(buf, msg_len);
  time_t *resp_time = (time_t*) buf;
  printf("Decrypted time: %d\n", *resp_time);
  if (auth.now + 1 == *resp_time) {
    printf("AUTH SUCCESS!\n");
  } else {
    printf("Got wrong time from server (%d + 1 != %d), authentication failed.\n", auth.now, *resp_time);
  }
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
    BYTE *key = NULL;
    DWORD key_size = 0;
    get_key_blob(prov, argv[4], &key, &key_size);
    user.key = key;
    user.key_size = key_size;

    append_self_to_users_file(argv[2], user);
  } else if (strcmp(argv[1], "--authenticate") == 0) {
    BYTE *key_blob = NULL;
    DWORD key_size = 0;
    get_key_blob(prov, argv[4], &key_blob, &key_size);

    HCRYPTKEY user_key;
    get_key_from_binary(key_blob, key_size, prov, &user_key);

    HCRYPTKEY tgs_session_key;
    BYTE *encrypted_tgs_mandate = NULL;
    DWORD encrypted_tgs_mandate_size = 0;
    communicate_with_as(argv[2], argv[3], prov, user_key,
                        &tgs_session_key, &encrypted_tgs_mandate,
                        &encrypted_tgs_mandate_size);

    HCRYPTKEY r_session_key;
    BYTE *encrypted_r_mandate = NULL;
    DWORD encrypted_r_mandate_size = 0;
    communicate_with_tgs(argv[2], argv[3], prov, tgs_session_key,
                         encrypted_tgs_mandate, encrypted_tgs_mandate_size,
                         &r_session_key, &encrypted_r_mandate,
                         &encrypted_r_mandate_size);

    communicate_with_r(argv[2], argv[3], prov, r_session_key,
                       encrypted_r_mandate, encrypted_r_mandate_size);
  } else {
    fprintf(stderr, "Usage: %s [--append-self <users_file> | --authenticate <server_host> <server_port>] <name> <password>\n", argv[0]);
    return -1;
  }

  return 0;
}
