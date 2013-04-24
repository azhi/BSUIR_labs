#include "area_container.h"

int main(int argc, char **argv)
{
  list< Area<char const *, char *> >* ari1 = new list< Area<char const *, char *> >;
  for( int i = 0; i < 100; ++i )
  {
    vector< Interspace<char const *, char *> >* index1 = new vector< Interspace<char const *, char *> >;
    for( int j = 0; j < 100; ++j )
      index1->push_back(Interspace<char const *, char *>(50, "aaaa"));
    ari1->push_back(Area<char const *, char *>(200, index1));
  }
  AreaContainer<char const *, char *> ar1(ari1);

  list< Area<long, char *> >* ari2 = new list< Area<long, char *> >;
  for( int i = 0; i < 100; ++i )
  {
    vector< Interspace<long, char *> >* index2 = new vector< Interspace<long, char *> >;
    for( int j = 0; j < 100; ++j )
      index2->push_back(Interspace<long, char *>(50, 2000));
    ari2->push_back(Area<long, char *>(200, index2));
  }
  AreaContainer<long, char *> ar2(ari2);

  return 0;
}
