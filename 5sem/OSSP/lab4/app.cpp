#include <stdio.h>
#include <tchar.h>

#include "app.h"

#include "db_funs.h"

enum {
  C_PHONE_NO,
  C_NAME,
  C_ADDRESS,
  C_FIND_BY_NAME,
  C_FIND_BY_ADDRESS
};

App::App(HINSTANCE hInst, LPCTSTR szAppTitle)
{
  //_tprintf(TEXT("%s 0x%0.8x\n"), szAppTitle, hInst);
  WNDCLASS wc = { style         : CS_HREDRAW | CS_VREDRAW,
                  lpfnWndProc   : this->WndProc,
                  cbClsExtra    : 0,
                  cbWndExtra    : 0,
                  hInstance     : hInst,
                  hIcon         : LoadIcon(NULL, IDI_WINLOGO),
                  hCursor       : LoadCursor(NULL, IDC_ARROW),
                  hbrBackground : (HBRUSH)(COLOR_WINDOW),
                  lpszMenuName  : NULL,
                  lpszClassName : szAppTitle };

  if (!RegisterClass(&wc))
    throw;

  hwnd = CreateWindow(szAppTitle, szAppTitle, WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
                      NULL, NULL, hInst, NULL);

  if (!hwnd)
    throw;

  db_w = DB_Wrapper(TEXT("DB.dll"));

  //printf("Hello!\n");
}

void App::run(int nCmdShow)
{
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

App::~App()
{
}

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT msg,
			      WPARAM wParam, LPARAM lParam)
{
  HDC hdc;

  switch(msg) {
  case WM_CREATE:
    CreateWindow(TEXT("edit"), TEXT(""),
		 WS_VISIBLE | WS_CHILD | WS_BORDER,
		 10, 10, 70, 24,
		 hwnd, (HMENU) C_PHONE_NO, NULL, NULL);

    CreateWindow(TEXT("edit"), TEXT(""),
		 WS_VISIBLE | WS_CHILD | WS_BORDER,
		 10, 35, 70, 24,
		 hwnd, (HMENU) C_NAME, NULL, NULL);

    CreateWindow(TEXT("edit"), TEXT(""),
		 WS_VISIBLE | WS_CHILD | WS_BORDER,
		 10, 60, 70, 24,
		 hwnd, (HMENU) C_ADDRESS, NULL, NULL);


    CreateWindow(TEXT("button"), TEXT("Find by phone no"),
		 WS_VISIBLE | WS_CHILD,
		 90, 10, 250, 24,
		 hwnd, (HMENU) C_FIND_BY_NAME, NULL, NULL);

    CreateWindow(TEXT("button"), TEXT("Find by address"),
		 WS_VISIBLE | WS_CHILD,
		 90, 60, 250, 24,
		 hwnd, (HMENU) C_FIND_BY_ADDRESS, NULL, NULL);
    break;

  case WM_COMMAND:
    switch(LOWORD(wParam)) {
    case C_FIND_BY_NAME:
      break;
    case C_FIND_BY_ADDRESS:
      break;
    }

  case WM_PAINT:
    // hdc = GetDC(hwnd);
    // TextOut(hdc, 10, 10, TEXT("HI!"), 3);
    // ReleaseDC(hwnd, hdc);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  }

  return DefWindowProcW(hwnd, msg, wParam, lParam);
}
