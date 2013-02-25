#include <stdlib.h>
#include <stdio.h>

#include "virtual_hash_table.h"

using namespace std;

VirtualHashTable::VirtualHashTable(unsigned package_count, unsigned package_size)
  : package_count(package_count), package_size(package_size)
{
  packages = new Package[package_count];
  for (unsigned i = 0; i < package_count; ++i)
    packages[i].data = new Item[package_size];
  package_count_order = 0;
  unsigned package_count_tmp = package_count;
  while ( package_count_tmp != 0 )
  {
    package_count_order++;
    package_count_tmp /= 10;
  }
  fprintf(stderr, "order: %d\n", package_count_order);
}

VirtualHashTable::~VirtualHashTable()
{
  for (unsigned i = 0; i < package_count; ++i)
    delete packages[i].data;
  delete packages;
}

void VirtualHashTable::add_record(Item& record)
{
  unsigned numeric_key = convert_key_to_int(record.key);
  unsigned hash = calc_hash(numeric_key);
  unsigned package_index = scale_hash(hash);

  unsigned actual_package_index = package_index;
  while( !add_record_to_package(actual_package_index, record) )
    actual_package_index = (actual_package_index + 1) % package_count;
  if ( package_index == actual_package_index )
    count_in_packages++;
  else
    count_in_overflow++;
}

Item* VirtualHashTable::find_record(string& key)
{
  unsigned numeric_key = convert_key_to_int(key);
  unsigned hash = calc_hash(numeric_key);
  unsigned package_index = scale_hash(hash);

  Item* res = NULL;
  do
  {
    res = find_record_in_package(package_index++, key);
  } while ( packages[package_index].count == package_size && res == NULL );
  return res;
}

unsigned VirtualHashTable::convert_key_to_int(string& key)
{
  return strtol(key.c_str(), NULL, 16);
}

bool VirtualHashTable::add_record_to_package(unsigned package_index, Item& record)
{
  if ( packages[package_index].count == package_size )
    return false;
  packages[package_index].data[ packages[package_index].count++ ] = record;
  return true;
}

Item* VirtualHashTable::find_record_in_package(unsigned package_index, string& key)
{
  Item* res = NULL;
  for (unsigned i = 0; i < packages[package_index].count; ++i)
  {
    if ( packages[package_index].data[i].key.compare(key) == 0 )
      res = &packages[package_index].data[i];
  }
  return res;
}
