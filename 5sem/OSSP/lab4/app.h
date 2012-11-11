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

  HWND hwnd;
  DB_Wrapper db_w;
};

#endif /* _APP_H_ */
