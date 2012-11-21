#include <iostream>
#include <fstream>

#include <time.h>

#include "BigNum.h"

using namespace std;

int main(int argc, const char *argv[])
{
  string str_nums;
  char buf[10240];
  fstream fs("in.txt");
  long base;
  fs >> base;
  fs.getline(buf, 10240);
  fs.getline(buf, 10240);
  str_nums = buf;
  BigNum bn1(base, str_nums);
  fs.getline(buf, 10240);
  str_nums = buf;
  BigNum bn2(base, str_nums);
  fs.close();

  BigNum res(base);
  clock_t t1;
  clock_t t2;

  t1 = clock();
  res = bn1 + bn2;
  t2 = clock();
  cout << "add: " << res.to_string() << endl;
  cout << "time=" << (t2 - t1) / 1000 << "ms" << endl;
  cout << endl;

  t1 = clock();
  res = bn1 * bn2;
  t2 = clock();
  cout << "mul: " << res.to_string() << endl;
  cout << "time=" << (t2 - t1) / 1000 << "ms" << endl;
  cout << endl;

  t1 = clock();
  res = bn1.karatsubaMultiply(bn2);
  t2 = clock();
  cout << "karatsuba mul: " << res.to_string() << endl;
  cout << "time=" << (t2 - t1) / 1000 << "ms" << endl;
  cout << endl;

  t1 = clock();
  res = bn1 / bn2;
  t2 = clock();
  cout << "div: " << res.to_string() << endl;
  cout << "time=" << (t2 - t1) / 1000 << "ms" << endl;
  cout << endl;

  t1 = clock();
  res = bn1 % bn2;
  t2 = clock();
  cout << "mod: " << res.to_string() << endl;
  cout << "time=" << (t2 - t1) / 1000 << "ms" << endl;

  return 0;
}
