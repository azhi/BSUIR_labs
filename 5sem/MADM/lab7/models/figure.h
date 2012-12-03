#ifndef FIGURE_H
#define FIGURE_H

#include <SDL/SDL.h>

#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) > (y) ? (x) : (y))

struct Point
{
  int x,y;
  Point() : x(0), y(0) {}
  Point(int x, int y) : x(x), y(y) {}
};

struct Range
{
  int p1, p2;
  Range() : p1(0), p2(0) {};
  Range(int p1, int p2) : p1(p1), p2(p2) {};
};

struct Rect_range
{
  Range xrange, yrange;
};

#define T_LINE 1
#define T_ELLIPSE 2
#define T_RECTANGLE 3

class Figure
{
  public:
    Figure(short type) : type(type) {};
    virtual ~Figure() {};

    void set_surface(SDL_Surface* surface) { this->surface = surface; };
    short get_type() { return type; };

    virtual void draw() {};

    virtual Rect_range get_rect_space() {};

    bool is_above(Figure* other)
    {
      Rect_range my_range = get_rect_space();
      Rect_range other_range = other->get_rect_space();
      if ( abs( my_range.yrange.p2 - other_range.yrange.p1 ) < 20 ||
          my_range.yrange.p2 < other_range.yrange.p1 )
        return true;
      else
        return false;
    };

    bool is_left(Figure* other)
    {
      Rect_range my_range = get_rect_space();
      Rect_range other_range = other->get_rect_space();
      if ( abs( my_range.xrange.p2 - other_range.xrange.p1 ) < 20 ||
          my_range.xrange.p2 < other_range.xrange.p1 )
        return true;
      else
        return false;
    };

    bool is_inside(Figure* other)
    {
      Rect_range my_range = get_rect_space();
      Rect_range other_range = other->get_rect_space();
      if ( (my_range.xrange.p1 > other_range.xrange.p1 - 20) &&
          (my_range.xrange.p1 < other_range.xrange.p2) &&
          (my_range.xrange.p2 < other_range.xrange.p2 + 20) &&
          (my_range.xrange.p2 > other_range.xrange.p1) &&
          (my_range.yrange.p1 > other_range.yrange.p1 - 20) &&
          (my_range.yrange.p1 < other_range.yrange.p2) &&
          (my_range.yrange.p2 < other_range.yrange.p2 + 20) &&
          (my_range.yrange.p2 > other_range.yrange.p1)
         )
        return true;
      else
        return false;
    };

    bool is_rigth_beneath(Figure* other)
    {
      Rect_range my_range = get_rect_space();
      Rect_range other_range = other->get_rect_space();
      if ( abs( my_range.yrange.p1 - other_range.yrange.p2 ) < 20 )
        return true;
      else
        return false;
    };

    bool is_under(Figure* other)
    {
      Rect_range my_range = get_rect_space();
      Rect_range other_range = other->get_rect_space();
      if ( abs( my_range.yrange.p1 - other_range.yrange.p2 ) < 20 ||
           my_range.yrange.p1 > other_range.yrange.p2 )
        return true;
      else
        return false;
    };

  protected:
    SDL_Surface* surface;
    short type;
};

#endif // FIGURE_H
