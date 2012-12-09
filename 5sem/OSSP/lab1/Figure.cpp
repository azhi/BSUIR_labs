#include "Figure.h"
#include <windows.h>
#include <stdio.h>

Figure::Figure(int type, POINT begin, LPCSTR text)
{
  points.push_back(begin);
  init(type, text);
}

Figure::Figure(int type, std::vector<POINT>& points)
{
  this->points.insert(this->points.begin(), points.begin(), points.end());
  init(type, NULL);
}

Figure::~Figure()
{
}

void Figure::init(int type, LPCSTR text)
{
  this->type = type;
  if (text)
    _tcscpy(this->text, text);
}
