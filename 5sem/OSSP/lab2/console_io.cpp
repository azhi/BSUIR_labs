#include "console_io.h"

void ConsoleIO::cprint(TCHAR* msg)
{
  HANDLE hStdout;
  TCHAR msgBuf[BUF_SIZE];
  size_t cchStringSize;
  DWORD dwChars;
  hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  if( hStdout == INVALID_HANDLE_VALUE )
    return;
  StringCchPrintf(msgBuf, BUF_SIZE, TEXT(msg)); 
  StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
  WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);
}

TCHAR* ConsoleIO::cgets()
{
  HANDLE hStdin;
  TCHAR* msgBuf = (TCHAR*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(TCHAR) * BUF_SIZE);
  size_t cchStringSize;
  DWORD dwChars;
  hStdin = GetStdHandle(STD_INPUT_HANDLE);
  if( hStdin == INVALID_HANDLE_VALUE )
    return NULL;
  ReadConsole(hStdin, msgBuf, (DWORD) BUF_SIZE, &dwChars, NULL);
  return msgBuf;
}
