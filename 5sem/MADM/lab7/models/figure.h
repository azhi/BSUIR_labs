#ifndef FIGURE_H
#define FIGURE_H

#include <SDL/SDL.h>

struct Point
{
  int x,y;
  Point() : x(0), y(0) {}
  Point(int x, int y) : x(x), y(y) {}
};

class Figure
{
  public:
    Figure() {};
    virtual ~Figure() {};

    void set_surface(SDL_Surface* surface) { this->surface = surface; };

    virtual void draw() {};

  protected:
    SDL_Surface* surface;
};

#endif // FIGURE_H
