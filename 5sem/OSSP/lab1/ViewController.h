#ifndef _VIEW_CONTROLLER_H_
#define _VIEW_CONTROLLER_H_

#include <windows.h>
#include "Figure.h"

class ViewController
{
  public:
    ViewController(int height = 1000, int width = 1600);
    ~ViewController();

    void setPen(HPEN pen);
    void setBrush(HBRUSH brush);
    HPEN getPen() { return hPen; };
    HBRUSH getBrush() { return hBrush; };
    void SetScrollSettings(HWND hWnd, int WindowHeight, int WindowWidth);
    void UpdateVerticalScroll(HWND hWnd, int Move, bool Absolut = false);
    void UpdateHorizontalScroll(HWND hWnd, int Move, bool Absolut = false);
    void DrawToScreen(HDC hdc);
    void UpdateScreen(HDC hdc);
    void DrawToMetaFile(Figure DrawedFigure);
    void ChangeScale(float DeltaScale) { Scale += DeltaScale; };
    void SaveMetaFile(TCHAR* MetaFileName);
    void OpenMetaFile(TCHAR* MetaFileName);
    void Print(HDC phDC);
    void SetPrintClip(int x1, int y1, int x2, int y2);

  private:
    void CopyMetaFile();

    SCROLLINFO VerticalScrollInfo;
    SCROLLINFO HorizontalScrollInfo;

    HDC MetaFileContext;
    //HENHMETAFILE MetaFile;
    HMETAFILE MetaFile;

    HPEN hPen;
    HBRUSH hBrush;

    HRGN printClipRgn;

    int Height;
    int Width;

    float Scale;
};

#endif // _VIEW_CONTROLLER_H_
