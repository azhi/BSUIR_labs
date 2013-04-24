#ifndef __ITEMS_H_
#define __ITEMS_H_

#include <string>

#include <boost/serialization/export.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

template<class Tk>
int compare(const Tk key, const Tk key2);

template<class Tk, class Tf>
struct Item
{
  Tk key;
  Tf field;

  int compare_to(const Tk key) const;

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned version);
};

template<class Tk, class Tf>
int Item<Tk, Tf>::compare_to(const Tk key) const
{
  return compare(this->key, key);
}

template<class Tk, class Tf>
template<class Archive>
void Item<Tk, Tf>::serialize(Archive &ar, const unsigned version)
{
  ar & BOOST_SERIALIZATION_NVP(key);
  ar & BOOST_SERIALIZATION_NVP(field);
}

template<>
int compare(const int key, const int key2)
{
  if (key == key2)
      return 0;
    else if (key < key2)
      return -1;
    else
      return 1;
}

template<>
int compare(const std::string key, const std::string key2)
{
  return strcmp(key.c_str(), key2.c_str());
}

template<class Tk, class Tf>
int item_comparer(Item<Tk, Tf> &item1, Item<Tk, Tf> &item2)
{
  return item1.compare_to(item2.key);
}

#endif // __ITEMS_H_
