#ifndef __INTERSPACE_H_
#define __INTERSPACE_H_

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include <list>

#include <boost/utility.hpp>

#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "item.h"

using namespace std;

template<class Tk, class Tf>
class Interspace
{
  public:
    Interspace(unsigned length);
    Interspace(unsigned length, vector< Item<Tk, Tf> > *items);
    ~Interspace();

    vector< Item<Tk, Tf> *> *find_item(Tk key);
    bool add_item(Item<Tk, Tf>& item);
    Interspace<Tk, Tf> *divide();
    std::string to_json() const;

    Tk get_max_key();
    void set_max_key(Tk val);
    bool is_free();

  private:
    Interspace(){};
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned version);

    unsigned length;
    Item<Tk, Tf> max_key_item;
    Tk max_key;
    bool free;
    vector< Item<Tk, Tf> > *items;

    typedef Item<Tk, Tf> TItem;
    typedef typename vector<TItem>::iterator items_iterator;

    items_iterator get_middle_iterator();
};

template<class Tk, class Tf>
Tk Interspace<Tk, Tf>::get_max_key()
{
  return max_key;
}

template<class Tk, class Tf>
void Interspace<Tk, Tf>::set_max_key(Tk val)
{
  max_key = val;
  max_key_item.key = max_key;
}

template<class Tk, class Tf>
bool Interspace<Tk, Tf>::is_free()
{
  return free;
}

template<class Tk, class Tf>
Interspace<Tk, Tf>::Interspace(unsigned length)
  : length(length)
{
  items = new vector< Item<Tk ,Tf> >();
  items->reserve(length);
  max_key_item.key = Tk();
  max_key_item.field = Tf();
  free = true;
}

template<class Tk, class Tf>
Interspace<Tk, Tf>::Interspace(unsigned length, vector< Item<Tk, Tf> > *items)
  : length(length), items(items)
{
  items_iterator max_key_it = max_element(items->begin(), items->end(),
                             item_comparer<Tk, Tf>);
  max_key_item = *max_key_it;
  max_key = max_key_item.key;
  free = false;
}

template<class Tk, class Tf>
Interspace<Tk, Tf>::~Interspace()
{
  delete items;
}

template<class Tk, class Tf>
vector< Item<Tk, Tf> *> *Interspace<Tk, Tf>::find_item(Tk key)
{
  vector< Item<Tk, Tf> *> *res = new vector< Item<Tk, Tf> *>();
  if (free)
    return res;

  if (max_key_item.compare_to(key) > 0)
    return res;

  TItem key_item = {key, Tf()};
  items_iterator lb = lower_bound(items->begin(), items->end(), key_item,
      item_comparer<Tk, Tf>);

  while (lb != items->end() && item_comparer<Tk, Tf>(key_item, *lb) == 0)
  {
    res->push_back(&(*lb));
    lb++;
  }
  return res;
}

template<class Tk, class Tf>
bool Interspace<Tk, Tf>::add_item(Item<Tk, Tf>& item)
{
  if (item_comparer<Tk, Tf>(max_key_item, item))
    throw new invalid_argument("key is larger than max_key");

  free = false;
  if ( items->size() >= length )
    return false;
  else
  {
    items_iterator ub = lower_bound(items->begin(), items->end(), item,
        item_comparer<Tk, Tf>);
    items->insert(ub, item);
    return true;
  }
}

template<class Tk, class Tf>
Interspace<Tk, Tf> *Interspace<Tk, Tf>::divide()
{
  // Copy [size/2 .. last] elements to a new vector.
  items_iterator middle = get_middle_iterator();
  vector< Item<Tk, Tf> > *o_items =
    new vector< Item<Tk, Tf> >(middle, items->end());
  o_items->reserve(length);

  // Clean the interspace vector up.
  items->erase(middle, items->end());

  items_iterator max_key_item_it = max_element(items->begin(), items->end(),
                             item_comparer<Tk, Tf>);
  max_key_item = *max_key_item_it;
  max_key = max_key_item.key;

  return new Interspace(length, o_items);
}

template<class Tk, class Tf>
std::string Interspace<Tk, Tf>::to_json() const
{
  std::string res = std::string("{\"maxKey\" : \"") +
    std::to_string(max_key) +
    "\", \"items\" : [\n";

  for (auto it = items->begin(); it != items->end(); ++it) {
    res += it->to_json();
    if (boost::next(it) == items->end())
      res += "\n";
    else
      res += ",\n";
  }
  res += std::string("]}");
  return res;
}

template<class Tk, class Tf>
typename vector< Item<Tk, Tf> >::iterator Interspace<Tk, Tf>::get_middle_iterator()
{
  items_iterator middle = items->begin() + items->size() / 2;
  Tk key = middle->key;
  while (middle < items->end() && middle->compare_to(key) == 0)
    middle++;

  if ( middle == items->end() ) {
    middle = items->end() - 1 - items->size() / 2;
    key = middle->key;
    while (middle >= items->begin() && middle->compare_to(key) == 0)
      middle--;
  }

  return middle;
}

template<class Tk, class Tf>
template<class Archive>
void Interspace<Tk, Tf>::serialize(Archive &ar, const unsigned version)
{
  ar & BOOST_SERIALIZATION_NVP(length);
  ar & BOOST_SERIALIZATION_NVP(max_key_item);
  ar & BOOST_SERIALIZATION_NVP(max_key);
  ar & BOOST_SERIALIZATION_NVP(free);
  ar & BOOST_SERIALIZATION_NVP(items);
}

#endif // __INTERSPACE_H_
