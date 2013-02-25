#include "shift_hash_table.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

int main (int argc, char const* argv[])
{
  ShiftHashTable* sht = new ShiftHashTable(2000, 60);
  fstream in("../base.txt", fstream::in);
  string key;
  long val2;
  string val1;
  Item it, *pit;
  for(int i = 0; i < 1000000; ++i)
  {
    in >> key; in >> val2; in >> val1;
    it = {key, val1, val2};
    sht->add_record(it);
  }
  in.close();
  fprintf(stderr, "in packages: %u, overflowed: %u\n",
          sht->count_in_packages, sht->count_in_overflow);
  fprintf(stderr, "start testing...\n");
  in.open("../base.txt", fstream::in);
  for(int i = 0; i < 1000000; ++i)
  {
    in >> key; in >> val2; in >> val1;
    pit = sht->find_record(key);
    if ( i % 10000 == 0 )
      fprintf(stderr, ".");
    if ( pit == NULL || pit->field1.compare(val1) != 0 || pit->field2 != val2 )
    {
      fprintf(stderr, "NOT EQUALS!!!!!\n");
      exit(1);
    }
  }
  return 0;
}
