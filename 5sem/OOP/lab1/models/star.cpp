#include "star.h"

Star::Star() : Figure()
{
  p1 = Point();
  p2 = Point();
  p3 = Point();
  p4 = Point();
  p5 = Point();
}

Star::Star(vector<Point>* points) : Figure()
{
  center = (*points)[0];
  int dx = abs(center.x - (*points)[1].x);
  int dy = center.y - (*points)[1].y;
  p1 = Point( center.x + round( 2.0 / 3.0 * dx ), center.y + dy );
  p2 = Point( center.x - dx, center.y - round( 1.0 / 3.0 * dy ) );
  p3 = Point( center.x + dx, center.y - round( 1.0 / 3.0 * dy ) );
  p4 = Point( center.x - round( 2.0 / 3.0 * dx ), center.y + dy );
  p5 = Point( center.x, center.y - dy );
}

void Star::draw()
{
  lineRGBA(surface, p1.x, p1.y, p2.x, p2.y, 0, 0, 0, 255);
  lineRGBA(surface, p2.x, p2.y, p3.x, p3.y, 0, 0, 0, 255);
  lineRGBA(surface, p3.x, p3.y, p4.x, p4.y, 0, 0, 0, 255);
  lineRGBA(surface, p4.x, p4.y, p5.x, p5.y, 0, 0, 0, 255);
  lineRGBA(surface, p5.x, p5.y, p1.x, p1.y, 0, 0, 0, 255);
}

void Star::move_point2(Point p)
{
  int dx = abs(center.x - p.x);
  int dy = center.y - p.y;
  p1 = Point( center.x + round( 2.0 / 3.0 * dx ), center.y + dy );
  p2 = Point( center.x - dx, center.y - round( 1.0 / 3.0 * dy ) );
  p3 = Point( center.x + dx, center.y - round( 1.0 / 3.0 * dy ) );
  p4 = Point( center.x - round( 2.0 / 3.0 * dx ), center.y + dy );
  p5 = Point( center.x, center.y - dy );
}
