#ifndef __ITEMS_H_
#define __ITEMS_H_

#include <cstring>

template<class Tk, class Tf>
struct Item
{
  Tk key;
  Tf field;

  int compare_to(Tk key);
};

template<class Tf>
struct Item<char *, Tf>
{
  char* key;
  Tf field;

  int compare_to(char* key)
  {
    return strcmp(this->key, key);
  }
};

template<class Tf>
struct Item<long, Tf>
{
  long key;
  Tf field;

  int compare_to(long key)
  {
    if ( this->key == key )
      return 0;
    else if ( this->key < key )
      return -1;
    else
      return 1;
  }
};

template<class Tk, class Tf>
int item_comparer(Item<Tk, Tf> &item1, Item<Tk, Tf> &item2)
{
  return item1.compare_to(item2.key);
}

#endif // __ITEMS_H_
