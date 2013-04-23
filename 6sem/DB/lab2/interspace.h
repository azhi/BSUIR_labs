#ifndef __INTERSPACE_H_
#define __INTERSPACE_H_

#include <vector>
#include <algorithm>
#include <stdexcept>

#include "item.h"

using namespace std;

template<class Tk, class Tf>
class Interspace
{
  public:
    Interspace(int length, const Tk max_key);
    Interspace(int length, Item<Tk, Tf> *items);

    Item<Tk, Tf> *find_item(Tk key);
    void add_item(Item<Tk, Tf>& item);
    Interspace<Tk, Tf> *divide();

  private:
    int length;
    Item<Tk, Tf> max_key_item;
    Tk max_key;
    bool free;
    vector< Item<Tk, Tf> > *items;

    typedef Item<Tk, Tf> TItem;
    typedef typename vector<TItem>::iterator items_iterator;
};

template<class Tk, class Tf>
Interspace<Tk, Tf>::Interspace(int length, const Tk max_key)
  : length(length), max_key(max_key)
{
  items = new vector< Item<Tk ,Tf> >();
  items->reserve(length);
  max_key_item = { max_key, Tf() };
  free = true;
}

template<class Tk, class Tf>
Interspace<Tk, Tf>::Interspace(int length, Item<Tk, Tf> *items)
  : length(length), items(items)
{
  max_key_item = max_element(items->begin(), items->end(),
                             item_comparer<Tk, Tf>);
  max_key = max_key_item.key;
  free = false;
}

template<class Tk, class Tf>
Item<Tk, Tf> *Interspace<Tk, Tf>::find_item(Tk key)
{
  if (free)
    return nullptr;

  TItem key_item = {key, 0};
  if (item_comparer<Tk, Tf>(max_key_item, key_item) > 0)
    return nullptr;

  items_iterator lb = lower_bound(items->first(), items->last(), key_item,
      item_comparer<Tk, Tf>);

  if (lb != items->last() && item_comparer<Tk, Tf>(key_item, *lb) == 0)
    return &(*lb);
  else
    return nullptr;
}

template<class Tk, class Tf>
void Interspace<Tk, Tf>::add_item(Item<Tk, Tf>& item)
{
  if (item_comparer<Tk, Tf>(max_key_item, item) > 0)
    throw new invalid_argument("key is larger than max_key");

  free = false;
  items_iterator ub = upper_bound(items->first(), items->last(), item,
      item_comparer<Tk, Tf>);
  items->insert(ub, item);
}

template<class Tk, class Tf>
Interspace<Tk, Tf> *Interspace<Tk, Tf>::divide()
{
  // Assert interspace to be full.

  int size = items->size();

  // Copy [size/2 .. last] elements to a new vector.
  vector< Item<Tk, Tf> > *o_items =
    new vector< Item<Tk, Tf> >(items->begin() + size/2, items->end());
  o_items->reserve(length);

  // Clean the interspace vector up.
  items->resize(size/2);
  items->shrink_to_fit();
  items->reserve(size);
  max_key_item = max_element(items->begin(), items->end(),
                             item_comparer<Tk, Tf>);
  max_key = max_key_item.key;

  return Interspace(length, o_items);
}

#endif // __INTERSPACE_H_
