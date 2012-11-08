#include "BigNum.h"

#include <math.h>
#include <sstream>
#include <stdexcept>
#include <iostream>

BigNum::BigNum(long base) : base(base) {}

BigNum::BigNum(long base, vector<long> nums) : base(base)
{
  set_numbers(nums);
}

BigNum::~BigNum()
{
}

BigNum BigNum::operator+(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<long> res_numbers;
  vector<long> o_numbers = other.get_numbers();
  long j = 0; char k = 0;
  long n = max(numbers.size(), o_numbers.size());
  long m_size = numbers.size();
  long o_size = o_numbers.size();

  while ( j < n )
  {
    long s1 = j < m_size ? numbers[j] : 0;
    long s2 = j < o_size ? o_numbers[j] : 0;
    res_numbers.push_back(s1 + s2 + k);
    if ( res_numbers[j] >= base )
    {
      res_numbers[j] -= base;
      k = 1;
    }
    else
      k = 0;
    ++j;
  }
  res_numbers.push_back(k);
  BigNum res(base, res_numbers);
  res.trim_zeroes();
  return res;
}

BigNum BigNum::operator*(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<long> o_numbers = other.get_numbers();
  long m = numbers.size();
  long n = o_numbers.size();

  vector<long> res_numbers;
  for(long i = 0; i < m + n; ++i)
    res_numbers.push_back(0);

  long j = 0;
  while ( j < n )
  {
    if ( o_numbers[j] > 0 )
    {
      long i = 0;
      long k = 0;
      while ( i < m )
      {
        long t = o_numbers[j] * numbers[i] + res_numbers[i + j] + k;
        res_numbers[i + j] = t % base;
        k = t / base;
        ++i;
      }
      res_numbers[j + m] = k;
    }
    else
      res_numbers[j + m] = 0;
    ++j;
  }

  BigNum res(base, res_numbers);
  res.trim_zeroes();
  return res;
}

BigNum BigNum::operator/(BigNum& other)
{

}

BigNum BigNum::karatsubaMultiply(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  trim_zeroes(); other.trim_zeroes();
  vector<long> o_numbers = other.get_numbers();
  if ( numbers.size() == 1 && o_numbers.size() == 1 )
    if ( numbers[0] < 4294967296 && o_numbers[0] < 4294967296 )
    {
      vector<long> res_numbers;
      res_numbers.push_back(numbers[0] * o_numbers[0]);
      BigNum res(base, res_numbers);
      return res;
    }

  vector<long> u1, u0, v1, v0;
  // TODO: find a way to calculate the first even power of two, that greater
  // than this BigNum
}

void BigNum::trim_zeroes()
{
  vector<long>::iterator it = numbers.end() - 1;
  while ( (*it) == 0 )
    --it;
  numbers.erase(it + 1, numbers.end());
}

string BigNum::to_string()
{
  stringstream ss;
  vector<long>::reverse_iterator it = numbers.rbegin();
  while ( it != numbers.rend() )
  {
    ss << (*it) << " ";
    ++it;
  }
  string res = ss.str();
  return res;
}
