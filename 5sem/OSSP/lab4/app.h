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

  void findAbonent(Abonent *abonent=NULL);
  void insertAbonent(Abonent *abonent=NULL);
  Abonent *loadAbonent();

  void initializeList(HWND hwndList);
  void updateList(Abonent **ids=NULL, DWORD num=0);
  void updateStatus(LPCTSTR status, BOOL in_progress=FALSE);

  HINSTANCE hInst;
  HWND hwnd;
  DB_Wrapper db_w;
  BOOL in_progress;
};

#endif /* _APP_H_ */
