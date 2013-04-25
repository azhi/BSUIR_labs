#include <iostream>
#include <fstream>
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

template<class Tk, class Tf>
void save_sample(const AreaContainer<Tk, Tf> &ac);

int main(int argc, char **argv)
{
  ifstream in("base1.txt");
  if (!in.is_open())
  {
    cerr << "Can't open file base1.txt" << endl;
    return 1;
  }
  char key[7];
  char val1[13];
  long val2;
  Item<string, string> *pit = new Item<string, string>;
  vector< Item<string, string> >* items = new vector< Item<string, string> >;
  vector< Interspace<string, string> *>* interspaces = new vector< Interspace<string, string> *>;
  list< Area<string, string> >* areas = new list< Area<string, string> >;
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
        interspaces->push_back(new Interspace<string, string>(INTERSPACE_LENGTH));
        interspaces_count++;
      }

      if ( interspaces_count == AREA_SIZE )
      {
        areas->push_back(Area<string, string>(AREA_SIZE, interspaces));
        interspaces->clear();
        interspaces_count = 0;
      } else {
        interspaces->push_back(new Interspace<string, string>(INTERSPACE_LENGTH, items));
        interspaces_count++;
        items->clear();
        items_count = 0;
      }
    }
  }
  AreaContainer<string, string> ac(areas);
  save_sample(ac);
  delete pit;
  in.close();

  cerr << "File " << "base1.txt" << " loaded." << endl;
  return 0;
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
