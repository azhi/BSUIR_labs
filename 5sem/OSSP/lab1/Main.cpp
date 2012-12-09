#include <windows.h>
//#include <stdlib.h>
//#include <string.h>
#include <tchar.h>
#include <stdio.h>
#include "ViewController.h"
#include "Creator.h"
#include <zmouse.h>
#include "resources.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

ViewController* viewController;
Creator* mainCreator;

HINSTANCE hInst;

int CreatingShapeType = 0;

int RegisterWindowsClass(HINSTANCE hInstance, LRESULT (WINAPI *WindowProc)(HWND, UINT, WPARAM, LPARAM), LPCSTR ClassName)
{
  WNDCLASSEX wcex;

  wcex.cbSize         = sizeof(WNDCLASSEX);
  wcex.style          = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc    = WindowProc;
  wcex.cbClsExtra     = 0;
  wcex.cbWndExtra     = 0;
  wcex.hInstance      = hInstance;
  wcex.hIcon          = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 0, 0, LR_SHARED);
  wcex.hCursor        = (HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
  wcex.hbrBackground  = (HBRUSH) (COLOR_WINDOW+1);
  wcex.lpszMenuName   = MAKEINTRESOURCE(IDR_MAINMENU);
  wcex.lpszClassName  = ClassName;
  wcex.hIconSm        = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, LR_SHARED);

  if (!RegisterClassEx(&wcex))
  {
    MessageBox(NULL,
        _T("Call to RegisterClassEx failed!"),
        _T("Win32 Guided Tour"),
        NULL);

    return 1;
  }

  return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevhInstance, LPSTR lpCmdLime, int nCmdShow)
{

	LPCSTR ClassName = _T("MainWindowClass");
	LPCSTR WindowTitle = _T("Hello");

	hInst = hInstance;

	if (RegisterWindowsClass(hInstance, WndProc, ClassName))
	{
		return 1;
	}

	HWND hWnd = CreateWindow(
		ClassName,
		WindowTitle,
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	ShowWindow(hWnd, nCmdShow);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
  HPEN hPen;
  HBRUSH hBrush;
	BOOL success;

  static BOOL enteringText = FALSE;
	static TCHAR text[256];
	static DWORD textLength = 0;

	static OPENFILENAME OpenFileName;
	static TCHAR FilePath[256];

  static PRINTDLG printDialog;
  ZeroMemory(&printDialog, sizeof(printDialog));
  printDialog.lStructSize = sizeof(printDialog);
  printDialog.hwndOwner = hWnd;
  printDialog.Flags = PD_RETURNDC;

  static CHOOSECOLOR chooseColorStructure;
  static COLORREF acrCustClr[16];
  ZeroMemory(&chooseColorStructure, sizeof(chooseColorStructure));
  chooseColorStructure.lStructSize = sizeof(chooseColorStructure);
  chooseColorStructure.hwndOwner = hWnd;
  chooseColorStructure.lpCustColors = (LPDWORD) acrCustClr;
  chooseColorStructure.rgbResult = 0;
  chooseColorStructure.Flags = CC_FULLOPEN | CC_RGBINIT;

  static DWORD lastX, lastY;

	switch (Message)
	{
	case WM_CREATE:
		viewController = new ViewController();
		viewController->SetScrollSettings(hWnd, CW_USEDEFAULT, CW_USEDEFAULT);
		mainCreator = new Creator();

		OpenFileName.lStructSize = sizeof(OPENFILENAME);
		OpenFileName.hwndOwner = hWnd;
		OpenFileName.lpstrFile = FilePath;
		OpenFileName.nMaxFile = sizeof(FilePath);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_OPEN:
			FilePath[0] = '\0';
			success = GetOpenFileName(&OpenFileName);
			if (success)
			{
				viewController->OpenMetaFile(OpenFileName.lpstrFile);
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_SAVEAS:
			FilePath[0] = '\0';
			success = GetSaveFileName(&OpenFileName);
			if (success)
			{
				viewController->SaveMetaFile(OpenFileName.lpstrFile);
			}
			break;
		case ID_NEW:
			delete viewController;
			viewController = new ViewController();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
    case ID_FILE_EXIT:
      DestroyWindow(hWnd);
      break;
    case ID_PEN_COLOR:
      success = ChooseColor(&chooseColorStructure);
      if (success)
      {
        hPen = CreatePen(PS_SOLID, 1, chooseColorStructure.rgbResult);
        viewController->setPen(hPen);
      }
      break;
    case ID_BRUSH_COLOR:
      success = ChooseColor(&chooseColorStructure);
      if (success)
      {
        hBrush = CreateSolidBrush(chooseColorStructure.rgbResult);
        viewController->setBrush(hBrush);
      }
      break;
    case ID_PRINT:
      {
        PrintDlg(&printDialog);
        HDC printerhDC = printDialog.hDC;
        DOCINFO di = { sizeof (DOCINFO), _T("Printing...")};
        StartDoc(printerhDC, &di);
        StartPage(printerhDC);
        viewController->Print(printerhDC);
        EndPage(printerhDC);
        EndDoc(printerhDC);
        DeleteDC(printerhDC);
      }
      break;
    case ID_SET_CLIP:
      CreatingShapeType = -CLIP;
      break;
		}
		break;
	case WM_SIZE:
		viewController->SetScrollSettings(hWnd, HIWORD(lParam), LOWORD(lParam));
		break;
	case WM_VSCROLL:
	{
		int Move;
		bool AbsolutPosition = false;
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			Move = -50;
			break;

		case SB_LINEDOWN:
			Move = 50;
			break;
			
		case SB_THUMBTRACK:
			Move = HIWORD(wParam);
			AbsolutPosition = true;
			break;

		default:
			Move = 0;
		}
		viewController->UpdateVerticalScroll(hWnd, Move, AbsolutPosition);
		break;
	}
	case WM_HSCROLL:
	{
		int Move;
		bool AbsolutPosition = false;
		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			Move = -20;
			break;

		case SB_LINERIGHT:
			Move = 20;
			break;

		case SB_THUMBTRACK:
			Move = HIWORD(wParam);
			AbsolutPosition = true;
			break;

		default:
			Move = 0;
		}
		viewController->UpdateHorizontalScroll(hWnd, Move, AbsolutPosition);
		break;
	}
	case WM_LBUTTONDOWN:
    lastX = LOWORD(lParam);
    lastY = HIWORD(lParam);
    if (CreatingShapeType == -MTEXT)
    {
			CreatingShapeType = -CreatingShapeType;
      mainCreator->BeginCreatingShape(CreatingShapeType, lastX, lastY);
      textLength = 0;
      enteringText = TRUE;
    }
    else if (CreatingShapeType == MTEXT)
    {
      text[textLength++] = '\0';
      Figure tex = mainCreator->CreateShape(text);
			viewController->DrawToMetaFile(tex);
			CreatingShapeType = -CreatingShapeType;
      enteringText = FALSE;
    }
    else if (CreatingShapeType < 0)
		{
			CreatingShapeType = -CreatingShapeType;
      if (CreatingShapeType != MOVE)
        mainCreator->BeginCreatingShape(CreatingShapeType, lastX, lastY);
		}
    else if (CreatingShapeType > 0)
    {
      mainCreator->AddPoint(lastX, lastY);
			hdc = GetDC(hWnd);
			RECT r;
			GetClientRect(hWnd, &r);
			FillRect(hdc, &r, NULL);
			viewController->UpdateScreen(hdc);
      SelectObject(hdc, viewController->getPen());
      SelectObject(hdc, viewController->getBrush());
			mainCreator->DrawShape(hdc, 0, 0, NULL, 0);
			ReleaseDC(hWnd, hdc);
    }
		break;
	case WM_LBUTTONUP:
    if (CreatingShapeType > 0 && CreatingShapeType != MOVE && CreatingShapeType != MTEXT &&
        CreatingShapeType != POLYLINE && CreatingShapeType != POLYGON)
		{
      mainCreator->AddPoint(LOWORD(lParam), HIWORD(lParam));
      if (CreatingShapeType == CLIP)
        {
          viewController->SetPrintClip(mainCreator->points[0].x, mainCreator->points[0].y,
              mainCreator->points[1].x, mainCreator->points[1].y);
          hdc = GetDC(hWnd);
          RECT r;
          GetClientRect(hWnd, &r);
          FillRect(hdc, &r, NULL);
          viewController->UpdateScreen(hdc);
          ReleaseDC(hWnd, hdc);
        }
      else
        viewController->DrawToMetaFile(mainCreator->CreateShape());
		}
    if (CreatingShapeType != MTEXT && CreatingShapeType != POLYLINE && CreatingShapeType != POLYGON)
      CreatingShapeType = -CreatingShapeType;
    UpdateWindow(hWnd);
		break;
	case WM_RBUTTONDOWN:
    lastX = LOWORD(lParam);
    lastY = HIWORD(lParam);
    if (CreatingShapeType > 0)
    {
      viewController->DrawToMetaFile(mainCreator->CreateShape());
    }
	case WM_MOUSEMOVE:
    if (CreatingShapeType == MOVE)
    {
      DWORD dx = LOWORD(lParam) - lastX;
      DWORD dy = HIWORD(lParam) - lastY;
      lastX = LOWORD(lParam);
      lastY = HIWORD(lParam);
      viewController->UpdateVerticalScroll(hWnd, -dy, FALSE);
      viewController->UpdateHorizontalScroll(hWnd, -dx, FALSE);
    }
    else if (CreatingShapeType == PEN)
    {
      mainCreator->AddPoint(LOWORD(lParam), HIWORD(lParam));
			hdc = GetDC(hWnd);
			RECT r;
			GetClientRect(hWnd, &r);
			FillRect(hdc, &r, NULL);
			viewController->UpdateScreen(hdc);
      SelectObject(hdc, viewController->getPen());
      SelectObject(hdc, viewController->getBrush());
			mainCreator->DrawShape(hdc, 0, 0, NULL, 0);
			ReleaseDC(hWnd, hdc);
    }
    else if (CreatingShapeType > 0)
		{
			hdc = GetDC(hWnd);
			RECT r;
			GetClientRect(hWnd, &r);
			FillRect(hdc, &r, NULL);
			viewController->UpdateScreen(hdc);
      if (CreatingShapeType == CLIP)
      {
        SelectObject(hdc, GetStockObject(BLACK_PEN));
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
      }
      else
      {
        SelectObject(hdc, viewController->getPen());
        SelectObject(hdc, viewController->getBrush());
      }
			mainCreator->DrawShape(hdc, LOWORD(lParam), HIWORD(lParam), text, textLength);
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_MOUSEWHEEL:
		if (LOWORD(wParam) & MK_CONTROL)
		{
			viewController->ChangeScale(((short)HIWORD(wParam)) / ((float)(WHEEL_DELTA * 4)));
			RECT r;
			GetClientRect(hWnd, &r);
			viewController->SetScrollSettings(hWnd, r.bottom -  r.top, r.right - r.left);
		}
		else if (LOWORD(wParam) & MK_SHIFT)
		{
			int Move;
			Move = -((short)HIWORD(wParam));
			viewController->UpdateHorizontalScroll(hWnd, Move);
		}
		else
		{
			int Move;
			Move = -((short)HIWORD(wParam));
			viewController->UpdateVerticalScroll(hWnd, Move);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
  case WM_CHAR:
    if (enteringText)
    {
      text[textLength++] = wParam;
			hdc = GetDC(hWnd);
			RECT r;
			GetClientRect(hWnd, &r);
			FillRect(hdc, &r, NULL);
			viewController->UpdateScreen(hdc);
      SelectObject(hdc, viewController->getPen());
      SelectObject(hdc, viewController->getBrush());
			mainCreator->DrawShape(hdc, 0, 0, text, textLength);
			ReleaseDC(hWnd, hdc);
    }
    break;
	case WM_KEYDOWN:
    if (!enteringText)
      switch (wParam)
      {
        case 69: // E
          CreatingShapeType = -ELLIPSE;
          break;
        case 76: // L
          CreatingShapeType = -LINE;
          break;
        case 82: // R
          CreatingShapeType = -RECTANGLE;
          break;
        case 84: // T
          CreatingShapeType = -MTEXT;
          break;
        case 77: // M
          CreatingShapeType = -MOVE;
          break;
        case 73: // I
          CreatingShapeType = -POLYLINE;
          break;
        case 71: // G
          CreatingShapeType = -POLYGON;
          break;
        case 80: // P
          CreatingShapeType = -PEN;
          break;
      }
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		viewController->UpdateScreen(hdc);
		EndPaint(hWnd, &ps);
    break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return 0;
}
