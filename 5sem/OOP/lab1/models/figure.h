#ifndef FIGURE_H
#define FIGURE_H

#include <SDL/SDL.h>

typedef unsigned short FIGURE_TYPE;

#define FG_LINE 1
#define FG_RECTANGLE 2
#define FG_ELLIPSE 3

struct Point
{
  int x,y;
  Point() : x(0), y(0) {}
  Point(int x, int y) : x(x), y(y) {}
};

class Figure
{
  public:
    Figure(FIGURE_TYPE type) : type(type) {};
    virtual ~Figure() {};

    FIGURE_TYPE get_type() { return type; };

    void set_surface(SDL_Surface* surface) { this->surface = surface; };

    virtual void draw() {};

  protected:
    SDL_Surface* surface;

  private:
    FIGURE_TYPE type;

};

#endif // FIGURE_H
