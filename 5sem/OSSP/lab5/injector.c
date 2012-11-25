#include <stdio.h>
#include <time.h>

#include <windows.h>

#include "common.h"

int main(int argc, char **argv)
{
  srand((unsigned)time(NULL));
  //for (int i = 0; i < argc; ++i)
  //  printf("%2d: %s\n", i, argv[i]);

  HANDLE pipe = INVALID_HANDLE_VALUE;
  CHAR pipe_name[256];
  sprintf(pipe_name, "\\\\.\\pipe\\inj%0.4x%0.4x", rand(), rand());

  printf("Pipe path: %s\n", pipe_name);

  pipe = CreateNamedPipe(pipe_name, PIPE_ACCESS_DUPLEX | WRITE_DAC,
                         PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                         PIPE_UNLIMITED_INSTANCES, 1024, 1024, 0, NULL);
  if (pipe == INVALID_HANDLE_VALUE) {
    printf("Failed to create pipe. Error code: %d.", GetLastError());
    return -1;
  }

  char *target_name = argv[1];
  char *injn_name   = argv[2]; SIZE_T injn_name_len = strlen(injn_name);

  printf("LoadLibraryA address: ");
  LPTHREAD_START_ROUTINE loader_addr =
    (LPTHREAD_START_ROUTINE) GetProcAddress(LoadLibraryA("kernel32.dll"),
                                            "LoadLibraryA");
  printf("0x%0.8x.\n", loader_addr);

  STARTUPINFO stp_info = {sizeof(stp_info)};
  PROCESS_INFORMATION proc_infn;

  printf("Launching %s: ", target_name);
  CreateProcess(NULL, target_name, NULL, NULL, FALSE, CREATE_SUSPENDED,
                NULL, NULL, &stp_info, &proc_infn);
  printf("done. Process handle is 0x%0.8x.\n", proc_infn.hProcess);

  printf("Allocating %d Bytes into target process memspace: ", injn_name_len);
  LPVOID vpointer = VirtualAllocEx(proc_infn.hProcess, NULL, injn_name_len,
                                   MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  printf("allocated at 0x%0.8x.\n", vpointer, GetLastError());

  printf("Writing to allocated target memory: ");
  WriteProcessMemory(proc_infn.hProcess, vpointer,
                     injn_name, injn_name_len, NULL);
  printf("done.\n");

  printf("Creating remote thread: ");
  HANDLE hth = CreateRemoteThread(proc_infn.hProcess, NULL, 0,
                                  loader_addr, vpointer, 0, NULL);
  printf("created with handle: 0x%0.8x.\n", hth);

  printf("Getting target process injection location: ");
  DWORD trg_injn_mod_addr;
  do {
    GetExitCodeThread(hth, &trg_injn_mod_addr);
    asm("xor %eax, %eax");
  } while (STILL_ACTIVE == trg_injn_mod_addr);
  printf("located at 0x%0.8x.\n", trg_injn_mod_addr);

  VirtualFreeEx(proc_infn.hProcess, vpointer, injn_name_len, MEM_DECOMMIT);

  HINSTANCE loc_injn_mod_addr = LoadLibraryA(injn_name);
  LPTHREAD_START_ROUTINE loc_injn_init_addr =
    (LPTHREAD_START_ROUTINE) GetProcAddress(loc_injn_mod_addr, "init");

  printf("Local injection module location: 0x%0.8x\n", loc_injn_mod_addr);
  LPTHREAD_START_ROUTINE trg_injn_init_addr =
    loc_injn_init_addr - (DWORD) loc_injn_mod_addr + trg_injn_mod_addr;
  printf("Injection init fun location: 0x%0.8x\n", trg_injn_init_addr);

  printf("Allocating %d Bytes into target process memspace: ", init_info_size);
  vpointer = VirtualAllocEx(proc_infn.hProcess, NULL, init_info_size,
                            MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  printf("allocated at 0x%0.8x.\n", vpointer, GetLastError());

  init_info_t init_info = { thread_id: proc_infn.dwThreadId };
  strcpy(init_info.pipe_name, pipe_name);

  printf("Writing to allocated target memory: ");
  WriteProcessMemory(proc_infn.hProcess, vpointer,
                     &init_info, init_info_size, NULL);
  ReadProcessMemory(proc_infn.hProcess, vpointer,
                    &init_info, init_info_size, NULL);

  printf("done.\n");

  hth = CreateRemoteThread(proc_infn.hProcess, NULL, 0,
                           trg_injn_init_addr, vpointer, 0, NULL);

  printf("Injection inserted and activated. Starting listening pipe.\n");

  // Primitive pipe server
  CHAR msg[1024];
  BOOL resume = TRUE;
  for (;resume;) {
    BOOL connected = ConnectNamedPipe(pipe, NULL) ? TRUE :
      (GetLastError() == ERROR_PIPE_CONNECTED);
    if (connected) {
      for (;;) {
        DWORD b_read;
        BOOL success = ReadFile(pipe, msg, 1024, &b_read, NULL);
        if (success) {
          if (!strcmp(msg, "QUIT")) resume = FALSE;
          printf("(from pipe) %s\n", msg);
        }
        if (!success || (b_read < 1024)) {
          DisconnectNamedPipe(pipe);
          break;
        } else printf("(from pipe) %s\n", msg);
      }
    }
  }

  printf("Got Quit message. Bye!\n");

  CloseHandle(pipe);
  ExitProcess(0);
  return 0;
}
