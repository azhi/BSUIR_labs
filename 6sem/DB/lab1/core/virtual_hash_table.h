#ifndef __VIRTUAL_HASH_TABLE_H_
#define __VIRTUAL_HASH_TABLE_H_

#include <string>

struct Item
{
  std::string key;
  std::string field1;
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
    Item* find_record(std::string& key);

    unsigned count_in_packages, count_in_overflow;

  protected:
    virtual unsigned calc_hash(unsigned key) = 0;
    virtual unsigned scale_hash(unsigned hash) = 0;

    unsigned package_count_order;
    unsigned package_count, package_size;

  private:
    Package* packages;

    unsigned convert_key_to_int(std::string& key);

    bool add_record_to_package(unsigned package_index, Item& record);
    Item* find_record_in_package(unsigned package_index, std::string& key);

};

#endif // __VIRTUAL_HASH_TABLE_H_
