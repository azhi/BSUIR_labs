#include "digit_analyze_hash_table.h"

#include <cmath>

BOOST_CLASS_EXPORT_IMPLEMENT(DigitAnalyzeHashTable)

using namespace std;

DigitAnalyzeHashTable::DigitAnalyzeHashTable()
  : VirtualHashTable()
{
}

DigitAnalyzeHashTable::DigitAnalyzeHashTable(unsigned package_count,
                                             unsigned package_size)
  : VirtualHashTable(package_count, package_size)
{
  unsigned order = min(6u, package_count_order);
  mod = pow(10, order + 1);
}

ull DigitAnalyzeHashTable::calc_hash(ull key)
{
  return key % mod;
}

ull DigitAnalyzeHashTable::scale_hash(ull hash)
{
  return hash * (package_count - 1) / (mod - 1);
}
