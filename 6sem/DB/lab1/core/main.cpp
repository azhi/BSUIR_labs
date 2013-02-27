#include "digit_analyze_hash_table.h"
#include "shift_hash_table.h"

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define RECORDS_COUNT 1000000
#define SPACE_COUNT 1200000
#define KEYS_COUNT 1000200

int main (int argc, char const *argv[])
{
  unsigned package_count = atoi(argv[2]);
  VirtualHashTable *sht;
  char const *suffix;
  if (string(argv[1]) == "--shift") {
    suffix = "shift";
    sht = new ShiftHashTable(package_count, SPACE_COUNT / package_count);
  } else if(string(argv[1]) == "--dig") {
    suffix = "dig";
    sht = new DigitAnalyzeHashTable(package_count, SPACE_COUNT / package_count);
  } else
    throw("Wrong number of arguments.");

  fstream in("../base.txt", fstream::in);
  char key[7];
  long val2;
  char val1[13];
  Item *pit = new Item;
  for(int i = 0; i < RECORDS_COUNT; ++i)
  {
    in >> key; in >> val2; in >> val1;
    strcpy(pit->key, key); strcpy(pit->field1, val1); pit->field2 = val2;
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
  sprintf(path, "%d_%s_results.txt", package_count, suffix);
  out.open(path, fstream::out);
  out << sht->count_in_packages / (double) SPACE_COUNT << " ";
  out << sht->count_in_overflow / (double) sht->count_in_packages << " ";
  out << (t2 - t1) / (double) CLOCKS_PER_SEC << endl;
  out.close();
  delete sht;
  return 0;
}
