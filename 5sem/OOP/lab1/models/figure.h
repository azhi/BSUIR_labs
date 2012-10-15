#ifndef FIGURE_H
#define FIGURE_H

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/nvp.hpp>
#include <string>
#include <vector>

using namespace std;

struct Point
{
  int x,y;
  Point() : x(0), y(0) {}
  Point(int x, int y) : x(x), y(y) {}

  template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & BOOST_SERIALIZATION_NVP(x);
      ar & BOOST_SERIALIZATION_NVP(y);
    }
};

class Figure
{
  public:
    Figure() {};
    virtual ~Figure() {};

    void set_surface(SDL_Surface* surface) { this->surface = surface; };

    virtual void draw() {};
    virtual void move_point2(Point p) {};

    template<class Archive>
      void serialize(Archive& ar, const unsigned int version) {};

  protected:
    SDL_Surface* surface;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Figure)
#endif // FIGURE_H
