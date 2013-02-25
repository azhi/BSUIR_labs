#ifndef __DIGIT_ANALYZE_HASH_TABLE_H_
#define __DIGIT_ANALYZE_HASH_TABLE_H_

#include "virtual_hash_table.h"

class DigitAnalyzeHashTable : public VirtualHashTable
{
  public:
    DigitAnalyzeHashTable(unsigned package_count, unsigned package_size)
      : VirtualHashTable(package_count, package_size) {};

  private:
    virtual unsigned calc_hash(unsigned key);
    virtual unsigned scale_hash(unsigned hash);
};

#endif // __DIGIT_ANALYZE_HASH_TABLE_H_
