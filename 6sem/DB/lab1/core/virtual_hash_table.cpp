#include "virtual_hash_table.h"

#include <cstdlib>
#include <cstdio>

using namespace std;

VirtualHashTable::VirtualHashTable()
  : packages(nullptr)
{
}

VirtualHashTable::VirtualHashTable(unsigned package_count, unsigned package_size)
  : package_count(package_count), package_size(package_size)
{
  packages = new Package[package_count];
  for (unsigned i = 0; i < package_count; ++i)
  {
    packages[i].data = new Item[package_size];
    packages[i].count = 0;
  }
  package_count_order = find_order(package_count);
}

VirtualHashTable::~VirtualHashTable()
{
  for (unsigned i = 0; i < package_count; ++i)
    delete packages[i].data;
  delete packages;
}

void VirtualHashTable::add_record(Item& record)
{
  ull numeric_key = convert_key_to_int(record.key);
  ull hash = calc_hash(numeric_key);
  ull package_index = scale_hash(hash);

  unsigned actual_package_index = package_index;
  while( !add_record_to_package(actual_package_index, record) )
    actual_package_index = (actual_package_index + 1) % package_count;
  if ( package_index == actual_package_index )
    count_in_packages++;
  else
    count_in_overflow++;
}

Item* VirtualHashTable::find_record(char* key)
{
  ull numeric_key = convert_key_to_int(key);
  ull hash = calc_hash(numeric_key);
  ull package_index = scale_hash(hash);

  Item* res = nullptr;
  do
  {
    res = find_record_in_package(package_index, key);
    package_index = (package_index + 1) % package_count;
  } while ( packages[package_index - 1].count == package_size && res == NULL );
  return res;
}

void VirtualHashTable::dump(ofstream &ofs)
{
  if (!ofs) return;
  boost::archive::binary_oarchive oa(ofs);
  oa << BOOST_SERIALIZATION_NVP(this);
}

ull VirtualHashTable::convert_key_to_int(char* key)
{
  ull res = 0;
  for (int i = 0; i < 6; ++i)
  {
    char sym = key[i];
    if ( sym >= '0' && sym <= '9' )
      sym -= '0';
    else if ( sym >= 'a' && sym <= 'z' )
      sym -= 'a' + '9' - '0';
    else if ( sym >= 'A' && sym <= 'Z' )
      sym -= 'A' + 'z' - 'a' + '9' - '0';
    res *= 62;
    res += sym;
  }
  return res;
}

bool VirtualHashTable::add_record_to_package(unsigned package_index, Item& record)
{
  if ( packages[package_index].count == package_size )
    return false;
  packages[package_index].data[ packages[package_index].count++ ] = record;
  return true;
}

Item* VirtualHashTable::find_record_in_package(unsigned package_index, char* key)
{
  Item* res = NULL;
  for (unsigned i = 0; i < packages[package_index].count; ++i)
  {
    if ( str_comparer(packages[package_index].data[i].key, key) )
    {
      res = &packages[package_index].data[i];
      break;
    }
  }
  return res;
}

bool VirtualHashTable::str_comparer(char* s1, char* s2)
{
  int* pi1 = (int*) s1; int* pi2 = (int*) s2;
  short* ps1 = (short*) s1 + 2; short* ps2 = (short*) s2 + 2;
  return *pi1 == *pi2 && *ps1 == *ps2;
}

unsigned VirtualHashTable::find_order(ull number)
{
  unsigned res = 0;
  while ( number != 0 )
  {
    res++;
    number /= 10;
  }
  return res;
}
