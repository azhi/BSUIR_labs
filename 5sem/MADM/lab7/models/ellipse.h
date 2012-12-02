#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "figure.h"

class Ellipse : public Figure
{
  public:
    Ellipse(int x, int y, int rx, int ry);
    virtual ~Ellipse() {};

    void draw();

  private:
    Point center;
    int rx, ry;

};

#endif // ELLIPSE_H
