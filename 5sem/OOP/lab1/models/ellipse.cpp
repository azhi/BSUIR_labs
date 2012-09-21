#include "ellipse.h"

Ellipse::Ellipse() : Figure(), rx(0), ry(0)
{
  center = Point();
}

Ellipse::Ellipse(vector<Point>* points) : Figure()
{
  center = (*points)[0];
  rx = abs(center.x - (*points)[1].x);
  ry = abs(center.y - (*points)[1].y);
}

void Ellipse::draw()
{
  ellipseRGBA(surface, center.x, center.y, rx, ry, 0, 0, 0, 255);
}

void Ellipse::move_point2(Point p)
{
  rx = abs(center.x - p.x);
  ry = abs(center.y - p.y);
}
