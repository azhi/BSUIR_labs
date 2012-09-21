#include "line.h"

Line::Line() : Figure()
{
  p1 = Point();
  p2 = Point();
}

Line::Line(vector<Point>* points) : Figure()
{
  p1 = (*points)[0];
  p2 = (*points)[1];
}

void Line::draw()
{
  lineRGBA(surface, p1.x, p1.y, p2.x, p2.y, 0, 0, 0, 255);
}

void Line::move_point2(Point p)
{
  p2 = p;
}
