#ifndef __AREA_H_
#define __AREA_H_

#include <vector>

#include "items.h"

using namespace std;

template<class Tk, class Tf>
class Area
{
  public:
    Area(int size, Tk max_key);
    Area(int size, int vector< Interspace<Tk, Tf> *> *index, Tk max_key);

    Item<Tk, Tf> *find_item(Tk key);
    void add_item(Item<Tk, Tf> &item);

    void add_interspace(Interspace<Tk, Tf> *interspace);

  private:
    int size, interspace_length;
    Tk max_key;
    vector< Interspace<Tk, Tf> *> *interspace_index;
};

#endif // __AREA_H_
