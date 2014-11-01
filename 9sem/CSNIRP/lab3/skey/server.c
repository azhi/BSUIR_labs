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

    DWORD n = 0;
    fread(&n, 2, 1, users_file);
    user.n = n;

    if (feof(users_file))
      break;

    DWORD name_length = 0;
    fread(&name_length, 2, 1, users_file);
    LPTSTR name = (LPTSTR) malloc(name_length + 1);
    fread(name, 1, name_length, users_file);
    name[name_length] = '\0';
    user.name = name;

    DWORD key_length = 0;
    fread(&key_length, 2, 1, users_file);
    LPTSTR key = (LPTSTR) malloc(key_length + 1);
    fread(key, 1, key_length, users_file);
    key[key_length] = '\0';
    user.key = key;

    res.users[res.count++] = user;
  }
  fclose(users_file);
  return res;
}

void save_users_file(const char *path, struct Users users)
{
  FILE* users_file = fopen(path, "wb");
  for (int i = 0; i < users.count; i++) {
    struct User user = users.users[i];

    fwrite(&user.n, 2, 1, users_file);

    DWORD name_length = strlen(user.name);
    fwrite(&name_length, 2, 1, users_file);
    fwrite(user.name, 1, name_length, users_file);

    DWORD key_length = strlen(user.key);
    fwrite(&key_length, 2, 1, users_file);
    fwrite(user.key, 1, key_length, users_file);
  }
  fclose(users_file);
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

SOCKET initialize_server_socket(const char *port)
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
  sin.sin_port = htons(atoi(port));
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

void process_client(SOCKET sock, HCRYPTPROV prov, struct Users users)
{
  printf("Start processing client\n");
  int retVal;
  char buf[10240];

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

  DWORD n_1 = user->n - 1;
  printf("Sending n-1: %d\n", n_1);
  retVal = send(sock, (char*) &n_1, 2, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to send\n");
    exit(-1);
  }

  retVal = recv(sock, buf, 10240, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to recv\n");
    exit(-1);
  }
  buf[retVal] = '\0';
  printf("Got key, hashing and comparing\n");
  LPTSTR hashed_key = do_hash_round(prov, buf);

  BOOL ans = strcmp(hashed_key, user->key) == 0;

  if (ans) {
    user->n = user->n - 1;
    free(user->key);
    user->key = malloc(strlen(buf) + 1);
    strcpy(user->key, buf);
    printf("Auth OK\n");
  } else {
    printf("Auth FAIL\n");
  }

  retVal = send(sock, (char*) &ans, 1, 0);
  if (retVal == SOCKET_ERROR) {
    fprintf(stderr, "Unable to send\n");
    exit(-1);
  }

  closesocket(sock);
}

void server_loop(SOCKET sock, HCRYPTPROV prov, struct Users users)
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

      process_client(clientSock, prov, users);
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
    fprintf(stderr, "Usage: %s <users_file> <server_port>", argv[0]);
    return -1;
  }

  struct Users users = load_users_file(argv[1]);

  HCRYPTPROV prov;
  create_prov(&prov);

  SOCKET sock = initialize_server_socket(argv[2]);

  signal(SIGINT, trap_exit);
  server_loop(sock, prov, users);

  printf("Saving users file...\n");
  save_users_file(argv[1], users);
  closesocket(sock);
  finalize_prov(prov);

  return 0;
}
