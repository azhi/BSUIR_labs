#ifndef LINE_H
#define LINE_H

#include "figure.h"

class Line : public Figure
{
  public:
    Line(int x1, int y1, int x2, int y2);
    virtual ~Line() {};

    void draw();

  private:
    Point p1, p2;

};

#endif // LINE_H
