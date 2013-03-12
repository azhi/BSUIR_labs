#ifndef __DIGIT_ANALYZE_HASH_TABLE_H_
#define __DIGIT_ANALYZE_HASH_TABLE_H_

#include "virtual_hash_table.h"

class DigitAnalyzeHashTable : public VirtualHashTable
{
  public:
    DigitAnalyzeHashTable();
    DigitAnalyzeHashTable(unsigned package_count, unsigned package_size);

  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned version);

    virtual ull calc_hash(ull key);
    virtual ull scale_hash(ull hash);

    unsigned mod;
};

BOOST_CLASS_EXPORT_KEY(DigitAnalyzeHashTable)

template<class Archive>
void DigitAnalyzeHashTable::serialize(Archive &ar, const unsigned version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(VirtualHashTable);
  ar & BOOST_SERIALIZATION_NVP(mod);
}

#endif // __DIGIT_ANALYZE_HASH_TABLE_H_
