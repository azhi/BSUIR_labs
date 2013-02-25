#include "shift_hash_table.h"

#include <math.h>
#include <stdio.h>

unsigned ShiftHashTable::calc_hash(unsigned key)
{
  unsigned res = key;
  unsigned length = 32;
  do
  {
    unsigned right_mask = (1l << (length / 2)) - 1;
    unsigned left_mask = ((1l << length) - 1) ^ right_mask;
    res = (res & left_mask) + (res & right_mask);
    length /= 2;
  }
  while ( res > pow(10, package_count_order) );
  return res;
}

unsigned ShiftHashTable::scale_hash(unsigned hash)
{
  return hash * (package_count - 1) / (pow(10, package_count_order) - 1);
}
