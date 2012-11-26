#include <stdio.h>
#include <tchar.h>

#include <windows.h>

#include <commctrl.h>

#include "app.h"

enum {
  C_PHONE_NO,
  C_FAMILY_NAME,
  C_NAME,
  C_MIDDLE_NAME,
  C_STREET,
  C_HOUSE,
  C_BUILDING,
  C_FLAT,

  C_PHONE_NO_LABEL,
  C_FAMILY_NAME_LABEL,
  C_NAME_LABEL,
  C_MIDDLE_NAME_LABEL,
  C_STREET_LABEL,
  C_HOUSE_LABEL,
  C_BUILDING_LABEL,
  C_FLAT_LABEL,

  C_FIND,
  C_STATUSBAR,
  C_LIST,
};

App::App(HINSTANCE hInst, LPCTSTR szAppTitle) : in_progress(FALSE),
                                                hInst(hInst)
{
  if ( db_w.last_error() )
    throw;

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
                      CW_USEDEFAULT, CW_USEDEFAULT, 600, 450,
                      NULL, NULL, hInst, NULL);

  if (!hwnd)
    throw;


  INITCOMMONCONTROLSEX icc = { dwSize : sizeof(icc),
                               dwICC  : ICC_STANDARD_CLASSES };
  InitCommonControlsEx(&icc);

  SetWindowLongPtr(hwnd, GWLP_USERDATA, (long)this);
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
  Abonent **abonents = waste_my_memory(256);
  DWORD count = db_w.find_abonents(abonents, 256, abonent);
  clear_abonent(abonent);
  if (count) {
    updateList(abonents, count < 256 ? count : 256);
    LPTSTR status = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    _stprintf(status, TEXT("%d record(s) found."), count);
    updateStatus(status);
    free(status);
  } else {
    updateList();
    updateStatus(TEXT("Sorry, no records found."));
  }
  if (bCr)
    i_want_my_memory_back1(abonents);
}

#define LOAD(c_id, where) {                    \
    HWND hwndChld = GetDlgItem(hwnd, c_id);    \
    GetWindowText(hwndChld, ab->where, 256);   \
  }

Abonent *App::loadAbonent()
{
  Abonent *ab = create_abonent();
  LOAD(C_PHONE_NO, phone_no);
  LOAD(C_FAMILY_NAME, family_name);
  LOAD(C_NAME, name);
  LOAD(C_MIDDLE_NAME, middle_name);
  LOAD(C_STREET, street);
  LOAD(C_HOUSE, house);
  LOAD(C_BUILDING, building);
  LOAD(C_FLAT, flat);
  return ab;
}

#undef LOAD

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
  INS_CLMN(1, "Phone no.",    60);
  INS_CLMN(2, "Family name",  90);
  INS_CLMN(3, "Name",        110);
  INS_CLMN(4, "Middle name", 100);
  INS_CLMN(5, "Street",       90);
  INS_CLMN(6, "House",        30);
  INS_CLMN(7, "Building",     30);
  INS_CLMN(8, "Flat",         30);
}

#undef INS_CLMN

#define UPD_CLMN(_i, _what)                                     \
  ListView_SetItemText(hwndList, i, _i, abonents[i]->_what);


void App::updateList(Abonent **abonents, DWORD num)
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
                  lParam     : 0 };

  ListView_DeleteAllItems(hwndList);
  for (DWORD i = 0; i < num; ++i) {
    lvI.pszText = (LPTSTR) malloc(sizeof(TCHAR) * 256);
    lvI.iItem = i;
    _stprintf(lvI.pszText, TEXT("%d"), abonents[i]->id);
    ListView_InsertItem(hwndList, &lvI);
    free(lvI.pszText);


    UPD_CLMN(1, phone_no);
    UPD_CLMN(2, family_name);
    UPD_CLMN(3, name);
    UPD_CLMN(4, middle_name);
    UPD_CLMN(5, street);
    UPD_CLMN(6, house);
    UPD_CLMN(7, building);
    UPD_CLMN(8, flat);
  }

  clear_abonent(ab);
}

#undef UPD_CLMN

App::~App()
{
}

#define input_with_lbl(id, desc, y, x)                                  \
  CreateWindow(WC_STATIC, TEXT(desc), WS_VISIBLE | WS_CHILD,            \
               x, y, 90, 24, hwnd, (HMENU) id##_LABEL, NULL, NULL);     \
  CreateWindow(WC_EDIT, TEXT(""), WS_VISIBLE | WS_CHILD | WS_BORDER,    \
               x + 100, y, 120, 24, hwnd, (HMENU) id, NULL, NULL);

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

    input_with_lbl(C_PHONE_NO,    "Phone no.:",    10,  10);
    input_with_lbl(C_FAMILY_NAME, "Family name:",  35,  10);
    input_with_lbl(C_MIDDLE_NAME, "Middle name:",  60,  10);
    input_with_lbl(C_NAME,        "Name:",         85,  10);

    input_with_lbl(C_STREET,      "Street:",       10, 240);
    input_with_lbl(C_HOUSE,       "House:",        35, 240);
    input_with_lbl(C_BUILDING,    "Building:",     60, 240);
    input_with_lbl(C_FLAT,        "Flat:",         85, 240);


    CreateWindow(WC_BUTTON, TEXT("Find"),
                 WS_VISIBLE | WS_CHILD,
                 480, 90, 100, 24,
                 hwnd, (HMENU) C_FIND, NULL, NULL);

    hwndChld = CreateWindow(WC_LISTVIEW, TEXT(""),
                            WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
                            10, 120, 570, 270,
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
      self->findAbonent();
      break;
    }
    break; // WM_COMMAND

  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  }

  return DefWindowProcW(hwnd, msg, wParam, lParam);
}
