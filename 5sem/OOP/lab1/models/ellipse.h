#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "figure.h"
#include <boost/serialization/export.hpp>

class Ellipse : public Figure
{
  public:
    Ellipse();
    Ellipse(int x, int y, int rx, int ry);
    virtual ~Ellipse() {};

    void draw();

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

BOOST_CLASS_EXPORT_KEY(Ellipse)
#endif // ELLIPSE_H
