#include "rectangle.h"

#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL.h>

Rectangle::Rectangle(int x1, int y1, int x2, int y2) : Figure(FG_RECTANGLE)
{
  p1 = Point(x1, y1);
  p2 = Point(x2, y2);
}

void Rectangle::draw()
{
  rectangleRGBA(surface, p1.x, p1.y, p2.x, p2.y, 0, 0, 0, 255);
}
