#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "figure.h"
#include <boost/serialization/export.hpp>

class Ellipse : public Figure
{
  public:
    Ellipse();
    Ellipse(vector<Point>* points);
    virtual ~Ellipse() {};

    void draw();
    void move_point2(Point p);

    template<class Archive>
      void serialize(Archive& ar, const unsigned int version)
      {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Figure);
        ar & BOOST_SERIALIZATION_NVP(center.x);
        ar & BOOST_SERIALIZATION_NVP(center.y);
        ar & BOOST_SERIALIZATION_NVP(rx);
        ar & BOOST_SERIALIZATION_NVP(ry);
      }

  private:
    Point center;
    int rx, ry;

};

extern "C"
{
  Figure* maker(vector<Point>* points)
  {
    return (Figure*) new Ellipse(points);
  }

  bool draw_mode()
  {
    return false;
  }
}

BOOST_CLASS_EXPORT_KEY(Ellipse)
BOOST_CLASS_EXPORT_IMPLEMENT(Ellipse)
#endif // ELLIPSE_H
