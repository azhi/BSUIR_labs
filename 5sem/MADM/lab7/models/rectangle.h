#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "figure.h"

class Rectangle : public Figure
{
  public:
    Rectangle(int x1, int y1, int x2, int y2);
    virtual ~Rectangle() {};

    void draw();

  private:
    Point p1, p2;

};

#endif // RECTANGLE_H
