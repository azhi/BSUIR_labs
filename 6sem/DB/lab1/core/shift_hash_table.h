#ifndef __SHIFT_HASH_TABLE_H_
#define __SHIFT_HASH_TABLE_H_

#include "virtual_hash_table.h"

class ShiftHashTable : public VirtualHashTable
{
  ShiftHashTable(int package_count, int package_size)
    : VirtualHashTable(package_count, package_size) {};

  private:
    virtual long calc_hash(long key);
    virtual int scale_hash(long hash);
};

#endif // __SHIFT_HASH_TABLE_H_
