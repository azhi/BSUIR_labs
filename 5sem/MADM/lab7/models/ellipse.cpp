#include "ellipse.h"

#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL.h>

Ellipse::Ellipse(int x, int y, int rx, int ry) : Figure(T_ELLIPSE), rx(rx), ry(ry)
{
  center = Point(x, y);
}

void Ellipse::draw()
{
  ellipseRGBA(surface, center.x, center.y, rx, ry, 0, 0, 0, 255);
}

Rect_range Ellipse::get_rect_space()
{
  Rect_range res;
  res.xrange = Range( center.x - rx, center.x + rx );
  res.yrange = Range( center.y - ry, center.y + ry );
  return res;
}
