#ifndef __VIRTUAL_HASH_TABLE_H_
#define __VIRTUAL_HASH_TABLE_H_

#include <string>

typedef unsigned long long ull;

struct Item
{
  char key[7];
  char field1[13];
  long field2;
};

struct Package
{
  Item *data;
  unsigned count;
};

class VirtualHashTable
{
  public:
    VirtualHashTable(unsigned package_count, unsigned package_size);
    virtual ~VirtualHashTable();

    void add_record(Item& record);
    Item* find_record(char* key);

    unsigned count_in_packages, count_in_overflow;
    unsigned package_count_order;

  protected:
    virtual ull calc_hash(ull key) = 0;
    virtual ull scale_hash(ull hash) = 0;
    unsigned find_order(ull number);

    unsigned package_count, package_size;

  private:
    Package* packages;

    ull convert_key_to_int(char* key);

    bool add_record_to_package(unsigned package_index, Item& record);
    Item* find_record_in_package(unsigned package_index, char* key);
    static bool str_comparer(char* s1, char* s2);
};

#endif // __VIRTUAL_HASH_TABLE_H_
