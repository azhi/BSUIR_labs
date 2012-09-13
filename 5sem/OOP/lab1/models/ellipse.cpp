#include "ellipse.h"

#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL.h>

Ellipse::Ellipse() : Figure(), rx(0), ry(0)
{
  center = Point();
}

Ellipse::Ellipse(int x, int y, int rx, int ry) : Figure(), rx(rx), ry(ry)
{
  center = Point(x, y);
}

void Ellipse::draw()
{
  ellipseRGBA(surface, center.x, center.y, rx, ry, 0, 0, 0, 255);
}
