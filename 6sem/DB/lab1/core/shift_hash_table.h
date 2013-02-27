#ifndef __SHIFT_HASH_TABLE_H_
#define __SHIFT_HASH_TABLE_H_

#include "virtual_hash_table.h"

class ShiftHashTable : public VirtualHashTable
{
  public:
    ShiftHashTable(unsigned package_count, unsigned package_size)
      : VirtualHashTable(package_count, package_size) {
      };

  private:
    virtual ull calc_hash(ull key);
    virtual ull scale_hash(ull hash);
};

#endif // __SHIFT_HASH_TABLE_H_
