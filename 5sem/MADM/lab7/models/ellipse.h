#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "figure.h"

class Ellipse : public Figure
{
  public:
    Ellipse(int x, int y, int rx, int ry);
    virtual ~Ellipse() {};

    Point get_center() { return center; };
    int get_rx() { return rx; };
    int get_ry() { return ry; };

    void draw();

    Rect_range get_rect_space();

  private:
    Point center;
    int rx, ry;

};

#endif // ELLIPSE_H
