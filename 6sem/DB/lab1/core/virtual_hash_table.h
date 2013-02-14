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
  long count;
};

class VirtualHashTable
{
  public:
    VirtualHashTable(int package_count, int package_size);
    virtual ~VirtualHashTable();

    void add_record(Item& record);
    Item* find_record(std::string& key);


  protected:
    virtual long calc_hash(long key) = 0;
    virtual int scale_hash(long hash);

  private:
    Package* packages;
    int package_count, package_size;
    int package_count_order;

    int count_in_packages, count_in_overflow;

    long convert_key_to_long(std::string& key);

    bool add_record_to_package(int package_index, Item& record);
    Item* find_record_in_package(int package_index, std::string& key);

};

#endif // __VIRTUAL_HASH_TABLE_H_
