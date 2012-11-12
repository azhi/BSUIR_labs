#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <commctrl.h>

#include "app.h"

enum {
  C_PHONE_NO,
  C_NAME,
  C_ADDRESS,
  C_FIND_BY_PHONE_NO,
  C_FIND_BY_NAME,
  C_FIND_BY_ADDRESS,
  C_INSERT,
  C_STATUSBAR,
  C_LIST,
  C_DELETE_1 = 0x0100
};

App::App(HINSTANCE hInst, LPCTSTR szAppTitle) : in_progress(FALSE),
                                                hInst(hInst)
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
                      CW_USEDEFAULT, CW_USEDEFAULT, 360, 420,
                      NULL, NULL, hInst, NULL);

  if (!hwnd)
    throw;

  SetWindowLongPtr(hwnd, GWLP_USERDATA, (long)this);

  db_w = DB_Wrapper(TEXT("DB.dll"));
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

void App::findByPhoneNo(LPCTSTR phone_no)
{
  updateStatus(TEXT("Searching..."));
  in_progress = TRUE;
  DWORD *ids = (DWORD *) malloc(sizeof(DWORD) * 256);
  _tprintf(phone_no);
  DWORD count = db_w.find_by_phone_no(ids, 256, phone_no);
  if (count) {
    updateList(ids, count < 256 ? count : 256);
    LPTSTR status = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    _stprintf(status, TEXT("%d record(s) found."), count);
    updateStatus(status);
    free(status);
  } else {
    updateList();
    updateStatus(TEXT("Sorry, no records found."));
  }
  free(ids);
}

void App::findByName(LPCTSTR name)
{
  updateStatus(TEXT("Searching..."));
  in_progress = TRUE;
  DWORD *ids = (DWORD *) malloc(sizeof(DWORD) * 256);
  DWORD count = db_w.find_by_name(ids, 256, name);
  if (count) {
    updateList(ids, count < 256 ? count : 256);
    LPTSTR status = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    _stprintf(status, TEXT("%d record(s) found."), count);
    updateStatus(status);
    free(status);
  } else {
    updateList();
    updateStatus(TEXT("Sorry, no records found."));
  }
  free(ids);
}

void App::findByAddress(LPCTSTR address)
{
  updateStatus(TEXT("Searching..."));
  in_progress = TRUE;
  DWORD *ids = (DWORD *) malloc(sizeof(DWORD) * 256);
  DWORD count = db_w.find_by_address(ids, 256, address);
  if (count) {
    updateList(ids, count < 256 ? count : 256);
    LPTSTR status = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    _stprintf(status, TEXT("%d record(s) found."), count);
    updateStatus(status);
    free(status);
  } else {
    updateList();
    updateStatus(TEXT("Sorry, no records found."));
  }
  free(ids);
}

void App::insertAbonent(LPCTSTR phone_no, LPCTSTR name, LPCTSTR address)
{
  Abonent ab = { id       : 0,
                 name     : const_cast<LPTSTR>(name),
                 phone_no : const_cast<LPTSTR>(phone_no),
                 address  : const_cast<LPTSTR>(address) };
  DWORD id = db_w.insert_abonent(&ab);

  if (id == -1)
    throw;

  LPTSTR status = (LPTSTR) malloc(sizeof(TCHAR) * 256);
  _stprintf(status, TEXT("Record with id %d created."), id);
  updateStatus(status);
  free(status);
}

void App::updateStatus(LPCTSTR status)
{
  in_progress = FALSE;
  HWND hwndStatus = GetDlgItem(hwnd, C_STATUSBAR);
  SetWindowText(hwndStatus, status);
}

void App::initializeList(HWND hwndList)
{
  LV_COLUMN lvC = { mask : LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
                    fmt  : LVCFMT_LEFT,
                    cx   : 75 };

  lvC.pszText = const_cast<LPTSTR>(TEXT("id"));
  lvC.cx = 24;
  lvC.iSubItem = 0;
  ListView_InsertColumn(hwndList, 0, &lvC);

  lvC.cx = 80;
  lvC.pszText = const_cast<LPTSTR>(TEXT("Phone no."));
  lvC.iSubItem = 1;
  ListView_InsertColumn(hwndList, 1, &lvC);

  lvC.cx = 100;

  lvC.pszText = const_cast<LPTSTR>(TEXT("Name"));
  lvC.iSubItem = 2;
  ListView_InsertColumn(hwndList, 2, &lvC);

  lvC.pszText = const_cast<LPTSTR>(TEXT("Address"));
  lvC.iSubItem = 3;
  ListView_InsertColumn(hwndList, 3, &lvC);
}

void App::updateList(DWORD *ids, DWORD num)
{
  Abonent *ab = create_abonent();

  HWND hwndList = GetDlgItem(hwnd, C_LIST);
  LV_ITEM lvI = { mask       : LVIF_TEXT | LVIF_PARAM | LVIF_STATE,
                  iItem      : 0,
                  iSubItem   : 0,
                  state      : 0,
                  stateMask  : 0,
                  pszText    : NULL,
                  cchTextMax : 255,
                  iImage     : 0,
                  lParam     : NULL };

  ListView_DeleteAllItems(hwndList);
  for (DWORD i = 0; i < num; ++i) {
    db_w.get_by_id(ids[i], ab);
    DWORD id = ids[i];

    lvI.pszText = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    lvI.iItem = i;
    _stprintf(lvI.pszText, TEXT("%d"), ids[i]);

    ListView_InsertItem(hwndList, &lvI);
    free(lvI.pszText);

    ListView_SetItemText(hwndList, i, 1, ab->phone_no);
    ListView_SetItemText(hwndList, i, 2, ab->name);
    ListView_SetItemText(hwndList, i, 3, ab->address);
  }

  clear_abonent(ab);
}

App::~App()
{
}

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT msg,
                              WPARAM wParam, LPARAM lParam)
{
  App *self((App*)GetWindowLongPtr(hwnd, GWLP_USERDATA));
  LPTSTR name, phone_no, address;
  HWND hwndChld;
  long len;

  switch(msg) {
  case WM_CREATE:

    InitCommonControls();

    CreateWindow(WC_EDIT, TEXT(""),
                 WS_VISIBLE | WS_CHILD | WS_BORDER,
                 10, 10, 120, 24,
                 hwnd, (HMENU) C_PHONE_NO, NULL, NULL);

    CreateWindow(WC_EDIT, TEXT(""),
                 WS_VISIBLE | WS_CHILD | WS_BORDER,
                 10, 35, 120, 24,
                 hwnd, (HMENU) C_NAME, NULL, NULL);

    CreateWindow(WC_EDIT, TEXT(""),
                 WS_VISIBLE | WS_CHILD | WS_BORDER,
                 10, 60, 120, 24,
                 hwnd, (HMENU) C_ADDRESS, NULL, NULL);


    CreateWindow(WC_BUTTON, TEXT("Find by phone no"),
                 WS_VISIBLE | WS_CHILD,
                 140, 10, 200, 24,
                 hwnd, (HMENU) C_FIND_BY_PHONE_NO, NULL, NULL);

    CreateWindow(WC_BUTTON, TEXT("Find by name"),
                 WS_VISIBLE | WS_CHILD,
                 140, 35, 200, 24,
                 hwnd, (HMENU) C_FIND_BY_NAME, NULL, NULL);

    CreateWindow(WC_BUTTON, TEXT("Find by address"),
                 WS_VISIBLE | WS_CHILD,
                 140, 60, 200, 24,
                 hwnd, (HMENU) C_FIND_BY_ADDRESS, NULL, NULL);

    CreateWindow(WC_BUTTON, TEXT("Insert"),
                 WS_VISIBLE | WS_CHILD,
                 25, 90, 300, 24,
                 hwnd, (HMENU) C_INSERT, NULL, NULL);

    hwndChld = CreateWindow(WC_LISTVIEW, TEXT(""),
                            WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
                            10, 120, 330, 250,
                            hwnd, (HMENU) C_LIST, NULL, NULL);

    self->initializeList(hwndChld);

    CreateWindow(TEXT("msctls_statusbar32"), TEXT(""),
                 WS_VISIBLE | WS_CHILD,
                 0,0,0,0,
                 hwnd, (HMENU) C_STATUSBAR, NULL, NULL);
    break;

  case WM_COMMAND:
    if(self->in_progress)
      break;

    switch(LOWORD(wParam)) {
    case C_FIND_BY_PHONE_NO:
      hwndChld = GetDlgItem(hwnd, C_PHONE_NO);
      len = GetWindowTextLength(hwndChld) + 1;
      phone_no = (LPTSTR) malloc(sizeof(TCHAR) * 256);
      GetWindowText(hwndChld, phone_no, len);
      self->findByPhoneNo(phone_no);
      free(phone_no);
      break;

    case C_FIND_BY_NAME:
      hwndChld = GetDlgItem(hwnd, C_NAME);
      len = GetWindowTextLength(hwndChld) + 1;
      name = (LPTSTR) malloc(sizeof(TCHAR) * 256);
      GetWindowText(hwndChld, name, len);
      self->findByName(name);
      free(name);
      break;

    case C_FIND_BY_ADDRESS:
      hwndChld = GetDlgItem(hwnd, C_ADDRESS);
      len = GetWindowTextLength(hwndChld) + 1;
      address = (LPTSTR) malloc(sizeof(TCHAR) * 256);
      GetWindowText(hwndChld, address, len);
      self->findByAddress(address);
      free(address);
      break;

    case C_INSERT:
      hwndChld = GetDlgItem(hwnd, C_PHONE_NO);
      len = GetWindowTextLength(hwndChld) + 1;
      phone_no = (LPTSTR) malloc(sizeof(TCHAR) * 256);
      GetWindowText(hwndChld, phone_no, len);

      hwndChld = GetDlgItem(hwnd, C_NAME);
      len = GetWindowTextLength(hwndChld) + 1;
      name = (LPTSTR) malloc(sizeof(TCHAR) * 256);
      GetWindowText(hwndChld, name, len);

      hwndChld = GetDlgItem(hwnd, C_ADDRESS);
      len = GetWindowTextLength(hwndChld) + 1;
      address = (LPTSTR) malloc(sizeof(TCHAR) * 256);
      GetWindowText(hwndChld, address, len);

      self->insertAbonent(phone_no, name, address);

      free(phone_no);
      free(name);
      free(address);
      break;
    }
    break; // WM_COMMAND

  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  }

  return DefWindowProcW(hwnd, msg, wParam, lParam);
}
