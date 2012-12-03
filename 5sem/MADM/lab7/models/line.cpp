#include "line.h"

#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL.h>

Line::Line(int x1, int y1, int x2, int y2) : Figure(T_LINE)
{
  p1 = Point(x1, y1);
  p2 = Point(x2, y2);
}

void Line::draw()
{
  lineRGBA(surface, p1.x, p1.y, p2.x, p2.y, 0, 0, 0, 255);
}

Rect_range Line::get_rect_space()
{
  Rect_range res;
  Range xr( min(p1.x, p2.x), max(p1.x, p2.x) );
  res.xrange = xr;
  Range yr( min(p1.y, p2.y), max(p1.y, p2.y) );
  res.yrange = yr;
  return res;
}
