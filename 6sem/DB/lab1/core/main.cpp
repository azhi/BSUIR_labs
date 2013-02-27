#include "shift_hash_table.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

#define RECORDS_COUNT 1000000
#define SPACE_COUNT 1200000
#define KEYS_COUNT 1000200

int main (int argc, char const* argv[])
{
  unsigned package_count = atoi(argv[1]);
  ShiftHashTable* sht = new ShiftHashTable(package_count, SPACE_COUNT / package_count);
  fstream in("../base.txt", fstream::in);
  string key;
  long val2;
  string val1;
  Item *pit = new Item;
  for(int i = 0; i < RECORDS_COUNT; ++i)
  {
    in >> key; in >> val2; in >> val1;
    pit->key = key; pit->field1 = val1; pit->field2 = val2;
    sht->add_record(*pit);
  }
  delete pit;
  in.close();

  in.open("../keys.txt", fstream::in);
  clock_t t1 = clock();
  for(int i = 0; i < KEYS_COUNT; ++i)
  {
    in >> key;
    pit = sht->find_record(key);
  }
  clock_t t2 = clock();
  in.close();

  fstream out;
  char path[1000];
  sprintf(path, "%d_results.txt", package_count);
  out.open(path, fstream::out);
  out << sht->count_in_packages / (double) SPACE_COUNT << " ";
  out << sht->count_in_overflow / (double) sht->count_in_packages << " ";
  out << (t2 - t1) / (double) CLOCKS_PER_SEC << endl;
  out.close();

  sprintf(path, "%d_distr.txt", package_count);
  out.open(path, fstream::out);
  for ( int i = 0; i < pow(10, sht->package_count_order); ++i )
    out << i << " " << sht->distr[i] << endl;
  out.close();
  return 0;
}
