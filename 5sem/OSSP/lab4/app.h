#ifndef _APP_H_
#define _APP_H_

#include <windows.h>

#include "db_wrapper.h"


class App
{
public:
  App(HINSTANCE hInst=0, LPCTSTR szAppTitle=TEXT("lab4"));
  App();
  virtual ~App();

  void run(int nCmdShow);

private:
  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
                                  WPARAM wParam, LPARAM lParam);

  void findByPhoneNo(LPCTSTR phone_no);
  void findByName(LPCTSTR name);
  void findByAddress(LPCTSTR address);
  void insertAbonent(LPCTSTR phone_no, LPCTSTR name, LPCTSTR address);

  void initializeList(HWND hwndList);
  void updateList(DWORD *ids=NULL, DWORD num=0);
  void updateStatus(LPCTSTR status);

  HINSTANCE hInst;
  HWND hwnd;
  DB_Wrapper db_w;
  BOOL in_progress;
};

#endif /* _APP_H_ */
