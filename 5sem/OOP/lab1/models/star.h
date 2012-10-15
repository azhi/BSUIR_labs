#ifndef STAR_H
#define STAR_H

#include "figure.h"
#include <boost/serialization/export.hpp>

class Star : public Figure
{
  public:
    Star();
    Star(vector<Point>* points);
    virtual ~Star() {};

    void draw();
    void move_point2(Point p);

    template<class Archive>
      void serialize(Archive& ar, const unsigned int version)
      {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Figure);
        ar & BOOST_SERIALIZATION_NVP(center.x);
        ar & BOOST_SERIALIZATION_NVP(center.y);
        ar & BOOST_SERIALIZATION_NVP(p1.x);
        ar & BOOST_SERIALIZATION_NVP(p1.y);
        ar & BOOST_SERIALIZATION_NVP(p2.x);
        ar & BOOST_SERIALIZATION_NVP(p2.y);
        ar & BOOST_SERIALIZATION_NVP(p3.x);
        ar & BOOST_SERIALIZATION_NVP(p3.y);
        ar & BOOST_SERIALIZATION_NVP(p4.x);
        ar & BOOST_SERIALIZATION_NVP(p4.y);
        ar & BOOST_SERIALIZATION_NVP(p5.x);
        ar & BOOST_SERIALIZATION_NVP(p5.y);
      }

  private:
    Point center;
    Point p1, p2, p3, p4, p5;

};

extern "C"
{
  Figure* maker(vector<Point>* points)
  {
    return (Figure*) new Star(points);
  }

  bool draw_mode()
  {
    return false;
  }
}

BOOST_CLASS_EXPORT_KEY(Star)
BOOST_CLASS_EXPORT_IMPLEMENT(Star)
#endif // STAR_H
