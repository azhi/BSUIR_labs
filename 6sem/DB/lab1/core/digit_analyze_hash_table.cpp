#include "digit_analyze_hash_table.h"

#include <cmath>

using namespace std;

ull DigitAnalyzeHashTable::calc_hash(ull key)
{
  unsigned order = min(6u, package_count_order);
  unsigned mod = pow(10, order + 1);
  return key % mod;
}

ull DigitAnalyzeHashTable::scale_hash(ull hash)
{
  unsigned order = min(6u, package_count_order);
  unsigned mod = pow(10, order + 1);
  return hash * (package_count - 1) / (mod - 1);
}
