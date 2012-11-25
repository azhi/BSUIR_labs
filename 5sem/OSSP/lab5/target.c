#include <windows.h>

int main(int argc, char **argv)
{
  HANDLE file = CreateFileA("F1", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL, NULL);
  file = CreateFileW(L"F2", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                     FILE_ATTRIBUTE_NORMAL, NULL);
  return 0;
}
