#include "area_container.h"

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#define SORT_RECORDS_COUNT 800000
#define UNSORT_RECORDS_COUNT 200000

#define INTERSPACE_LENGTH 100
#define EFFECTIVE_INTERSPACE_LENGTH 90
#define AREA_SIZE 40

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
  Item<char *, char *> *pit = new Item<char *, char *>;
  vector< Item<char *, char *> >* items = new vector< Item<char *, char *> >;
  vector< Interspace<char *, char *> *>* interspaces = new vector< Interspace<char *, char *> *>;
  list< Area<char *, char *> >* areas = new list< Area<char *, char *> >;
  int items_count = 0, interspaces_count = 0;

  for(int i = 0; i < SORT_RECORDS_COUNT; ++i) {
    in >> key >> val2 >> val1;

    strcpy(pit->key, val1);
    strcpy(pit->field, key);
    items->push_back(*pit);
    items_count++;

    if ( items_count == EFFECTIVE_INTERSPACE_LENGTH )
    {
      if ( interspaces_count % 10 == 9 )
      {
        interspaces->push_back(new Interspace<char *, char *>(INTERSPACE_LENGTH));
        interspaces_count++;
      }
      interspaces->push_back(new Interspace<char *, char *>(INTERSPACE_LENGTH, items));
      interspaces_count++;
      items->clear();
      items_count = 0;
    }

    if ( interspaces_count == AREA_SIZE )
    {
      areas->push_back(Area<char *, char *>(AREA_SIZE, interspaces));
      interspaces->clear();
      interspaces_count = 0;
    }
  }
  AreaContainer<char *, char *> ac(areas);
  delete pit;
  in.close();

  cerr << "File " << "base1.txt" << " loaded." << endl;
  return 0;
}
