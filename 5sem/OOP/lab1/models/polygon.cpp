#include "polygon.h"

Polygon::Polygon() : Figure(), points(NULL) {}

Polygon::Polygon(vector<Point>* points) : Figure()
{
  this->points = new vector<Point>;
  this->points->insert(this->points->begin(), points->begin(), points->end());
}

void Polygon::draw()
{
  vector<Point>::iterator cur_point, prev_point;
  prev_point = points->begin();
  cur_point = prev_point + 1;
  while ( cur_point != points->end() )
  {
    lineRGBA(surface, prev_point->x, prev_point->y, cur_point->x, cur_point->y, 0, 0, 0, 255);
    cur_point++;
    prev_point++;
  }
}
