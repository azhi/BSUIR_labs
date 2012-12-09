#ifndef _FIGURE_H_
#define _FIGURE_H_

#include <windows.h>
#include <tchar.h>
#include <vector>

#define LINE 1
#define RECTANGLE 2
#define ELLIPSE 3
#define PEN 4
#define POLYLINE 5
#define POLYGON 6
#define MTEXT 7
#define MOVE 8
#define CLIP 9

class Figure
{
public:
	Figure(int type, POINT begin, LPCSTR text);
  Figure(int type, std::vector<POINT>& points);
	~Figure();

	int type;
  std::vector<POINT> points;
	TCHAR text[256];

private:
  void init(int type, LPCSTR text);
};

#endif // _FIGURE_H_
