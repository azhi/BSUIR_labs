#include <windows.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  DWORD dwScope;
  DWORD dwType;
  DWORD dwDisplayType;
  LPSTR lpRemoteName;
  BOOL callOk;
} RESOURCE_INFO;

typedef struct {
  RESOURCE_INFO *buf;
  long size;
} RESOURCE_OUT_BUF;

BOOL WINAPI EnumerateResource(RESOURCE_OUT_BUF* out, LPNETRESOURCE lpnr)
{
  DWORD dwResult, dwResultEnum;
  HANDLE hEnum;
  DWORD cbBuffer = 16384;
  DWORD cEntries = -1;
   // Искать все объекты
  LPNETRESOURCE lpnrLocal;
  DWORD i;
  // Вызов функции WNetOpenEnum для начала перечисления компьютеров.
  dwResult = WNetOpenEnum(RESOURCE_GLOBALNET, // all network
                          RESOURCETYPE_ANY, // all resources
                          0, // enumerate all resources
                          lpnr, // NULL first time the function is called
                          &hEnum); // handle to the resourceдескриптор ресурса
  if (dwResult != NO_ERROR) {
    return FALSE;
  }
  // Вызвов функции GlobalAlloc для выделения ресурсов.
  lpnrLocal = (LPNETRESOURCE) GlobalAlloc(GPTR, cbBuffer);
  if (lpnrLocal == NULL) {
    return FALSE;
  }
  do
  {
    ZeroMemory(lpnrLocal, cbBuffer);
    dwResultEnum = WNetEnumResource(hEnum,
                                    &cEntries,
                                    lpnrLocal,
                                    &cbBuffer);
    if (dwResultEnum == NO_ERROR)
    {
      for(i = 0; i < cEntries; i++)
      {
        out->buf[out->size].dwScope = lpnrLocal[i].dwScope;
        out->buf[out->size].dwType = lpnrLocal[i].dwType;
        out->buf[out->size].dwDisplayType = lpnrLocal[i].dwDisplayType;
        out->buf[out->size].lpRemoteName = malloc(sizeof(char) * strlen(lpnrLocal[i].lpRemoteName));
        out->buf[out->size].callOk = TRUE;
        strcpy(out->buf[out->size].lpRemoteName, lpnrLocal[i].lpRemoteName);
        out->size++;

        if(RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONTAINER))
          out->buf[out->size].callOk = EnumerateResource(out, &lpnrLocal[i]);
      }
    }
    else if (dwResultEnum != ERROR_NO_MORE_ITEMS)
    {
      break;
    }
  } while(dwResultEnum != ERROR_NO_MORE_ITEMS);
  dwResult = WNetCloseEnum(hEnum);
  if(dwResult != NO_ERROR) {
    return FALSE;
  }
  return TRUE;
};

const char *display_types[] = {"RDT_GENERIC", "RDT_DOMAIN", "RDT_SERVER",
                               "RDT_SHARE", "RDT_FILE", "RDT_GROUP",
                               "RDT_NETWORK", "RDT_ROOT", "RDT_SHAREADMIN",
                               "RDT_DIRECTORY", "RDT_TREE", "RDT_NDSCONTAINER"};

int main()
{
  RESOURCE_OUT_BUF resources;
  resources.buf = malloc(sizeof(NETRESOURCE) * 10240);
  resources.size = 0;
  BOOL res = EnumerateResource(&resources, NULL);
  for(int i = 0; i < resources.size; ++i) {
    RESOURCE_INFO r = resources.buf[i];
    switch(r.dwScope) {
      case RESOURCE_CONNECTED:
        printf("R_CONNECTED\t");
        break;
      case RESOURCE_GLOBALNET:
        printf("R_GLOBALNET\t");
        break;
      case RESOURCE_REMEMBERED:
        printf("R_REMEMBERED\t");
        break;
      default:
        printf("R_UNKNOWN\t");
        break;
    }

    switch(r.dwType) {
      case RESOURCETYPE_ANY:
        printf("RT_ANY\t");
        break;
      case RESOURCETYPE_DISK:
        printf("RT_DISK\t");
        break;
      case RESOURCETYPE_PRINT:
        printf("RT_PRINT\t");
        break;
      default:
        printf("RT_UNKNOWN\t");
        break;
    }

    printf("%s\t", display_types[r.dwDisplayType]);

    printf("%s\t", r.lpRemoteName);
    if (!r.callOk)
      printf("ERROR CALLING WNet Services");
    printf("\n");
  }
  return 0;
}
