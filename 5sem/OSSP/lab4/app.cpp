#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <commctrl.h>

#include "app.h"

enum {
  C_PHONE_NO,
  C_NAME,
  C_ADDRESS,
  C_FIND,
  C_INSERT,
  C_STATUSBAR,
  C_LIST,
  C_DELETE
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

  hwnd = CreateWindow(szAppTitle, szAppTitle,
                      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
                      WS_MINIMIZEBOX,
                      CW_USEDEFAULT, CW_USEDEFAULT, 360, 420,
                      NULL, NULL, hInst, NULL);

  if (!hwnd)
    throw;


  INITCOMMONCONTROLSEX icc = { dwSize : sizeof(icc),
                               dwICC  : ICC_STANDARD_CLASSES };
  InitCommonControlsEx(&icc);

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

void App::findAbonent(Abonent *abonent)
{
  BOOL bCr = abonent == NULL;
  if (!abonent) {
    abonent = loadAbonent();
    printf("Abonent loaded.\n");
  }

  updateStatus(TEXT("Searching..."), TRUE);
  DWORD *ids = (DWORD *) malloc(sizeof(DWORD) * 256);
  DWORD count = 0;//db_w.find_abonents(ids, 256, abonent);
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
  if (bCr)
    clear_abonent(abonent);
}

#define LOAD(c_id, where) {                    \
    HWND hwndChld = GetDlgItem(hwnd, c_id);    \
    GetWindowText(hwndChld, ab->where, 256);   \
  }

Abonent *App::loadAbonent()
{
  Abonent *ab = create_abonent();
  LOAD(C_PHONE_NO, phone_no);
  LOAD(C_NAME, name);
  LOAD(C_ADDRESS, street);
  return ab;
}

#undef LOAD

void App::insertAbonent(Abonent *abonent)
{
  BOOL bCr = abonent == NULL;
  if (!abonent)
    abonent = loadAbonent();

  DWORD id = 0;//db_w.insert_abonent(abonent);

  if (id == -1)
    throw;

  LPTSTR status = (LPTSTR) malloc(sizeof(TCHAR) * 256);
  _stprintf(status, TEXT("Record with id %d created."), id);
  updateStatus(status);
  free(status);

  if(bCr)
    clear_abonent(abonent);
}

void App::updateStatus(LPCTSTR status, BOOL in_progress)
{
  this->in_progress = in_progress;
  HWND hwndStatus = GetDlgItem(hwnd, C_STATUSBAR);
  SetWindowText(hwndStatus, status);
}

#define INS_CLMN(_i, _str, _cx)                 \
  lvC.pszText = const_cast<LPTSTR>(TEXT(_str)); \
  lvC.cx = _cx;                                 \
  lvC.iSubItem = _i;                            \
  ListView_InsertColumn(hwndList, _i, &lvC);

void App::initializeList(HWND hwndList)
{
  LV_COLUMN lvC = { mask : LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
                    fmt  : LVCFMT_LEFT };

  INS_CLMN(0, "id", 24);
  INS_CLMN(1, "Phone no.", 80);
  INS_CLMN(2, "Name", 100);
  INS_CLMN(3, "Address", 100);
}

#undef INS_CLMN

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
    ListView_SetItemText(hwndList, i, 3, ab->street);
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

  Abonent *ab;

  LPTSTR name, phone_no, address;
  HWND hwndChld;
  long len;

  switch(msg) {
  case WM_CREATE:

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


    CreateWindow(WC_BUTTON, TEXT("Find"),
                 WS_VISIBLE | WS_CHILD,
                 140, 10, 200, 24,
                 hwnd, (HMENU) C_FIND, NULL, NULL);

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
    case C_FIND:
      //ab = self->loadAbonent();
      self->findAbonent();
      //clear_abonent(ab);
      break;

    case C_INSERT:
      //ab = self->loadAbonent();
      self->insertAbonent();
      //clear_abonent(ab);
      break;
    }
    break; // WM_COMMAND

  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  }

  return DefWindowProcW(hwnd, msg, wParam, lParam);
}
