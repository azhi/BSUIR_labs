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
    Interspace(int length);
    Interspace(int length, vector< Item<Tk, Tf> > *items);

    Item<Tk, Tf> *find_item(Tk key);
    bool add_item(Item<Tk, Tf>& item);
    Interspace<Tk, Tf> *divide();

    Tk get_max_key();
    bool is_free();

  private:
    int length;
    Item<Tk, Tf> max_key_item;
    Tk max_key;
    bool free;
    vector< Item<Tk, Tf> > *items;

    typedef Item<Tk, Tf> TItem;
    typedef typename vector<TItem>::iterator items_iterator;

    items_iterator get_middle_iterator();
};

template<class Tk, class Tf>
Tk Interspace<Tk, Tf>::get_max_key()
{
  return max_key;
}

template<class Tk, class Tf>
bool Interspace<Tk, Tf>::is_free()
{
  return free;
}

template<class Tk, class Tf>
Interspace<Tk, Tf>::Interspace(int length)
  : length(length)
{
  items = new vector< Item<Tk ,Tf> >();
  items->reserve(length);
  max_key_item.key = Tk();
  max_key_item.field = Tf();
  free = true;
}

template<class Tk, class Tf>
Interspace<Tk, Tf>::Interspace(int length, vector< Item<Tk, Tf> > *items)
  : length(length), items(items)
{
  items_iterator max_key_it = max_element(items->begin(), items->end(),
                             item_comparer<Tk, Tf>);
  max_key_item = *max_key_it;
  max_key = max_key_item.key;
  free = false;
}

template<class Tk, class Tf>
Item<Tk, Tf> *Interspace<Tk, Tf>::find_item(Tk key)
{
  if (free)
    return nullptr;

  if (max_key_item.compare_to(key) > 0)
    return nullptr;

  TItem key_item = {key, 0};
  items_iterator lb = lower_bound(items->first(), items->last(), key_item,
      item_comparer<Tk, Tf>);

  if (lb != items->last() && item_comparer<Tk, Tf>(key_item, *lb) == 0)
    return &(*lb);
  else
    return nullptr;
}

template<class Tk, class Tf>
bool Interspace<Tk, Tf>::add_item(Item<Tk, Tf>& item)
{
  if (item_comparer<Tk, Tf>(max_key_item, item) > 0)
    throw new invalid_argument("key is larger than max_key");

  free = false;
  if ( items->size() >= length )
    return false;
  else
  {
    items_iterator ub = upper_bound(items->first(), items->last(), item,
        item_comparer<Tk, Tf>);
    items->insert(ub, item);
    return true;
  }
}

template<class Tk, class Tf>
Interspace<Tk, Tf> *Interspace<Tk, Tf>::divide()
{
  // Copy [size/2 .. last] elements to a new vector.
  items_iterator middle = get_middle_iterator(items);
  vector< Item<Tk, Tf> > *o_items =
    new vector< Item<Tk, Tf> >(middle, items->end());
  o_items->reserve(length);

  // Clean the interspace vector up.
  items->erase(middle, items->end());

  max_key_item = max_element(items->begin(), items->end(),
                             item_comparer<Tk, Tf>);
  max_key = max_key_item.key;

  return new Interspace(length, o_items);
}

template<class Tk, class Tf>
typename vector< Item<Tk, Tf> >::iterator Interspace<Tk, Tf>::get_middle_iterator()
{
  items_iterator middle = items->begin() + items->size() / 2;
  Tk key = middle->key;
  while (middle < items->end() && middle->compare_to(key) == 0)
    middle++;

  if ( middle == items->end() ) {
    middle = items->rbegin() + items->size() / 2;
    key = middle->key;
    while (middle < items->rend() && middle->compare_to(key) == 0)
      middle++;
  }

  return middle;
}

#endif // __INTERSPACE_H_
