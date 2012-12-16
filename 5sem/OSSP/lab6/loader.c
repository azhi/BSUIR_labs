/*
 * File created by Frank Rysanek <rysanek@fccps.cz>
 * Source code taken almost verbatim from "Driver Development, Part 1"
 * published by Toby Opferman at CodeProject.com
 */

#include <stdio.h>
#include <windows.h>
#include <unistd.h>    /* getcwd() */

#include "err_helpers.h"
#include "my_ioctl.h"
#include "my_names.h"

#define MAX_CWD_LEN 10240
/* the XXXXX.sys filename will get appended to this as well */
static char cwd[MAX_CWD_LEN+5];

DWORD WINAPI pipe_server(HANDLE pipe);
DWORD spawn_proc(LPWSTR prog);
void term_proc(DWORD pid);

int main(void)
{
  HANDLE pipe = INVALID_HANDLE_VALUE;
  LPWSTR pipe_name = L"\\\\.\\pipe\\drvtest";

  wprintf(L"Pipe path: %s\n", pipe_name);

  pipe = CreateNamedPipeW(pipe_name, PIPE_ACCESS_DUPLEX | WRITE_DAC,
                          PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                          PIPE_UNLIMITED_INSTANCES, sizeof(example_ioctl_data),
                          sizeof(example_ioctl_data), 0, NULL);
  if (pipe == INVALID_HANDLE_VALUE) {
    printf("Failed to create pipe. Error code: %d.\n", GetLastError());
    return -1;
  }

  CreateThread(NULL, 0, pipe_server, pipe, 0, NULL);
  CreateThread(NULL, 0, pipe_server, pipe, 0, NULL);
  CreateThread(NULL, 0, pipe_server, pipe, 0, NULL);

  HANDLE hSCManager;
  HANDLE hService;
  SERVICE_STATUS ss;
  int retval = 0;

  /* First of all, maybe concatenate the current working directory
   * with the desired driver file name - before we start messing with
   * the service manager etc. */
  if (getcwd(cwd, MAX_CWD_LEN) == NULL) {
    printf("Failed to learn the current working directory!\n");
    retval = -8;
    goto err_out1;
  } /* else got CWD just fine */

  if (strlen(cwd) + strlen(MY_DRIVER_FILENAME) + 1 > MAX_CWD_LEN) {
    printf("Current working dir + driver filename is longer than %d.\n",
           MAX_CWD_LEN);
    retval = -9;
    goto err_out1;
  } /* else our buffer is long enough */

  strcat(cwd, "\\");
  strcat(cwd, MY_DRIVER_FILENAME);
  printf("Driver path+name: %s\n", cwd);

  printf("Going to open the service manager... ");
  hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
  if (!hSCManager) {
    printf("FAIL.\n");
    process_error();
    retval = -1;
    goto err_out1;
  }

  printf("OK.\n");
  printf("Going to create the service... ");

  hService = CreateService(hSCManager, MY_SERVICE_NAME_SHORT,
                           MY_SERVICE_NAME_LONG,
                           SERVICE_START | DELETE | SERVICE_STOP,
                           SERVICE_KERNEL_DRIVER,
                           SERVICE_DEMAND_START,
                           SERVICE_ERROR_IGNORE,
                           cwd, NULL, NULL, NULL, NULL, NULL);

  if(!hService) {
    process_error();
    printf("\n already exists? Trying to open it... ");
    hService = OpenService(hSCManager, MY_SERVICE_NAME_SHORT,
                           SERVICE_START | DELETE | SERVICE_STOP);
  }

  if(!hService) {
    printf("FAILED.\n");
    process_error();
    retval = -2;
    goto err_out2;
  }

  printf("OK.\n");
  printf("Going to start the service... ");

  if (StartService(hService, 0, NULL) == 0) {
    printf("FAIL\n");
    process_error();
    retval = -3;
    goto err_out3;
  }

  printf("OK.\n");
  printf("\n >>> Press Enter to unload the driver. <<<\n");
  getchar();

  printf("Going to stop the service... ");
  if (ControlService(hService, SERVICE_CONTROL_STOP, &ss) == 0) {
    printf("FAIL.\n");
    process_error();
    retval = -4;
  } else printf("OK.\n");

 err_out3:
  printf("Going to delete the service... ");
  if (DeleteService(hService) == 0) {
    printf("FAIL.\n");
    process_error();
    retval = -5;
  } else printf("OK.\n");

  printf("Going to close the service handle... ");
  if (CloseServiceHandle(hService) == 0) {
    printf("FAIL.\n");
    process_error();
    retval = -6;
  } else printf("OK.\n");

 err_out2:
  printf("Going to close the service manager... ");
  if (CloseServiceHandle(hSCManager) == 0) {
    printf("FAIL.\n");
    process_error();
    retval = -7;
  } else printf("OK.\n");

err_out1:
  printf("Finished!\n");
  return(retval);
}

DWORD WINAPI pipe_server(HANDLE pipe)
{
  // Primitive pipe server
  example_ioctl_data in_data;
  BOOL resume = TRUE;
  for (;resume;) {
    BOOL connected = ConnectNamedPipe(pipe, NULL) ? TRUE :
      (GetLastError() == ERROR_PIPE_CONNECTED);
    if (connected) {
      for (;;) {
        DWORD b_read;
        BOOL success = ReadFile(pipe, &in_data, sizeof(example_ioctl_data),
                                &b_read, NULL);
        if (!success || (b_read < sizeof(example_ioctl_data))) {
          DisconnectNamedPipe(pipe);
          break;
        } else {
          DWORD pid = 0;

          switch (in_data.code) {
          case MCODE_QUIT:
            resume = FALSE;
            break;

          case MCODE_SPAWN:
            pid = spawn_proc(in_data.rprog);
            //printf("%d\n", pid);
            WriteFile(pipe, &pid, sizeof(DWORD),
                      &b_read, NULL);
            break;

          case MCODE_TERM:
            term_proc(in_data.pid);
            break;

          default:
            break;
          }
          if (!resume)
            break;
        }
      }
    }
  }
  MessageBoxA(NULL, "Closing...", "Closing...", 0);
  CloseHandle(pipe);
  return 0;
}

DWORD spawn_proc(LPWSTR prog)
{
  //MessageBoxW(NULL, L"Spawning!", prog, 0);
  STARTUPINFOW stp_info = {sizeof(stp_info)};
  PROCESS_INFORMATION proc_infn;
  CreateProcessW(prog, NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS,
                 NULL, NULL, &stp_info, &proc_infn);
  return proc_infn.dwProcessId;
}

void term_proc(DWORD pid)
{
  HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
  TerminateProcess(hProc, 0);
  return;
}
