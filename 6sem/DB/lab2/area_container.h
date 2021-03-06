#ifndef __AREA_CONTAINER_H_
#define __AREA_CONTAINER_H_

#include <cstdio>
#include <list>
#include <string>

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

    vector< Item<Tk, Tf> *> *find_item(Tk key);
    bool add_item(Item<Tk, Tf> &item);

    void add_area(Area<Tk, Tf> *area);
    std::string to_json() const;
    inline unsigned get_size()
    {
      return area_index->size();
    }

  private:
    AreaContainer() {}
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned version);

    list< Area<Tk, Tf> > *area_index;

    typedef Area<Tk, Tf> TArea;
    typedef typename list<TArea>::iterator area_iterator;

    static bool compare(TArea ar1, TArea ar2);
};

template<class Tk, class Tf>
AreaContainer<Tk, Tf>::AreaContainer(list< Area<Tk, Tf> > *areas)
{
  area_index = areas;
}

template<class Tk, class Tf>
vector< Item<Tk, Tf> *> *AreaContainer<Tk, Tf>::find_item(Tk key)
{
  TArea key_area(key);
  area_iterator lb = lower_bound(area_index->begin(), area_index->end(), key_area,
      compare);

  vector< Item<Tk, Tf> *> *res = lb->find_item(key);
  lb++;
  Item<Tk, Tf> area_max_key_item;
  bool finish = false;
  while (lb != area_index->end() && !finish)
  {
    vector< Item<Tk, Tf> *> *sub_res = lb->find_item(key);
    if (sub_res != nullptr)
      res->insert(res->end(), sub_res->begin(), sub_res->end());
    lb++;
    finish = key_compare(key, lb->get_max_key());
  }
  return res;
}

template<class Tk, class Tf>
bool AreaContainer<Tk, Tf>::add_item(Item<Tk, Tf> &item)
{
  area_iterator last = --area_index->end();
  if ( key_compare(last->get_max_key(), item.key) )
    last->set_max_key(item.key);

  TArea key_area(item.key);
  area_iterator ub = lower_bound(area_index->begin(), area_index->end(), key_area,
      compare);

  bool res = ub->add_item(item);
  if ( !res )
  {
    // cerr << "DIVIDING area " << ub->get_max_key() << endl;
    TArea* spl_area = ub->divide();
    add_area(spl_area);
    // cerr << "RESULT areas: " << ub->get_max_key() << " -- " << spl_area->get_max_key() << endl;

    // WARNING: potential never-ending recursion!
    res = add_item(item);
  }
  return res;
}

template<class Tk, class Tf>
void AreaContainer<Tk, Tf>::add_area(Area<Tk, Tf> *area)
{
  area_iterator ub = lower_bound(area_index->begin(), area_index->end(), *area,
      compare);
  area_index->insert(ub, *area);
}

template<class Tk, class Tf>
std::string AreaContainer<Tk, Tf>::to_json() const
{
  std::string res = std::string("{\"areas\" : [\n");
  for (auto it = area_index->begin(); it != area_index->end(); ++it) {
    res += it->to_json();
    if (boost::next(it) == area_index->end())
      res += "\n";
    else
      res += ",\n";
  }
  res += std::string("]}");
  return res;
}

template<class Tk, class Tf>
template<class Archive>
void AreaContainer<Tk, Tf>::serialize(Archive &ar, const unsigned version)
{
  ar & BOOST_SERIALIZATION_NVP(area_index);
}

template<class Tk, class Tf>
bool AreaContainer<Tk, Tf>::compare(TArea ar1, TArea ar2)
{
  Tk k1 = ar1.get_max_key();
  Tk k2 = ar2.get_max_key();
  return key_compare(k1, k2);
}

#endif // __AREA_CONTAINER_H_
