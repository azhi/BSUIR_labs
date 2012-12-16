#include <stdio.h>
#include <windows.h>

#include "err_helpers.h"
#include "my_ioctl.h"
#include "my_names.h"

int main(void)
{
  HANDLE hFile;
  DWORD bytes_returned;
  struct example_ioctl_data my_data;

  hFile = CreateFileA("\\\\.\\" MY_SERVICE_NAME_SHORT,
                      GENERIC_READ | GENERIC_WRITE, 0,
                      NULL, OPEN_EXISTING, 0, NULL);

  if (!hFile) {
    printf("Sorry, fail while opening device io control.\n");
    process_error();
    return -1;
  }

  my_data.code = 42;
  /* We'll just use the same buffer for input and output.
     Our CTL code specifies buffered operation (IN+OUT). */
  /*
  if (!DeviceIoControl(hFile, IOCTL_EXAMPLE, &my_data,
                       sizeof(struct example_ioctl_data),
                       &my_data,
                       sizeof(struct example_ioctl_data),
                       &bytes_returned, NULL)) {
    printf("Failed to access device io control.\n");
    process_error();
    CloseHandle(hFile);
    return -2;
  }
  */
  my_data.code = MCODE_ADD;
  wcscpy(my_data.hprog,
          L"\\Device\\HarddiskVolume1\\WINDOWS\\System32\\calc.exe");
  wcscpy(my_data.rprog, L"c:\\windows\\system32\\notepad.exe");
  if (!DeviceIoControl(hFile, IOCTL_EXAMPLE, &my_data,
                       sizeof(struct example_ioctl_data),
                       &my_data,
                       sizeof(struct example_ioctl_data),
                       &bytes_returned, NULL)) {
    printf("Failed to access device io control.\n");
    process_error();
    CloseHandle(hFile);
    return -2;
  }
  CloseHandle(hFile);

  return 0;
}
