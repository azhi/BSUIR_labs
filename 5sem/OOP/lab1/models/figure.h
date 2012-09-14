#ifndef FIGURE_H
#define FIGURE_H

#include <SDL/SDL.h>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/nvp.hpp>

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
    template<class Archive>
      void serialize(Archive& ar, const unsigned int version) {};

  protected:
    SDL_Surface* surface;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Figure)
#endif // FIGURE_H
