#ifndef __SHIFT_HASH_TABLE_H_
#define __SHIFT_HASH_TABLE_H_

#include "virtual_hash_table.h"

class ShiftHashTable : public VirtualHashTable
{
  public:
    ShiftHashTable();
    ShiftHashTable(unsigned package_count, unsigned package_size);

  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned version);

    virtual ull calc_hash(ull key);
    virtual ull scale_hash(ull hash);

    double scale_multiplier;
    ull ten_in_package_count_order;
};

BOOST_CLASS_EXPORT_KEY(ShiftHashTable)

template<class Archive>
void ShiftHashTable::serialize(Archive &ar, const unsigned version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(VirtualHashTable);
  ar & BOOST_SERIALIZATION_NVP(scale_multiplier);
  ar & BOOST_SERIALIZATION_NVP(ten_in_package_count_order);
}

#endif // __SHIFT_HASH_TABLE_H_
