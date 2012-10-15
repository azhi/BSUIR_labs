#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "figure.h"
#include <boost/serialization/export.hpp>

class Rectangle : public Figure
{
  public:
    Rectangle();
    Rectangle(vector<Point>* points);
    virtual ~Rectangle() {};

    void draw();
    void move_point2(Point p);

    template<class Archive>
      void serialize(Archive& ar, const unsigned int version)
      {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Figure);
        ar & BOOST_SERIALIZATION_NVP(p1.x);
        ar & BOOST_SERIALIZATION_NVP(p1.y);
        ar & BOOST_SERIALIZATION_NVP(p2.x);
        ar & BOOST_SERIALIZATION_NVP(p2.y);
      }

  private:
    Point p1, p2;

};

extern "C"
{
  Figure* maker(vector<Point>* points)
  {
    return (Figure*) new Rectangle(points);
  }

  bool draw_mode()
  {
    return false;
  }
}

BOOST_CLASS_EXPORT_KEY(Rectangle)
BOOST_CLASS_EXPORT_IMPLEMENT(Rectangle)
#endif // RECTANGLE_H
