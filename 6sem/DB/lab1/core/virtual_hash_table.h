#ifndef __VIRTUAL_HASH_TABLE_H_
#define __VIRTUAL_HASH_TABLE_H_

#include <fstream>
#include <string>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

typedef unsigned long long ull;

struct Item
{
  char key[7];
  char field1[13];
  long field2;
};

struct Package
{
  Item *data;
  unsigned count;
};

class VirtualHashTable
{
  public:
    VirtualHashTable();
    VirtualHashTable(unsigned package_count, unsigned package_size);
    virtual ~VirtualHashTable();

    void add_record(Item& record);
    Item* find_record(char* key);
    virtual void dump(std::ofstream &ofs);

    static bool is_key(char* key);

    unsigned count_in_packages, count_in_overflow;
    unsigned package_count_order;
    unsigned package_count, package_size;

  protected:
    virtual ull calc_hash(ull key) = 0;
    virtual ull scale_hash(ull hash) = 0;
    unsigned find_order(ull number);

  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned version);

    Package* packages;

    ull convert_key_to_int(char* key);

    bool add_record_to_package(unsigned package_index, Item& record);
    Item* find_record_in_package(unsigned package_index, char* key);
    static bool str_comparer(char* s1, char* s2);
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(VirtualHashTable)

template<class Archive>
void VirtualHashTable::serialize(Archive &ar, const unsigned version)
{
  ar & BOOST_SERIALIZATION_NVP(count_in_packages);
  ar & BOOST_SERIALIZATION_NVP(count_in_overflow);
  ar & BOOST_SERIALIZATION_NVP(package_count_order);
  ar & BOOST_SERIALIZATION_NVP(package_count);
  ar & BOOST_SERIALIZATION_NVP(package_size);

  bool load = nullptr == packages;
  if (load)
    packages = new Package[package_count];

  for (unsigned i = 0; i < package_count; ++i) {
    ar & BOOST_SERIALIZATION_NVP(packages[i].count);
    if (load)
      packages[i].data = new Item[package_size];
    for (unsigned j = 0; j < package_size; ++j) {
      Item &item = packages[i].data[j];
      ar & BOOST_SERIALIZATION_NVP(item.key);
      ar & BOOST_SERIALIZATION_NVP(item.field1);
      ar & BOOST_SERIALIZATION_NVP(item.field2);
    }
  }
}

#endif // __VIRTUAL_HASH_TABLE_H_
