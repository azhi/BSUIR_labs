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

#define AREA_SIZE 40

#if TYPE == 0
#define KEY_TYPE string
#else
#define KEY_TYPE long
#endif

template<class Tk, class Tf>
void save_sample(const AreaContainer<Tk, Tf> &ac);

int main(int argc, char **argv)
{
  int interspace_length = atoi(argv[1]);
  int effective_interspace_length = round(0.9 * interspace_length);
  #if TYPE == 0
    ifstream in("base2.txt");
  #else
    ifstream in("base1.txt");
  #endif
  if (!in.is_open())
  {
    cerr << "Can't open file base" << endl;
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

    #if TYPE == 0
      pit->key = val1;
    #else
      pit->key = val2;
    #endif
    pit->field = key;
    items->push_back(*pit);
    items_count++;

    if ( items_count == effective_interspace_length )
    {
      if ( interspaces_count % 10 == 9 )
      {
        interspaces->push_back(new Interspace<KEY_TYPE, string>(interspace_length));
        interspaces_count++;
      }

      if ( interspaces_count == AREA_SIZE )
      {
        areas->push_back(Area<KEY_TYPE, string>(AREA_SIZE, interspaces));
        interspaces = new vector< Interspace<KEY_TYPE, string> *>;
        interspaces_count = 0;
      }

      interspaces->push_back(new Interspace<KEY_TYPE, string>(interspace_length, items));
      interspaces_count++;
      items = new vector< Item<KEY_TYPE, string> >;
      items_count = 0;
    }
  }
  interspaces->push_back(new Interspace<KEY_TYPE, string>(interspace_length, items));
  areas->push_back(Area<KEY_TYPE, string>(AREA_SIZE, interspaces));
  AreaContainer<KEY_TYPE, string> ac(areas);

  in.close();
  #if TYPE == 0
    cerr << "File " << "base2.txt" << " loaded." << endl;
  #else
    cerr << "File " << "base1.txt" << " loaded." << endl;
  #endif

  cerr << "Areas count before: " << ac.get_size() << endl;

  ofstream ofs("./before.json");
  ofs << ac.to_json();
  ofs.close();

  in.open("base_tail.txt");
  if (!in.is_open())
  {
    cerr << "Can't open file base_tail.txt" << endl;
    return 1;
  }

  for(int i = 0; i < UNSORT_RECORDS_COUNT; ++i) {
    in >> key >> val2 >> val1;

    #if TYPE == 0
      pit->key = val1;
    #else
      pit->key = val2;
    #endif
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
  cerr << "Areas count after: " << ac.get_size() << endl;

  delete pit;

  ofs.open("./after.json");
  ofs << ac.to_json();
  ofs.close();

  if ( argc > 2 )
  {
    vector< Item<KEY_TYPE, string> *> *res;
    #if TYPE == 0
      res = ac.find_item(argv[2]);
    #else
      res = ac.find_item(atol(argv[2]));
    #endif
    if ( res == nullptr )
      cerr << "No records found" << endl;
    else
    {
      vector< Item<KEY_TYPE, string> *>::iterator it;
      for( it = res->begin(); it < res->end(); ++it )
      #if TYPE == 0
        printf("%s %s\n", (*it)->key.c_str(), (*it)->field.c_str());
      #else
        printf("%ld %s\n", (*it)->key, (*it)->field.c_str());
      #endif
    }
  }

  return 0;
}

template<class Tk, class Tf>
void save_sample(const AreaContainer<Tk, Tf> &ac)
{
  ofstream ofs("./test.bin");
  boost::archive::binary_oarchive oa(ofs);
  oa.register_type< Item<Tk, Tf> >();
  oa.register_type< Interspace<Tk, Tf> >();
  oa.register_type< Area<Tk, Tf> >();
  oa.register_type< AreaContainer<Tk, Tf> >();
  oa << ac;
  ofs.close();
}
