#include <iostream>

#include "BigNum.h"

using namespace std;

int main(int argc, const char *argv[])
{
  vector<long> nums;
  nums.push_back(5);
  nums.push_back(1);
  nums.push_back(5);
  nums.push_back(5);
  nums.push_back(6);
  nums.push_back(2);
  nums.push_back(7);
  nums.push_back(9);
  nums.push_back(1);
  BigNum bn1(10, nums);

  nums.clear();
  nums.push_back(6);
  nums.push_back(5);
  nums.push_back(2);
  nums.push_back(9);
  nums.push_back(4);
  nums.push_back(3);
  nums.push_back(7);
  nums.push_back(2);
  nums.push_back(5);
  nums.push_back(8);
  nums.push_back(4);
  BigNum bn2(10, nums);

  BigNum res = bn1 + bn2;
  cout << res.to_string() << endl;

  return 0;
}
