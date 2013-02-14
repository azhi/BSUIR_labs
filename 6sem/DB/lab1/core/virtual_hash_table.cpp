#include <stdlib.h>
#include <stdio.h>

#include "virtual_hash_table.h"

VirtualHashTable::VirtualHashTable(int package_count, int package_size)
  : package_count(package_count), package_size(package_size)
{
  packages = new Package[package_count];
  for (int i = 0; i < package_count; ++i)
    packages[i].data = new Item[package_size];
  package_count_order = 0;
  int package_count_tmp = package_count;
  while ( package_count_tmp != 0 )
  {
    package_count_order++;
    package_count_tmp /= 10;
  }
  fprintf(stderr, "order: %d\n", package_count_order);
}

VirtualHashTable::~VirtualHashTable()
{
  for (int i = 0; i < package_count; ++i)
    delete packages[i].data;
  delete packages;
}

void VirtualHashTable::add_record(Item& record)
{
  long numeric_key = convert_key_to_long(record.key);
  long hash = calc_hash(numeric_key);
  long package_index = scale_hash(hash);

  long actual_package_index = package_index;
  while( !add_record_to_package(actual_package_index, record) )
    actual_package_index = (actual_package_index + 1) % package_count;
  if ( package_index == actual_package_index )
    count_in_packages++;
  else
    count_in_overflow++;
}

Item* VirtualHashTable::find_record(std::string& key)
{
  long numeric_key = convert_key_to_long(key);
  long hash = calc_hash(numeric_key);
  long package_index = scale_hash(hash);

  Item* res = NULL;
  do
  {
    res = find_record_in_package(package_index++, key);
  } while ( packages[package_index].count == package_size && res == NULL );
}

long VirtualHashTable::convert_key_to_long(std::string& key)
{
  return strtol(key.c_str(), NULL, 16);
}

int VirtualHashTable::scale_hash(long hash)
{
  // 99 - not magic number, actually this is just example, must be concretized
  // in inherited classes
  return hash * package_count / 99;
}

bool VirtualHashTable::add_record_to_package(int package_index, Item& record)
{
  if ( packages[package_index].count == package_size )
    return false;
  packages[package_index].data[ packages[package_index].count++ ] = record;
  return true;
}

Item* VirtualHashTable::find_record_in_package(int package_index, std::string& key)
{
  Item* res = NULL;
  for (int i = 0; i < packages[package_index].count; ++i)
    if ( packages[package_index].data[i].key.compare(key) != 0 )
      res = &packages[package_index].data[i];
  return res;
}
