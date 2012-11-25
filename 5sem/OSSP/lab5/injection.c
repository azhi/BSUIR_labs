#include <windows.h>
#include <imagehlp.h>

#include <stdio.h>

#include "common.h"

BOOL send_quit_after_detach = FALSE;
LPSTR pipe_name = NULL;
HMODULE injn_base = NULL;

#define MakePtr(type, base, offset) ((type) (((void *)(base)) + (offset)))

// Some prototypes
PIMAGE_NT_HEADERS PEHeaderFromHModule(HMODULE hModule);
inline BOOL send_msg(LPSTR msg);
BOOL handle_fn(LPSTR mod_name, LPSTR fn_name, PROC *fn_addr_p);


/***********************************************************************
 * Injection initialization fun
 */
void __declspec(dllexport) init(init_info_t *init_info)
{
  char buf[1024];
  pipe_name = init_info->pipe_name;
  Sleep(40);
  send_msg("Injection initialization started.");

  HMODULE mod_exe_addr = GetModuleHandle(0);
  PIMAGE_NT_HEADERS exe_nt_hdr = PEHeaderFromHModule(mod_exe_addr);
  if (!exe_nt_hdr)
    return;

  send_msg("PE Header found.");

  DWORD import_rva = exe_nt_hdr->OptionalHeader.DataDirectory
    [IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
  if (!import_rva)
    return;

  send_msg("PE imports table found.");

  PIMAGE_IMPORT_DESCRIPTOR import_desc = MakePtr(PIMAGE_IMPORT_DESCRIPTOR,
                                                 mod_exe_addr, import_rva);
  while (import_desc->Name) {
    LPSTR import_module_name = MakePtr(LPSTR, mod_exe_addr, import_desc->Name);

    sprintf(buf, "Lib: %s.", import_module_name);
    send_msg(buf);

    //if (!lstrcmpiA(import_module_name, "KERNEL32.dll")) {
    PIMAGE_THUNK_DATA thunk = MakePtr(PIMAGE_THUNK_DATA, mod_exe_addr,
                                      import_desc->FirstThunk);
    PIMAGE_THUNK_DATA othunk = MakePtr(PIMAGE_THUNK_DATA,mod_exe_addr,
                                       import_desc->OriginalFirstThunk);

    for (;thunk->u1.Function; ++thunk, ++othunk) {
      if (!IMAGE_SNAP_BY_ORDINAL(thunk->u1.Ordinal)) {
        LPSTR fn_name = MakePtr(PIMAGE_IMPORT_BY_NAME, mod_exe_addr,
                                othunk->u1.AddressOfData)->Name;
        PROC *fn_addr_p = (PROC *) &thunk->u1.Function;
        handle_fn(import_module_name, fn_name, fn_addr_p);
      }
    }
    ++import_desc;
  }
  send_msg("Injection finished. Resuming main process thread.");

  if (init_info->thread_id) {
    send_quit_after_detach = TRUE;
    ResumeThread(OpenThread(THREAD_SUSPEND_RESUME, FALSE, init_info->thread_id));
  }
}

/***********************************************************************
 * DLL initialization fun
 */

BOOL __declspec(dllexport) APIENTRY DllMain(HINSTANCE hinst, DWORD reason,
                                            LPVOID _lpvReserved)
{
  switch (reason) {
  case DLL_PROCESS_ATTACH:{
    send_quit_after_detach = FALSE;
    pipe_name = NULL;
    injn_base = hinst;
    break;}

  case DLL_PROCESS_DETACH:
    if(send_quit_after_detach)
      send_msg("QUIT");
    break;

  case DLL_THREAD_ATTACH:
    break;

  case DLL_THREAD_DETACH:
    break;
  }

  return TRUE;
}

/***********************************************************************
 * Misc.
 */

PIMAGE_NT_HEADERS PEHeaderFromHModule(HMODULE hModule)
{
  PIMAGE_NT_HEADERS nt_hdr = 0;
  if (((PIMAGE_DOS_HEADER)hModule)->e_magic != IMAGE_DOS_SIGNATURE)
    return NULL;
  nt_hdr = (PIMAGE_NT_HEADERS) ((PBYTE) hModule +
                                ((PIMAGE_DOS_HEADER) hModule)->e_lfanew);
  if (nt_hdr->Signature != IMAGE_NT_SIGNATURE)
    nt_hdr = NULL;
  return nt_hdr;
}

PROC replace_fn_pointer(PROC *fn_addr_p, LPSTR new_fn_name)
{
  PROC repl = GetProcAddress(injn_base, new_fn_name);
  DWORD prev_sec;
  VirtualProtect(fn_addr_p, sizeof(fn_addr_p),
                 PAGE_EXECUTE_READWRITE, &prev_sec);
  //ReadProcessMemory(GetCurrentProcess(), fn_addr, &old_fn_addr,
  //                sizeof(old_fn_addr), NULL);
  BOOL success = WriteProcessMemory(GetCurrentProcess(), fn_addr_p, &repl,
                                    sizeof(repl), NULL);
  VirtualProtect(fn_addr_p, sizeof(fn_addr_p), prev_sec, &prev_sec);
  return success ? repl : NULL;
}

BOOL handle_fn(LPSTR mod_name, LPSTR fn_name, PROC *fn_addr_p)
{
  CHAR buf[1024];
  BOOL handled = FALSE;
  PROC old_fn_addr = *fn_addr_p;

  if (!lstrcmpiA(mod_name, "KERNEL32.dll")) {
    if (!lstrcmpiA(fn_name, "CreateFileA")) {
      PROC new_fn_addr = replace_fn_pointer(fn_addr_p, "my_create_file_a");
      sprintf(buf, " + %s at 0x%0.8x -> 0x%0.8x",
              fn_name, old_fn_addr, new_fn_addr);
      handled = (BOOL) new_fn_addr;
    } else if (!lstrcmpiA(fn_name, "CreateFileW")) {
      PROC new_fn_addr = replace_fn_pointer(fn_addr_p, "my_create_file_w");
      sprintf(buf, " + %s at 0x%0.8x -> 0x%0.8x",
              fn_name, old_fn_addr, new_fn_addr);
      handled = (BOOL) new_fn_addr;
    }
  }
  if (!handled)
    sprintf(buf, " - %s at 0x%0.8x", fn_name, *fn_addr_p);

  send_msg(buf);
}

BOOL send_msg(LPSTR msg)
{
  CHAR ans[1024];
  DWORD b_read;
  BOOL success = CallNamedPipe(pipe_name, msg, strlen(msg)+1,
                               ans, 1024, &b_read, 2000);

  //LPSTR str = malloc(1024);
  //sprintf(str, "pipe: ===> %d, %d, %s\n", success, GetLastError(), pipe_name);
  //MessageBoxA(NULL, str, "Hi!", 0);

  if (success) return TRUE;
  else return FALSE;
}

/***********************************************************************
 * Funs to replace with:
 */

HANDLE __declspec(dllexport)
my_create_file_a(LPCSTR filename, DWORD access, DWORD share_mode,
                 LPSECURITY_ATTRIBUTES sec_attrs, DWORD creat_dispos,
                 DWORD flags_attrs, HANDLE template)
{
  CHAR buf[1024];
  sprintf(buf, "Triggered: CreateFilaA with %s", filename);
  send_msg(buf);
  CreateFileA(filename, access, share_mode, sec_attrs, creat_dispos,
              flags_attrs, template);
  return NULL;
}

HANDLE __declspec(dllexport)
my_create_file_w(LPCWSTR filename, DWORD access, DWORD share_mode,
                 LPSECURITY_ATTRIBUTES sec_attrs, DWORD creat_dispos,
                 DWORD flags_attrs, HANDLE template)
{
  CHAR buff[1000], buf[1024];
  DWORD wrtn = WideCharToMultiByte(CP_ACP, 0, filename, wcslen(filename),
                                   buff, 1024, NULL, NULL);
  if (wrtn < 0) wrtn = 0;
  buff[wrtn] = '\0';
  sprintf(buf, "Triggered: CreateFilaW with %s", buff);
  send_msg(buf);
  CreateFileW(filename, access, share_mode, sec_attrs, creat_dispos,
              flags_attrs, template);
  return NULL;
}
