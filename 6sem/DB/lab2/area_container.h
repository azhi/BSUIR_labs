#ifndef __AREA_CONTAINER_H_
#define __AREA_CONTAINER_H_

#include <vector>

#include "items.h"
#include "area.h"

using namespace std;

template<class Tk, class Tf>
class AreaContainer
{
  public:
    AreaContainer(int area_size);
    AreaContainer(int area_count, int area_size, int interspace_length);

    Item<Tk, Tf> *find_item(Tk key);
    void add_item(Item<Tk, Tf> &item);

    void add_area(Area<Tk, Tf> *area);

  private:
    int area_count, area_size;
    vector< Area<Tk, Tf> *> area_index;
};

#endif // __AREA_CONTAINER_H_
