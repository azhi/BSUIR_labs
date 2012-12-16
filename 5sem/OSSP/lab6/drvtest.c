/*
 * by Nick Herman
 *
 * Based on code by Frank Rysanek <rysanek@fccps.cz>
 * Based on the source code of ioperm.c by Marcel Telka <marcel@telka.sk>
 *   - thanks a million :-)
 *
 * Further knowledge taken from a series of 6 articles called
 * "Driver Development" published by Toby Opferman at CodeProject.com
 */

#include <ddk/ntddk.h>
#include <ddk/ntifs.h>
#include <ddk/ntapi.h>
#include <ntdef.h>

#include "my_ioctl.h"
#include "my_names.h"

/* undocumented functions */
/* Used by Telka's ioperm - may come in handy [*** grin ***]
void STDCALL Ke386IoSetAccessProcess( PEPROCESS, int );
void STDCALL Ke386QueryIoAccessMap( int, unsigned char * );
void STDCALL Ke386SetIoAccessMap( int, unsigned char * );
*/

/* workaround for bug 666316 and bug 671964   (by Marcel Telka) */
/* Huh? How relevant is this? Commenting it out... (by Frank Rysanek) */
/*
#undef IoGetCurrentIrpStackLocation
#define IoGetCurrentIrpStackLocation(irp) (*(struct _IO_STACK_LOCATION**)((char*)(irp) + 96))
*/


/******************************************************************************
 *                                Some globals                                *
 ******************************************************************************/

typedef struct PROC_ENTRY {
  DWORD pid;
  DWORD sl_pid;
} PROC_ENTRY;

#define ENT_CNT 256
PROC_ENTRY g_proc_table[ENT_CNT];

#define PRG_CNT 20
WCHAR g_pwc_hprogs[PRG_CNT][512];
WCHAR g_pwc_rprogs[PRG_CNT][512];

UNICODE_STRING g_usz_hprogs[PRG_CNT];
UNICODE_STRING g_usz_rprogs[PRG_CNT];

int g_used_progs = 0;

KMUTEX mutex;

/******************************************************************************
 *                                 Prototypes                                 *
 ******************************************************************************/
static VOID DDKAPI
my_unload(PDRIVER_OBJECT DriverObject);

static NTSTATUS DDKAPI
my_dispatch_create(PDEVICE_OBJECT DeviceObject, PIRP Irp);
static NTSTATUS DDKAPI
my_dispatch_device_control(PDEVICE_OBJECT DeviceObject, PIRP Irp);

VOID DDKAPI
create_process_watcher(HANDLE dwParentId, HANDLE dwProcessId, BOOLEAN create);
VOID DDKAPI
load_image_watcher(PUNICODE_STRING  uszFullImageName,
                   HANDLE dwProcessId, PIMAGE_INFO pImageInfo);

void run_process(DWORD dwProcessId, WCHAR *pwcProg);
void terminate_process(DWORD dwProcessId);

extern ULONG WINAPI RtlRandomEx(PULONG Seed);

/******************************************************************************
 ***  ==  ==  ==  ==  ==  => The driver entry point <=  ==  ==  ==  ==  ==  ***
 ******************************************************************************/
NTSTATUS DDKAPI
DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
  NTSTATUS status;
  PDEVICE_OBJECT DeviceObject;
  UNICODE_STRING DeviceNameW;
  UNICODE_STRING SymbolicLinkNameW;
  ANSI_STRING DeviceNameA;
  ANSI_STRING SymbolicLinkNameA;

  DbgPrint("DriverEntry called\r\n");

  // support for service stopping
  DriverObject->DriverUnload = my_unload;
  // create support
  DriverObject->MajorFunction[IRP_MJ_CREATE] = my_dispatch_create;
  // IOCTL support
  DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] =
    my_dispatch_device_control;

  KeInitializeMutex(&mutex, 0);
  //KeReleaseMutex(&mutex, FALSE);

  for (DWORD i = 0; i < ENT_CNT; ++i) {
    g_proc_table[i].pid = 0;
    g_proc_table[i].sl_pid = 0;
  }

  /* initialize counted unicode strings
   * If I want to use normal string combining logic in my_names.h,
   * need to mess with ANSI vs. Unicode */
  RtlInitString(&DeviceNameA, MY_DEVICE_NAME);
  RtlAnsiStringToUnicodeString(&DeviceNameW, &DeviceNameA, TRUE);
  RtlInitString(&SymbolicLinkNameA, MY_DOSDEVICE_NAME);
  RtlAnsiStringToUnicodeString(&SymbolicLinkNameW, &SymbolicLinkNameA, TRUE);

  // create device object
  status = IoCreateDevice(DriverObject, 0, &DeviceNameW, FILE_DEVICE_UNKNOWN,
                          0, FALSE, &DeviceObject);

  if (NT_SUCCESS(status)) {
    /* Clear a flag, set by IoCreateDevice.
     * This is not mandatory within DriverEntry,
     * but it *is* mandatory if used anywhere else. */
    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

    // create user-visible name for the device
    status = IoCreateSymbolicLink(&SymbolicLinkNameW, &DeviceNameW);
  }

  // Initializing process creation/load handlers
  if (NT_SUCCESS(status))
    status = PsSetCreateProcessNotifyRoutine(create_process_watcher, FALSE);
  if (NT_SUCCESS(status))
    status = PsSetLoadImageNotifyRoutine(load_image_watcher);

  if (!NT_SUCCESS(status))
    return status;

  return STATUS_SUCCESS;
}

/******************************************************************************
 *                            Driver unload handler                           *
 ******************************************************************************/
static VOID DDKAPI
my_unload(PDRIVER_OBJECT DriverObject)
{
  ANSI_STRING SymbolicLinkNameA;
  UNICODE_STRING SymbolicLinkNameW;
  DbgPrint("DriverUnload called\r\n");

  PsSetCreateProcessNotifyRoutine(create_process_watcher, TRUE);
  PsRemoveLoadImageNotifyRoutine(load_image_watcher);

  RtlInitString(&SymbolicLinkNameA, MY_DOSDEVICE_NAME);
  RtlAnsiStringToUnicodeString(&SymbolicLinkNameW, &SymbolicLinkNameA, TRUE);

  IoDeleteSymbolicLink(&SymbolicLinkNameW);
  IoDeleteDevice(DriverObject->DeviceObject);

  for (int i = 0; i < ENT_CNT; ++i)
    if(g_proc_table[i].pid)
      DbgPrint("Registered process stays: %d\r\n", g_proc_table[i].pid);
}

/******************************************************************************
 *                Device communication initialization handler                 *
 ******************************************************************************/
static NTSTATUS DDKAPI
my_dispatch_create(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
  Irp->IoStatus.Status = STATUS_SUCCESS;
  Irp->IoStatus.Information = 0;
  IoCompleteRequest( Irp, IO_NO_INCREMENT );

  return STATUS_SUCCESS;
}

/*
 Promising functions:
 PVOID MmMapIoSpace(PHYSICAL_ADDRESS phy, SIZE_T num_bytes, MmNonCached);
 VOID MmUnmapIoSpace(PVOID virt, SIZE_T num_bytes);

 Reserved for system use:
 MmGetPhysicalMemoryRanges()
 MmGetVirtualForPhysical()
 MmIsIoSpaceActive()
*/

#define EXIT_PROC                               \
  IoCompleteRequest(Irp, IO_NO_INCREMENT);      \
  return Irp->IoStatus.Status

/******************************************************************************
 *                            Messages handler                                *
 ******************************************************************************/
static NTSTATUS DDKAPI
my_dispatch_device_control(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
  PIO_STACK_LOCATION io_stack;

  // default return values
  Irp->IoStatus.Status = STATUS_NOT_IMPLEMENTED;
  Irp->IoStatus.Information = 0; /* !!! override as needed !!! */
  /* "Information" = dwBytesReturned = override this in switch cases
   * returning some data to user space */

  // get current I/O stack location
  io_stack = IoGetCurrentIrpStackLocation(Irp);
  if (!io_stack) {
    Irp->IoStatus.Status = STATUS_INTERNAL_ERROR;
    EXIT_PROC;
  }

  // if this fires, we've hooked up the ioctl() handler
  //   to the wrong callback slot
  if (io_stack->MajorFunction != IRP_MJ_DEVICE_CONTROL) {
    Irp->IoStatus.Status = STATUS_INTERNAL_ERROR;
    EXIT_PROC;
  }

  switch (io_stack->Parameters.DeviceIoControl.IoControlCode) {
  case IOCTL_EXAMPLE: {
    // get input buffer
    example_ioctl_data* my_data =
      (example_ioctl_data *) Irp->AssociatedIrp.SystemBuffer;

    // test input buffer size and existence
    if ((io_stack->Parameters.DeviceIoControl.InputBufferLength <
         sizeof(example_ioctl_data)) || !my_data) {
      Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
      EXIT_PROC;
    }

    // else okay, ready to go
    switch (my_data->code) {
    case MCODE_TEST:
      DbgPrint("ioctl(IOCTL_EXAMPLE) called, arg = %d\r\n", my_data->code);
      break;

    case MCODE_ADD:
      wcscpy(g_pwc_hprogs[g_used_progs], my_data->hprog);
      wcscpy(g_pwc_rprogs[g_used_progs], my_data->rprog);
      RtlInitUnicodeString(&g_usz_hprogs[g_used_progs],
                           g_pwc_hprogs[g_used_progs]);
      RtlInitUnicodeString(&g_usz_rprogs[g_used_progs],
                           g_pwc_rprogs[g_used_progs]);

      DbgPrint("Adding: %wZ -> %wZ\r\n",
               &g_usz_hprogs[g_used_progs], &g_usz_rprogs[g_used_progs]);
      ++g_used_progs;
      break;

    default:
      DbgPrint("Ignoring call with code %d\r\n", my_data->code);
      break;
    }
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0; // No data to copy_to_user()...
  }
    break;

  default: // STATUS_NOT_IMPLEMENTED takes action
    break;
  }

  EXIT_PROC;
}

#undef EXIT_PROC

/******************************************************************************
 *                          Process creation/deletion callback                *
 ******************************************************************************/
// Attention! PID has a HANDLE type. But it still is PID.
VOID DDKAPI
create_process_watcher(HANDLE dwParentId, HANDLE dwProcessId, BOOLEAN create)
{
  DbgPrint("CP callback runned PID: %5d, Status: %s\r\n",
           dwProcessId, create ? "up" : "down");

  DWORD i;
  if (create)
    // Process creation is handled on load, not creation
    return;

  for (i = 0; i < ENT_CNT && g_proc_table[i].sl_pid != dwProcessId; ++i);
  if (i != ENT_CNT)
    g_proc_table[i].sl_pid = 0;

  for (i = 0; i < ENT_CNT && g_proc_table[i].pid != dwProcessId; ++i);
  if (i == ENT_CNT) {
    // The process is not in list
    return;
  }
  DWORD pid = g_proc_table[i].sl_pid;
  g_proc_table[i].pid = 0;
  g_proc_table[i].sl_pid = 0;
  terminate_process(pid);
  return;
}

/******************************************************************************
 *                             Module load callback                           *
 ******************************************************************************/
// Attention! PID has a HANDLE type. But it still is PID.
VOID DDKAPI
load_image_watcher(PUNICODE_STRING  uszFullImageName, HANDLE dwProcessId,
                   PIMAGE_INFO pImageInfo)
{
  //DbgPrint("LI callback runned PID: %d, Name: %wZ\r\n",
  //         dwProcessId, uszFullImageName);
  int j;
  for (j = 0;
       j < g_used_progs &&
         !RtlEqualUnicodeString(&g_usz_hprogs[j], uszFullImageName, TRUE);
       ++j)
    //DbgPrint("%wZ\r\nvs\r\n%wZ\r\n", &g_usz_hprogs[j], uszFullImageName);
    ;
  if (j == g_used_progs)
    return;

  int i;
  // Let us find a free entry
  for (i = 0; i < ENT_CNT && g_proc_table[i].pid; ++i);
  if (g_proc_table[i].pid) {
    // No more free entries
    DbgPrint("Sorry, can not handle this process\r\n");
    return;
  }
  g_proc_table[i].pid = dwProcessId;
  run_process(i, g_pwc_rprogs[j]);

  return;
}

/*
 * This helper runs a program from the driver service.
 *   Connection is estabilished with named pipe.
 */
void run_process(DWORD i, WCHAR *pwcProg)
{
  DbgPrint("Runing %ls...\r\n", pwcProg);
  while (!NT_SUCCESS(KeWaitForMutexObject(&mutex, Executive,
                                          KernelMode, FALSE, NULL)));

  LARGE_INTEGER delay = RtlConvertUlongToLargeInteger(300000l);

  NTSTATUS status;
  HANDLE pipe;
  OBJECT_ATTRIBUTES fattrs;
  UNICODE_STRING pipe_name;
  IO_STATUS_BLOCK io_stat_block;
  RtlInitUnicodeString(&pipe_name, L"\\??\\pipe\\drvtest");
  InitializeObjectAttributes(&fattrs, &pipe_name,
                             OBJ_CASE_INSENSITIVE | 0x0200/*OBJ_KERNEL_HANDLE*/,
                             0, NULL);
  status = ZwCreateFile(&pipe, FILE_WRITE_DATA | FILE_READ_DATA | SYNCHRONIZE,
                        &fattrs, &io_stat_block, NULL, 0,
                        FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN,
                        FILE_NON_DIRECTORY_FILE, NULL, 0);
  if (!NT_SUCCESS(status))
    DbgPrint("Alert! 0x%0.8x, 0x%0.8x\r\n", status, io_stat_block.Status);

  example_ioctl_data out_data = { code: MCODE_SPAWN };
  wcscpy(out_data.rprog, pwcProg);
  status = ZwWriteFile(pipe, NULL, NULL, NULL, &io_stat_block, &out_data,
                       sizeof(out_data), NULL, NULL);
  if (!NT_SUCCESS(status))
    DbgPrint("Alert! 0x%0.8x\r\n", status);

  DWORD pid;
  do {
    status = ZwReadFile(pipe, NULL, NULL, NULL, &io_stat_block, &pid,
                        sizeof(DWORD), NULL, NULL);
    if (!NT_SUCCESS(status))
      KeDelayExecutionThread(KernelMode, FALSE, &delay);
  } while(STATUS_PENDING == status);
  if (!NT_SUCCESS(status))
    DbgPrint("Alert! 0x%0.8x\r\n", status);

  DbgPrint("PID: %d\r\n", pid);
  g_proc_table[i].sl_pid = pid;
  ZwClose(pipe);
  KeReleaseMutex(&mutex, FALSE);
  return;
  /* return proc_infn.dwProcessId; */
}

void terminate_process(DWORD dwProcessId)
{
  if (!dwProcessId)
    return;

  DbgPrint("Killing %d...\r\n", dwProcessId);
  while (!NT_SUCCESS(KeWaitForMutexObject(&mutex, Executive,
                                          KernelMode, FALSE, NULL)));

  LARGE_INTEGER delay = RtlConvertUlongToLargeInteger(300000l);

  NTSTATUS status;
  HANDLE pipe;
  OBJECT_ATTRIBUTES fattrs;
  UNICODE_STRING pipe_name;
  IO_STATUS_BLOCK io_stat_block;
  RtlInitUnicodeString(&pipe_name, L"\\??\\pipe\\drvtest");
  InitializeObjectAttributes(&fattrs, &pipe_name,
                             OBJ_CASE_INSENSITIVE | 0x0200/*OBJ_KERNEL_HANDLE*/,
                             0, NULL);
  for (int i = 0; i < 10; ++i) {
    status = ZwCreateFile(&pipe, FILE_WRITE_DATA | SYNCHRONIZE,
                          &fattrs, &io_stat_block, NULL, 0,
                          FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN,
                          FILE_NON_DIRECTORY_FILE, NULL, 0);
    if (!NT_SUCCESS(status))
      KeDelayExecutionThread(KernelMode, FALSE, &delay);
    else
      break;
  }
  if (!NT_SUCCESS(status))
    DbgPrint("Alert! 0x%0.8x, 0x%0.8x\r\n", status, io_stat_block.Status);

  example_ioctl_data out_data = { code: MCODE_TERM, pid: dwProcessId };
  status = ZwWriteFile(pipe, NULL, NULL, NULL, &io_stat_block, &out_data,
                       sizeof(out_data), NULL, NULL);
  if (!NT_SUCCESS(status))
    DbgPrint("Alert! 0x%0.8x\r\n", status);

  ZwClose(pipe);
  KeReleaseMutex(&mutex, FALSE);
  return;
}
