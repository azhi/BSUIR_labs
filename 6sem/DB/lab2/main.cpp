#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>

#include "area_container.h"

using namespace std;

#define SORT_RECORDS_COUNT 800000
#define UNSORT_RECORDS_COUNT 200000

#define INTERSPACE_LENGTH 100
#define EFFECTIVE_INTERSPACE_LENGTH 90
#define AREA_SIZE 40

#define KEY_TYPE string

template<class Tk, class Tf>
void save_sample(const AreaContainer<Tk, Tf> &ac);

int main(int argc, char **argv)
{
  ifstream in("base2.txt");
  if (!in.is_open())
  {
    cerr << "Can't open file base2.txt" << endl;
    return 1;
  }
  char key[7];
  char val1[13];
  long val2;
  Item<KEY_TYPE, string> *pit = new Item<KEY_TYPE, string>;
  vector< Item<KEY_TYPE, string> >* items = new vector< Item<KEY_TYPE, string> >;
  vector< Interspace<KEY_TYPE, string> *>* interspaces = new vector< Interspace<KEY_TYPE, string> *>;
  list< Area<KEY_TYPE, string> >* areas = new list< Area<KEY_TYPE, string> >;
  int items_count = 0, interspaces_count = 0;

  for(int i = 0; i < SORT_RECORDS_COUNT; ++i) {
    in >> key >> val2 >> val1;

    pit->key = val1;
    pit->field = key;
    items->push_back(*pit);
    items_count++;

    if ( items_count == EFFECTIVE_INTERSPACE_LENGTH )
    {
      if ( interspaces_count % 10 == 9 )
      {
        interspaces->push_back(new Interspace<KEY_TYPE, string>(INTERSPACE_LENGTH));
        interspaces_count++;
      }

      if ( interspaces_count == AREA_SIZE )
      {
        areas->push_back(Area<KEY_TYPE, string>(AREA_SIZE, interspaces));
        interspaces = new vector< Interspace<KEY_TYPE, string> *>;
        interspaces_count = 0;
      }

      interspaces->push_back(new Interspace<KEY_TYPE, string>(INTERSPACE_LENGTH, items));
      interspaces_count++;
      items = new vector< Item<KEY_TYPE, string> >;
      items_count = 0;
    }
  }
  interspaces->push_back(new Interspace<KEY_TYPE, string>(INTERSPACE_LENGTH, items));
  areas->push_back(Area<KEY_TYPE, string>(AREA_SIZE, interspaces));
  AreaContainer<KEY_TYPE, string> ac(areas);

  printf("%d %lu\n", items_count, areas->size());
  in.close();
  cerr << "File " << "base2.txt" << " loaded." << endl;

  in.open("base_tail.txt");
  if (!in.is_open())
  {
    cerr << "Can't open file base_tail.txt" << endl;
    return 1;
  }

  for(int i = 0; i < UNSORT_RECORDS_COUNT; ++i) {
    in >> key >> val2 >> val1;

    pit->key = val1;
    pit->field = key;
    try {
      ac.add_item(*pit);
    } catch (invalid_argument* ex)
    {
      cerr << ex->what() << endl;
      throw;
    }
  }
  in.close();
  cerr << "File " << "base_tail.txt" << " loaded." << endl;

  printf("%u\n", ac.get_size());
  delete pit;
  save_sample(ac);
}

template<class Tk, class Tf>
void save_sample(const AreaContainer<Tk, Tf> &ac)
{
  // Sample method, never used.
  ofstream ofs("./test.bin");
  boost::archive::binary_oarchive oa(ofs);
  oa.register_type< Item<Tk, Tf> >();
  oa.register_type< Interspace<Tk, Tf> >();
  oa.register_type< Area<Tk, Tf> >();
  oa.register_type< AreaContainer<Tk, Tf> >();
  oa << ac;
  ofs.close();
}
