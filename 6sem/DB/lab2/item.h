#ifndef __ITEMS_H_
#define __ITEMS_H_

template<class Tk, class Tf>
struct Item
{
  Tk key;
  Tf field;
};

template<class Tk, class Tf>
int item_comparer(const Item<Tk, Tf> item1, const Item<Tk, Tf> item2);

template<char*, class Tf>
int item_comparer(const Item<char*, Tf> &item1, const Item<char*, Tf> &item2)
{
  return strcmp(item1.key, item2.key);
}

template<long, class Tf>
int item_comparer(const Item<long, Tf> item1, const Item<long, Tf> item2)
{
  if ( item1.key == item2.key )
    return 0;
  else if ( item1.key < item2.key )
    return -1;
  else
    return 1;
}

#endif // __ITEMS_H_
