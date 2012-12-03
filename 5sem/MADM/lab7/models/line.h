#ifndef LINE_H
#define LINE_H

#include "figure.h"

class Line : public Figure
{
  public:
    Line(int x1, int y1, int x2, int y2);
    virtual ~Line() {};

    Point get_p1() { return p1; };
    Point get_p2() { return p2; };

    void draw();

    Rect_range get_rect_space();

  private:
    Point p1, p2;

};

#endif // LINE_H
