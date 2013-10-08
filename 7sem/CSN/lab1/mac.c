#include <windows.h>
#include <stdio.h>

typedef struct _ASTAT_
{
  ADAPTER_STATUS adapt;
  NAME_BUFFER NameBuff [30];
} ASTAT, * PASTAT;
ASTAT Adapter;

// Функция получения MAC адреса.
// На вход получает указатель на буфер, куда записывается строковое
// представление полученного MAC адреса.
BOOL GetMacAddress(char *buffer)
{
  NCB ncb;
  UCHAR uRetCode;
  char NetName[50];
  memset( &ncb, 0, sizeof(ncb) );
  ncb.ncb_command = NCBRESET;
  ncb.ncb_lana_num = 0;
  uRetCode = Netbios( &ncb );
  memset( &ncb, 0, sizeof(ncb) );
  ncb.ncb_command = NCBASTAT;
  ncb.ncb_lana_num = 0;
  strcpy( (char *) ncb.ncb_callname, "* " );
  ncb.ncb_buffer = (unsigned char *) &Adapter;
  ncb.ncb_length = sizeof(Adapter);
  uRetCode = Netbios( &ncb );
  if ( uRetCode == 0 )
  {
    sprintf(buffer, "%02X-%02X-%02X-%02X-%02X-%02X\n",
      Adapter.adapt.adapter_address[0],
      Adapter.adapt.adapter_address[1],
      Adapter.adapt.adapter_address[2],
      Adapter.adapt.adapter_address[3],
      Adapter.adapt.adapter_address[4],
      Adapter.adapt.adapter_address[5] );
    return TRUE;
  }
  return FALSE;
}

int main()
{
  char buf[1024];
  GetMacAddress(buf);
  printf("%s", buf);
  return 0;
}
