#ifndef __AREA_H_
#define __AREA_H_

#include <cstdio>
#include <iostream>
#include <vector>

#include <boost/utility.hpp>

#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "interspace.h"

using namespace std;

template<class Tk, class Tf>
class Area
{
  public:
    Area(Tk max_key);
    Area(unsigned size, vector< Interspace<Tk, Tf> *> *index);

    vector< Item<Tk, Tf> *> *find_item(Tk key);
    bool add_item(Item<Tk, Tf> &item);
    Area<Tk, Tf> *divide();

    Tk get_max_key();
    void set_max_key(Tk val);

    void add_interspace(Interspace<Tk, Tf> *interspace);
    std::string to_json() const;

  private:
    Area() {};
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned version);

    unsigned size, interspace_length;
    Tk max_key;
    Item<Tk, Tf> max_key_item;
    vector< Interspace<Tk, Tf>* > *interspace_index;

    typedef Interspace<Tk, Tf> TInterspace;
    typedef typename vector<TInterspace *>::iterator interspace_iterator;

    interspace_iterator get_middle_iterator();

    static bool compare(TInterspace* int1, TInterspace* int2);
};

template<class Tk, class Tf>
Area<Tk, Tf>::Area(Tk max_key) : size(0), interspace_length(0),
  max_key(max_key), interspace_index(0)
{}

template<class Tk, class Tf>
Area<Tk, Tf>::Area(unsigned size, vector< Interspace<Tk, Tf> *> *index) :
  size(size)
{
  interspace_index = index;
  interspace_iterator max_key_interspace = max_element(interspace_index->begin(), interspace_index->end(),
                                               compare);
  max_key = (*max_key_interspace)->get_max_key();
  max_key_item.key = max_key;
  max_key_item.field = Tf();
}

template<class Tk, class Tf>
Tk Area<Tk, Tf>::get_max_key()
{
  return max_key;
}

template<class Tk, class Tf>
void Area<Tk, Tf>::set_max_key(Tk val)
{
  max_key = val;
  max_key_item.key = max_key;
  interspace_iterator last = --interspace_index->end();
  (*last)->set_max_key(val);
}

template<class Tk, class Tf>
vector< Item<Tk, Tf> *> *Area<Tk, Tf>::find_item(Tk key)
{
  if (max_key_item.compare_to(key) > 0)
    return new vector< Item<Tk, Tf> *>;

  Item<Tk, Tf> key_item;
  key_item.key = key;
  key_item.field = Tf();

  interspace_iterator it;
  for (it = interspace_index->begin(); it < interspace_index->end(); ++it)
  {
    if ( (*it)->is_free() )
      continue;
    Item<Tk, Tf> interspace_max_key_item;
    interspace_max_key_item.key = (*it)->get_max_key();
    interspace_max_key_item.field = Tf();
    if ( item_comparer<Tk, Tf>(key_item, interspace_max_key_item) )
      break;
  }

  return (*it)->find_item(key);
}

template<class Tk, class Tf>
bool Area<Tk, Tf>::add_item(Item<Tk, Tf> &item)
{
  if (item_comparer<Tk, Tf>(max_key_item, item))
    throw new invalid_argument("key is larger than max_key");

  interspace_iterator it;
  for (it = interspace_index->begin(); it < interspace_index->end(); ++it)
  {
    if ( (*it)->is_free() )
      continue;
    Item<Tk, Tf> interspace_max_key_item;
    interspace_max_key_item.key = (*it)->get_max_key();
    interspace_max_key_item.field = Tf();
    if ( !item_comparer<Tk, Tf>(interspace_max_key_item, item) )
      break;
  }

  bool res = (*it)->add_item(item);
  if ( !res )
    if ( interspace_index->size() < size )
    {
      // cerr << "DIVIDING interspace " << (*it)->get_max_key() << endl;
      TInterspace* spl_interspace = (*it)->divide();
      add_interspace(spl_interspace);
      // cerr << "RESULT interspaces: " << (*it)->get_max_key() << " -- " << spl_interspace->get_max_key() << endl;

      // WARNING: potential never-ending recursion!
      res = add_item(item);
    }
  return res;
}

template<class Tk, class Tf>
Area<Tk, Tf> *Area<Tk, Tf>::divide()
{
  // Copy [size/2 .. last] elements to a new vector.
  vector<TInterspace *> *o_interspaces =
    new vector<TInterspace *>(interspace_index->begin() + size / 2, interspace_index->end());
  o_interspaces->reserve(size);

  // Clean the interspace vector up.
  interspace_index->erase(interspace_index->begin() + size / 2, interspace_index->end());

  interspace_iterator max_key_interspace_it = max_element(interspace_index->begin(), interspace_index->end(),
                                               compare);
  max_key = (*max_key_interspace_it)->get_max_key();
  max_key_item.key = max_key;
  max_key_item.field = Tf();

  return new Area(size, o_interspaces);
}

template<class Tk, class Tf>
void Area<Tk, Tf>::add_interspace(Interspace<Tk, Tf> *interspace)
{
  if (interspace_index->size() > size - 1)
    throw new invalid_argument("attempt to add interspace when no space left");

  Item<Tk, Tf> key_item;
  key_item.key = interspace->get_max_key();
  key_item.field = Tf();

  interspace_iterator it;
  for (it = interspace_index->begin(); it < interspace_index->end(); ++it)
  {
    if ( (*it)->is_free() )
      continue;
    Item<Tk, Tf> interspace_max_key_item;
    interspace_max_key_item.key = (*it)->get_max_key();
    interspace_max_key_item.field = Tf();
    if ( item_comparer<Tk, Tf>(key_item, interspace_max_key_item) )
      break;
  }
  interspace_index->insert(it, interspace);
}

template<class Tk, class Tf>
std::string Area<Tk, Tf>::to_json() const
{
  std::string res = std::string("{\"maxKey\" : \"") +
    std::to_string(max_key) +
    "\", \"interspaces\" : [\n";

  for (auto it = interspace_index->begin();
       it != interspace_index->end();
       ++it) {
    res += (*it)->to_json();
    if (boost::next(it) == interspace_index->end())
      res += "\n";
    else
      res += ",\n";
  }
  res += std::string("]}");
  return res;
}


template<class Tk, class Tf>
template<class Archive>
void Area<Tk, Tf>::serialize(Archive &ar, const unsigned version)
{
  ar & BOOST_SERIALIZATION_NVP(size);
  ar & BOOST_SERIALIZATION_NVP(interspace_length);
  ar & BOOST_SERIALIZATION_NVP(max_key);
  ar & BOOST_SERIALIZATION_NVP(max_key_item);
  ar & BOOST_SERIALIZATION_NVP(interspace_index);
}


template<class Tk, class Tf>
bool Area<Tk, Tf>::compare(TInterspace* int1, TInterspace* int2)
{
  Tk k1 = int1->get_max_key();
  Tk k2 = int2->get_max_key();
  return key_compare(k1, k2);
}

#endif // __AREA_H_
