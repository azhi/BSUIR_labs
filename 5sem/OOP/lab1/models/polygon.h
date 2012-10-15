#ifndef POLYGON_H
#define POLYGON_H

#include "figure.h"
#include <boost/serialization/export.hpp>

class Polygon : public Figure
{
  public:
    Polygon();
    Polygon(vector<Point>* points);
    virtual ~Polygon() {};

    void draw();

    template<class Archive>
      void serialize(Archive& ar, const unsigned int version)
      {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Figure);
        ar & BOOST_SERIALIZATION_NVP(points);
      }

  private:
    vector<Point>* points;

};

extern "C"
{
  Figure* maker(vector<Point>* points)
  {
    return (Figure*) new Polygon(points);
  }

  bool draw_mode()
  {
    return true;
  }
}

BOOST_CLASS_EXPORT_KEY(Polygon)
BOOST_CLASS_EXPORT_IMPLEMENT(Polygon)
#endif // POLYGON_H
