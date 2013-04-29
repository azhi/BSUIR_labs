#ifndef __ITEMS_H_
#define __ITEMS_H_

#include <string>

#include <boost/serialization/export.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

template<class Tk>
bool key_compare(const Tk key, const Tk key2);

template<class Tk, class Tf>
struct Item
{
  Tk key;
  Tf field;

  bool compare_to(const Tk key) const;

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned version);
};

template<class Tk, class Tf>
bool Item<Tk, Tf>::compare_to(const Tk key) const
{
  return key_compare(this->key, key);
}

template<class Tk, class Tf>
template<class Archive>
void Item<Tk, Tf>::serialize(Archive &ar, const unsigned version)
{
  ar & BOOST_SERIALIZATION_NVP(key);
  ar & BOOST_SERIALIZATION_NVP(field);
}

template<>
bool key_compare(const long key, const long key2)
{
  return key < key2;
}

template<>
bool key_compare(const std::string key, const std::string key2)
{
  return strcmp(key.c_str(), key2.c_str()) < 0;
}

template<class Tk, class Tf>
bool item_comparer(const Item<Tk, Tf> &item1,const Item<Tk, Tf> &item2)
{
  return item1.compare_to(item2.key);
}

#endif // __ITEMS_H_
