#include <winsock2.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "crypt.h"

struct Users {
  DWORD count;
  struct User *users;
};

BOOL exited = FALSE;

struct Users load_users_file(const char *path)
{
  struct Users res = {0, malloc(sizeof(struct User) * 100)};
  FILE* users_file = fopen(path, "rb");
  while (1) {
    struct User user;

    DWORD name_length = 0;
    fread(&name_length, 2, 1, users_file);

    if (feof(users_file))
      break;

    LPTSTR name = (LPTSTR) malloc(name_length + 1);
    fread(name, 1, name_length, users_file);
    name[name_length] = '\0';
    user.name = name;

    DWORD key_length = 0;
    fread(&key_length, 2, 1, users_file);
    LPTSTR key = (LPTSTR) malloc(key_length);
    fread(key, 1, key_length, users_file);
    user.key_size = key_length;
    user.key = key;

    res.users[res.count++] = user;
  }
  fclose(users_file);
  return res;
}

struct User *find_user_by_name(const char *name, struct Users users)
{
  int user_index = 0;
  BOOL found = FALSE;
  for (int i = 0; i < users.count; i++) {
    if (strcmp(users.users[i].name, name) == 0) {
      user_index = i;
      found = TRUE;
      break;
    }
  }
  if (found)
    return &users.users[user_index];
  else {
    return NULL;
  }
}

SOCKET initialize_server_socket()
{
  WORD sockVer;
  WSADATA wsaData;
  int retVal;

  sockVer = MAKEWORD(2,2);

  WSAStartup(sockVer, &wsaData);

  SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (servSock == INVALID_SOCKET) {
    fprintf(stderr, "Unable to create socket\n");
    WSACleanup();
    exit(-1);
  }

  SOCKADDR_IN sin;
  sin.sin_family = PF_INET;
  sin.sin_port = htons(AS_PORT);
  sin.sin_addr.s_addr = INADDR_ANY;

  retVal = bind(servSock, (LPSOCKADDR)&sin, sizeof(sin));
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to bind\n");
    WSACleanup();
    exit(-1);
  }

  retVal = listen(servSock, 10);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to listen\n");
    WSACleanup();
    exit(-1);
  }

  return servSock;
}

void process_client(SOCKET sock, HCRYPTPROV prov, struct Users users, HCRYPTKEY tgs_key)
{
  printf("---------- Start processing client ----------\n");
  int retVal;
  char* buf = malloc(10240);

  retVal = recv(sock, buf, 10240, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to recv\n");
    exit(-1);
  }
  buf[retVal] = '\0';
  printf("Name: %s\n", buf);

  struct User *user = find_user_by_name(buf, users);

  if (user == NULL) {
    printf("Couldn't find user with that name. Closing connection.\n");
    closesocket(sock);
    return;
  }

  HCRYPTKEY user_key;
  get_key_from_binary(user->key, user->key_size, prov, &user_key);

  printf("Generate new session key\n");
  DWORD session_key_size = 260;
  BYTE session_key_blob[session_key_size];
  generate_session_key(prov, &session_key_size, session_key_blob);

  printf("Session key plain: ");
  print_hex_binary(session_key_blob, session_key_size);
  DWORD msg_len = do_crypt(user_key, ENCODE, session_key_blob, buf, session_key_size, 10240);
  printf("Sending 1st part of the message (encrypted session key): ");
  print_hex_binary(buf, msg_len);
  retVal = send(sock, buf, msg_len, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to send\n");
    exit(-1);
  }

  struct Mandate mandate = {"TGS", user->name, time(NULL) + 24 * 60 * 60, session_key_blob, session_key_size};
  msg_len = create_mandate(buf, mandate, tgs_key);
  printf("Sending 2nd part of the message (encrypted mandate): ");
  print_hex_binary(buf, msg_len);
  retVal = send(sock, buf, msg_len, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to send\n");
    exit(-1);
  }

  printf("---------- Done processing client ----------\n");
  free(buf);
  closesocket(sock);
}

void server_loop(SOCKET sock, HCRYPTPROV prov, struct Users users, HCRYPTKEY tgs_key)
{
  SOCKET clientSock;
  while (!exited) {
    fd_set rfds;
    struct timeval tv;
    int retVal;

    FD_ZERO(&rfds);
    FD_SET(sock, &rfds);
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    retVal = select(1, &rfds, NULL, NULL, &tv);
    if (retVal == -1) {
      fprintf(stderr, "Select error\n");
      WSACleanup();
      exit(-1);
    } else if (retVal) {
      clientSock = accept(sock, NULL, NULL);
      if (clientSock == INVALID_SOCKET) {
        fprintf(stderr, "Unable to accept\n");
        WSACleanup();
        exit(-1);
      }

      process_client(clientSock, prov, users, tgs_key);
    }
  }
}

void trap_exit(int signal)
{
  exited = TRUE;
}

int main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <users_file> <tgs_key_file>", argv[0]);
    return -1;
  }

  struct Users users = load_users_file(argv[1]);

  HCRYPTPROV prov;
  create_prov(&prov);

  HCRYPTKEY tgs_key;
  load_binary_key_from_file(argv[2], prov, &tgs_key);

  SOCKET sock = initialize_server_socket();

  signal(SIGINT, trap_exit);
  server_loop(sock, prov, users, tgs_key);

  closesocket(sock);
  finalize_prov(prov);

  return 0;
}
