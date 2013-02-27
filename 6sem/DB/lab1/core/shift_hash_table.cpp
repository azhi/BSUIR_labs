#include "shift_hash_table.h"

#include <math.h>
#include <stdio.h>

ull ShiftHashTable::calc_hash(ull key)
{
  ull res = key;
  unsigned order = find_order(res);
  // res %= (ull) pow(10, order - 1);
  // order = find_order(res);
  unsigned half_order;
  do
  {
    if ( order < package_count_order * 2 )
      res = (res % (ull) pow(10, package_count_order)) + (res / (ull) pow(10, package_count_order));
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
  return hash * (package_count - 1) / (ull) (pow(10, package_count_order) - 1);
}
