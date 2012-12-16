#include <stdio.h>
#include <windows.h>
#include <unistd.h>    /* getcwd() */
#include "my_names.h"
#include "err_helpers.h"

#define MAX_CWD_LEN 10240
/* the XXXXX.sys filename will get appended to this as well */
static char cwd[MAX_CWD_LEN+5];

int main(void)
{
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
  } /* else our buffer is long enough :-) */

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
  printf("Going to open the service... ");
  hService = OpenService(hSCManager, MY_SERVICE_NAME_SHORT,
                       SERVICE_START | DELETE | SERVICE_STOP);
  if(!hService) {
    printf("FAILED.\n");
    process_error();
    retval = -2;
    goto err_out2;
  }

  printf("OK.\n");
  printf("Going to stop the service... ");
  if (ControlService(hService, SERVICE_CONTROL_STOP, &ss) == 0) {
    printf("FAIL.\n");
    process_error();
    retval = -4;
  } else printf("OK.\n");

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
  } else printf("okay.\n");

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
