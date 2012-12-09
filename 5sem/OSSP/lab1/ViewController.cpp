#include "ViewController.h"
#include <stdio.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))

ViewController::ViewController(int height, int width) : Height(height), Width(width)
{
  RECT Rect;
  SetRect(&Rect, 0, 0, Height, Width);
  //MetaFileContext = CreateEnhMetaFile(NULL, NULL, &Rect, NULL);
  MetaFileContext = CreateMetaFile(NULL);
  hPen = (HPEN) GetStockObject(BLACK_PEN);
  hBrush = (HBRUSH) GetStockObject(WHITE_BRUSH);
  SelectObject(MetaFileContext, hPen);
  SelectObject(MetaFileContext, hBrush);
  printClipRgn = CreateRectRgn(0, 0, width, height);
  Scale = 1;
  HorizontalScrollInfo.nPos = 0;
  VerticalScrollInfo.nPos = 0;
};

void ViewController::setPen(HPEN pen)
{
  DeleteObject(hPen);
  hPen = pen;
}

void ViewController::setBrush(HBRUSH brush)
{
  DeleteObject(hBrush);
  hBrush = brush;
}

void ViewController::CopyMetaFile()
{
	//RECT Rect;
	//SetRect(&Rect, 0, 0, Height, Width);
	//MetaFileContext = CreateEnhMetaFile(NULL, NULL, &Rect, NULL);
	//PlayEnhMetaFile(MetaFileContext, MetaFile, &Rect);
	//DeleteEnhMetaFile(MetaFile);
	MetaFileContext = CreateMetaFile(NULL);
	PlayMetaFile(MetaFileContext, MetaFile);
	DeleteMetaFile(MetaFile);
}

void ViewController::SetScrollSettings(HWND hWnd, int WindowHeight, int WindowWidth)
{
	VerticalScrollInfo.cbSize = sizeof(VerticalScrollInfo);
	VerticalScrollInfo.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	VerticalScrollInfo.nMin = 0;
	VerticalScrollInfo.nMax = Height * Scale + 0.5;
	VerticalScrollInfo.nPage = WindowHeight;

	SetScrollInfo(hWnd, SB_VERT, &VerticalScrollInfo, TRUE);

	HorizontalScrollInfo.cbSize = sizeof(HorizontalScrollInfo);
	HorizontalScrollInfo.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	HorizontalScrollInfo.nMin = 0;
	HorizontalScrollInfo.nMax = Width * Scale + 0.5;
	HorizontalScrollInfo.nPage = WindowWidth;

	SetScrollInfo(hWnd, SB_HORZ, &HorizontalScrollInfo, TRUE);
}

void ViewController::UpdateVerticalScroll(HWND hWnd, int Move, bool Absolut)
{
	if (Absolut)
	{
		Move -= VerticalScrollInfo.nPos;
	}
	Move = min(Move, Height * Scale - VerticalScrollInfo.nPage - VerticalScrollInfo.nPos);
	Move = max(Move, VerticalScrollInfo.nMin - VerticalScrollInfo.nPos);
	if (Move)
	{
		ScrollWindow(hWnd, 0, (-Move), NULL, NULL);
		VerticalScrollInfo.nPos += Move;
		SetScrollInfo(hWnd, SB_VERT, &VerticalScrollInfo, TRUE);
	}
}

void ViewController::UpdateHorizontalScroll(HWND hWnd, int Move, bool Absolut)
{
	if (Absolut)
	{
		Move -= HorizontalScrollInfo.nPos;
	}
	Move = min(Move, Width * Scale - HorizontalScrollInfo.nPage - HorizontalScrollInfo.nPos);
	Move = max(Move, HorizontalScrollInfo.nMin - HorizontalScrollInfo.nPos);
  if (Move)
	{
		ScrollWindow(hWnd, (-Move), 0, NULL, NULL);
		HorizontalScrollInfo.nPos += Move;
		SetScrollInfo(hWnd, SB_HORZ, &HorizontalScrollInfo, TRUE);
	}
}

void ViewController::DrawToScreen(HDC hdc)
{

}

void ViewController::UpdateScreen(HDC hdc)
{
	//RECT Rect;
	//SetViewportOrgEx(hdc, -HorizontalScrollInfo.nPos, -VerticalScrollInfo.nPos, NULL);
	//SetRect(&Rect, 0, 0, Height * Scale, Width * Scale);
	//PlayEnhMetaFile(hdc, MetaFile, &Rect);
	//SetViewportOrgEx(hdc, (int)(Width * Scale), (int)(Height * Scale), NULL);
	//SetViewportOrgEx(hdc, Width, Height, NULL);
	HRGN hRgn = CreateRectRgn(0, 0, Width * Scale + 1.5, Height* Scale + 1.5);
	SelectClipRgn(hdc, hRgn);
	DeleteObject(hRgn);
	SetGraphicsMode(hdc, GM_ADVANCED);
	XFORM* Convertion = new XFORM;
	Convertion->eDx = -HorizontalScrollInfo.nPos;
	Convertion->eDy = -VerticalScrollInfo.nPos;
	Convertion->eM11 = Scale;
	Convertion->eM22 = Scale;
	Convertion->eM12 = 0;
	Convertion->eM21 = 0;
	SetWorldTransform(hdc, Convertion);
	// Rectangle(hdc, 0, 0, Width, Height);
	PlayMetaFile(hdc, MetaFile);
}

void ViewController::SetPrintClip(int x1, int y1, int x2, int y2)
{
  DeleteObject(printClipRgn);
  x1 += HorizontalScrollInfo.nPos;
  x1 /= Scale;
  x2 += HorizontalScrollInfo.nPos;
  x2 /= Scale;
  y1 += VerticalScrollInfo.nPos;
  y1 /= Scale;
  y2 += VerticalScrollInfo.nPos;
  y2 /= Scale;
  printClipRgn = CreateRectRgn(x1, y1, x2, y2);
}

void ViewController::Print(HDC phDC)
{
	SelectClipRgn(phDC, printClipRgn);
	PlayMetaFile(phDC, MetaFile);
}

void ViewController::DrawToMetaFile(Figure DrawedFigure)
{
	CopyMetaFile();
  SelectObject(MetaFileContext, hPen);
  SelectObject(MetaFileContext, hBrush);
	switch (DrawedFigure.type)
	{
	case LINE:
		MoveToEx(MetaFileContext, int(((float)DrawedFigure.points[0].x + ((float)HorizontalScrollInfo.nPos)) / Scale + 0.5),
             int(((float)DrawedFigure.points[0].y + ((float)VerticalScrollInfo.nPos))/ Scale + 0.5), NULL);
		LineTo(MetaFileContext, int(((float)DrawedFigure.points[1].x + ((float)HorizontalScrollInfo.nPos)) / Scale + 0.5),
             int(((float)DrawedFigure.points[1].y + ((float)VerticalScrollInfo.nPos)) / Scale + 0.5));
		//MetaFile = CloseEnhMetaFile(MetaFileContext);
		break;
	case RECTANGLE:
		Rectangle(MetaFileContext, 
			int(((float)DrawedFigure.points[0].x + ((float)HorizontalScrollInfo.nPos)) / Scale + 0.5), 
			int(((float)DrawedFigure.points[0].y + ((float)VerticalScrollInfo.nPos))/ Scale + 0.5),
			int(((float)DrawedFigure.points[1].x + ((float)HorizontalScrollInfo.nPos)) / Scale + 0.5),
			int(((float)DrawedFigure.points[1].y + ((float)VerticalScrollInfo.nPos)) / Scale + 0.5));
		break;
	case ELLIPSE:
		Ellipse(MetaFileContext, 
			int(((float)DrawedFigure.points[0].x + ((float)HorizontalScrollInfo.nPos)) / Scale + 0.5), 
			int(((float)DrawedFigure.points[0].y + ((float)VerticalScrollInfo.nPos))/ Scale + 0.5),
			int(((float)DrawedFigure.points[1].x + ((float)HorizontalScrollInfo.nPos)) / Scale + 0.5),
			int(((float)DrawedFigure.points[1].y + ((float)VerticalScrollInfo.nPos)) / Scale + 0.5));
		break;
  case MTEXT:
    TextOut(MetaFileContext,
        int(((float)DrawedFigure.points[0].x + ((float)HorizontalScrollInfo.nPos)) / Scale + 0.5), 
        int(((float)DrawedFigure.points[0].y + ((float)VerticalScrollInfo.nPos))/ Scale + 0.5),
        DrawedFigure.text, _tcslen(DrawedFigure.text));
    break;
  case PEN:
  case POLYLINE:
    for(int i = 0; i < DrawedFigure.points.size(); ++i)
    {
      DrawedFigure.points[i].x = int(((float)DrawedFigure.points[i].x + ((float)HorizontalScrollInfo.nPos)) / Scale + 0.5); 
      DrawedFigure.points[i].y = int(((float)DrawedFigure.points[i].y + ((float)HorizontalScrollInfo.nPos)) / Scale + 0.5); 
    }
    Polyline(MetaFileContext, &DrawedFigure.points[0], DrawedFigure.points.size());
    break;
  case POLYGON:
    for(int i = 0; i < DrawedFigure.points.size(); ++i)
    {
      DrawedFigure.points[i].x = int(((float)DrawedFigure.points[i].x + ((float)HorizontalScrollInfo.nPos)) / Scale + 0.5); 
      DrawedFigure.points[i].y = int(((float)DrawedFigure.points[i].y + ((float)HorizontalScrollInfo.nPos)) / Scale + 0.5); 
    }
    Polygon(MetaFileContext, &DrawedFigure.points[0], DrawedFigure.points.size());
    break;
	}
  MetaFile = CloseMetaFile(MetaFileContext);
}

void ViewController::SaveMetaFile(TCHAR* MetaFileName)
{
	HDC SavedMetafileContext = CreateMetaFile(MetaFileName);
	PlayMetaFile(SavedMetafileContext, MetaFile);
	HMETAFILE SavedMetafile = CloseMetaFile(SavedMetafileContext);
	//DeleteMetaFile(SavedMetafile);
}

void ViewController::OpenMetaFile(TCHAR* MetaFileName)
{
	DeleteMetaFile(MetaFile);

	MetaFileContext = CreateMetaFile(NULL);
	HMETAFILE hmf = GetMetaFile(MetaFileName);
	PlayMetaFile(MetaFileContext, hmf);

	MetaFile = CloseMetaFile(MetaFileContext);
	DeleteMetaFile(hmf);
}

ViewController::~ViewController()
{
	DeleteMetaFile(MetaFile);
}
