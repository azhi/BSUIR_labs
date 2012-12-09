#ifndef _CREATOR_H_
#define _CREATOR_H_

#include <windows.h>
#include <vector>
#include "Figure.h"

class Creator
{
public:
	Creator();
	~Creator();

	void BeginCreatingShape(int type, int x, int y);
  void AddPoint(int x, int y);
  void DrawShape(HDC hdc, int x, int y, LPCSTR text, DWORD textLength);
	Figure CreateShape();
  Figure CreateShape(LPCSTR text);

	int currentCreatingShapeType;
  std::vector<POINT> points;
};

#endif // _CREATOR_H_
