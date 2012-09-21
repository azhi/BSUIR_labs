#include "rectangle.h"

Rectangle::Rectangle() : Figure()
{
  p1 = Point();
  p2 = Point();
}

Rectangle::Rectangle(vector<Point>* points) : Figure()
{
  p1 = (*points)[0];
  p2 = (*points)[1];
}

void Rectangle::draw()
{
  rectangleRGBA(surface, p1.x, p1.y, p2.x, p2.y, 0, 0, 0, 255);
}

void Rectangle::move_point2(Point p)
{
  p2 = p;
}
