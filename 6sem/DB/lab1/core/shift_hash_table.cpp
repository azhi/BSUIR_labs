#include "shift_hash_table.h"

#include <cmath>
#include <cstdio>

BOOST_CLASS_EXPORT_IMPLEMENT(ShiftHashTable)

ShiftHashTable::ShiftHashTable()
      : VirtualHashTable()
{
}

ShiftHashTable::ShiftHashTable(unsigned package_count, unsigned package_size)
      : VirtualHashTable(package_count, package_size)
{
  ten_in_package_count_order = (ull) pow(10, package_count_order);
  scale_multiplier = (package_count - 1) / (double) (ten_in_package_count_order - 1);
}

ull ShiftHashTable::calc_hash(ull key)
{
  ull res = key;
  unsigned order = find_order(res);
  unsigned half_order;
  do
  {
    if ( order < package_count_order * 2 )
      res = (res % ten_in_package_count_order) + (res / ten_in_package_count_order);
    else
    {
      half_order = (order + 1) / 2;
      res = (res % (ull) pow(10, half_order)) + (res / (ull) pow(10, half_order));
    }
    order = find_order(res);
  } while ( order > package_count_order );
  return res;
}

ull ShiftHashTable::scale_hash(ull hash)
{
  return llround(hash * scale_multiplier);
}
