#ifndef LINE_H
#define LINE_H

#include "figure.h"
#include <boost/serialization/export.hpp>

class Line : public Figure
{
  public:
    Line();
    Line(int x1, int y1, int x2, int y2);
    virtual ~Line() {};

    void draw();

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

BOOST_CLASS_EXPORT_KEY(Line)
#endif // LINE_H
