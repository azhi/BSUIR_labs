#ifndef __AREA_CONTAINER_H_
#define __AREA_CONTAINER_H_

#include <list>

#include <boost/serialization/export.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "area.h"

using namespace std;

template<class Tk, class Tf>
class AreaContainer
{
  public:
    AreaContainer(list< Area<Tk, Tf> > *areas);

    Item<Tk, Tf> *find_item(Tk key);
    bool add_item(Item<Tk, Tf> &item);

    void add_area(Area<Tk, Tf> *area);

  private:
    AreaContainer() {};
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned version);

    list< Area<Tk, Tf> > *area_index;

    typedef Area<Tk, Tf> TArea;
    typedef typename vector<TArea>::iterator area_iterator;

    static int compare(TArea ar1, TArea ar2);
};

template<class Tk, class Tf>
AreaContainer<Tk, Tf>::AreaContainer(list< Area<Tk, Tf> > *areas)
{
  area_index = areas;
}


template<class Tk, class Tf>
Item<Tk, Tf> *AreaContainer<Tk, Tf>::find_item(Tk key)
{
  TArea key_area(key);
  area_iterator lb = lower_bound(area_index->first(), area_index->last(), key_area,
      compare);

  return lb->find_item(key);
}

template<class Tk, class Tf>
bool AreaContainer<Tk, Tf>::add_item(Item<Tk, Tf> &item)
{
  TArea key_area(item.key);
  area_iterator ub = upper_bound(area_index->first(), area_index->last(), key_area,
      compare);

  bool res = ub->add_item(item);
  if ( !res )
  {
    TArea* spl_area = ub->divide();
    add_area(spl_area);
    // WARNING: potential never-ending recursion!
    res = add_item(item);
  }
  return res;
}

template<class Tk, class Tf>
void AreaContainer<Tk, Tf>::add_area(Area<Tk, Tf> *area)
{
  area_iterator ub = upper_bound(area_index->first(), area_index->last(), *area,
      compare);
  area_index->insert(ub, *area);
}


template<class Tk, class Tf>
template<class Archive>
void AreaContainer<Tk, Tf>::serialize(Archive &ar, const unsigned version)
{
  ar & BOOST_SERIALIZATION_NVP(area_index);
}


template<class Tk, class Tf>
int AreaContainer<Tk, Tf>::compare(TArea ar1, TArea ar2)
{
  Tk k1 = ar1.get_max_key();
  Tk k2 = ar2.get_max_key();
  if ( k1 == k2 )
    return 0;
  else if ( k1 < k2 )
    return -1;
  else
    return 1;
}

#endif // __AREA_CONTAINER_H_
