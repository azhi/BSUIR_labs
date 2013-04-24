#ifndef __AREA_H_
#define __AREA_H_

#include <vector>

#include "interspace.h"

using namespace std;

template<class Tk, class Tf>
class Area
{
  public:
    Area(Tk max_key);
    Area(int size, vector< Interspace<Tk, Tf> > *index);

    Item<Tk, Tf> *find_item(Tk key);
    bool add_item(Item<Tk, Tf> &item);
    Area<Tk, Tf> *divide();

    Tk get_max_key();

    void add_interspace(Interspace<Tk, Tf> *interspace);

  private:
    int size, interspace_length;
    Tk max_key;
    Item<Tk, Tf> max_key_item;
    vector< Interspace<Tk, Tf> > *interspace_index;

    typedef Interspace<Tk, Tf> TInterspace;
    typedef typename vector<TInterspace>::iterator interspace_iterator;

    interspace_iterator get_middle_iterator();

    static int compare(TInterspace int1, TInterspace int2);
};

template<class Tk, class Tf>
Area<Tk, Tf>::Area(Tk max_key) : size(0), interspace_length(0),
  interspace_index(0), max_key(max_key)
{}

template<class Tk, class Tf>
Area<Tk, Tf>::Area(int size, vector< Interspace<Tk, Tf> > *index) :
  size(size)
{
  interspace_index = index;
  interspace_iterator max_key_interspace = max_element(interspace_index->begin(), interspace_index->end(),
                                               compare);
  max_key = max_key_interspace->get_max_key();
  max_key_item.key = max_key;
  max_key_item.field = Tf();
}

template<class Tk, class Tf>
Tk Area<Tk, Tf>::get_max_key()
{
  return max_key;
}

template<class Tk, class Tf>
Item<Tk, Tf> *Area<Tk, Tf>::find_item(Tk key)
{
  if (max_key_item.compare_to(key) > 0)
    return nullptr;

  TInterspace key_interspace(key);
  interspace_iterator lb = lower_bound(interspace_index->first(), interspace_index->last(), key_interspace,
      compare);

  return lb->find_item(key);
}

template<class Tk, class Tf>
bool Area<Tk, Tf>::add_item(Item<Tk, Tf> &item)
{
  if (item_comparer<Tk, Tf>(max_key_item, item) > 0)
    throw new invalid_argument("key is larger than max_key");

  TInterspace key_interspace(item.key);
  interspace_iterator ub = upper_bound(interspace_index->first(), interspace_index->last(), key_interspace,
      compare);
  bool res = ub->add_item(item);
  if ( !res )
    if ( interspace_index->size() < size )
    {
      TInterspace* spl_interspace = ub->divide();
      add_interspace(spl_interspace);
      // WARNING: potential never-ending recursion!
      res = add_item(item);
      return res;
    }
    else
      return false;
  else
    return true;
}

template<class Tk, class Tf>
Area<Tk, Tf> *Area<Tk, Tf>::divide()
{
  // Copy [size/2 .. last] elements to a new vector.
  vector< Interspace<Tk, Tf> > *o_interspaces =
    new vector< Interspace<Tk, Tf> >(interspace_index->begin() + size / 2, interspace_index->end());
  o_interspaces->reserve(size);

  // Clean the interspace vector up.
  interspace_index->erase(interspace_index->begin() + size / 2, interspace_index->end());

  TInterspace max_key_interspace = max_element(interspace_index->begin(), interspace_index->end(),
                                               compare);
  max_key = max_key_interspace.get_max_key();
  max_key_item.key = max_key;
  max_key_item.field = Tf();

  return new Area(size, o_interspaces);
}

template<class Tk, class Tf>
void Area<Tk, Tf>::add_interspace(Interspace<Tk, Tf> *interspace)
{
  if (interspace_index->size() > size - 1)
    throw new invalid_argument("attempt to add interspace when no space left");
  interspace_iterator ub = upper_bound(interspace_index->first(), interspace_index->last(), *interspace,
      compare);
  interspace_index->insert(ub, *interspace);
}

template<class Tk, class Tf>
int Area<Tk, Tf>::compare(TInterspace int1, TInterspace int2)
{
  Tk k1 = int1.get_max_key();
  Tk k2 = int2.get_max_key();
  if ( k1 == k2 )
    return 0;
  else if ( k1 < k2 )
    return -1;
  else
    return 1;
}

#endif // __AREA_H_
