#include "digit_analyze_hash_table.h"

ull DigitAnalyzeHashTable::calc_hash(ull key)
{
  return key % 1000000000ull;
}

ull DigitAnalyzeHashTable::scale_hash(ull hash)
{
  return hash * (package_count - 1) / 999999999ull;
}
