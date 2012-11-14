#include <iostream>

#include "BigNum.h"

using namespace std;

int main(int argc, const char *argv[])
{
  string str_nums = "5 1 5 5 6 2 7 9 1 7 8 9 6 5 4 1 2 3 5 4 7 8 9 6 5 4 1 2 3 6 5 4 7 8 9 6 5 4 1 2 3 7 8 9 0 7 6 5 4 3 3 2 2 2 4 5 6 7 8 9 0 9 8 7 6 5 4 3 1 2 3 4 5";
  BigNum bn1(10, str_nums);

  str_nums = "7 6 5 4 8 9 0 5 4 1 2 3 7 6 7 8 9 0 3 2 6 7 8 9 4 2 5 7 7 6 5 5 4 4 3 8 9 0 9 8 7 6 5 5 4 1 9 8 1 2 3 4 5 6 7 8 4 5 2 1 5 6 3 2 1 4 5 7 8 9 6 5 4 1 2 6 5 2 9 4 3 7 2 5 8 4 8 9 6 5 1 2 3 4 7 8 9 5 2 1 3 4 7 8 9 6 5 2 1";
  BigNum bn2(10, str_nums);

  BigNum res = bn1 * bn2;
  cout << res.to_string() << endl;

  res = bn1.karatsubaMultiply(bn2);
  cout << res.to_string() << endl;

  return 0;
}
