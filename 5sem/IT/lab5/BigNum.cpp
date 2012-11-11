#include "BigNum.h"

#include <math.h>
#include <sstream>
#include <stdexcept>
#include <iostream>

BigNum::BigNum(ulong base) : base(base) {}

BigNum::BigNum(ulong base, vector<ulong> nums) : base(base)
{
  set_numbers(nums);
}

BigNum::BigNum(ulong base, string str_nums) : base(base)
{
  string::iterator sym = str_nums.begin();
  vector<ulong> rev_nums;
  string num;
  while ( sym != str_nums.end() )
  {
    num.clear();
    while ( (*sym) == ' ' && sym != str_nums.end() )
      ++sym;
    while ( (*sym) != ' ' && sym != str_nums.end() )
    {
      num += (*sym);
      ++sym;
    }
    if ( num.size() != 0 )
      rev_nums.push_back( strtoul(num.c_str(), (char**) NULL, 10) );
  }
  set_rev_numbers(rev_nums);
}

BigNum::~BigNum()
{
}

BigNum& BigNum::operator+(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<ulong> res_numbers;
  vector<ulong> o_numbers = other.get_numbers();
  long j = 0; char k = 0;
  long n = max(numbers.size(), o_numbers.size());
  long m_size = numbers.size();
  long o_size = o_numbers.size();

  while ( j < n )
  {
    ulong s1 = j < m_size ? numbers[j] : 0;
    ulong s2 = j < o_size ? o_numbers[j] : 0;
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
  BigNum* res = new BigNum(base, res_numbers);
  res->trim_zeroes();
  return (*res);
}

BigNum& BigNum::operator-(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<ulong> res_numbers;
  vector<ulong> o_numbers = other.get_numbers();
  long j = 0; char k = 0;
  long n = max(numbers.size(), o_numbers.size());
  long m_size = numbers.size();
  long o_size = o_numbers.size();

  while ( j < n )
  {
    long long s1 = j < m_size ? numbers[j] : 0;
    long long s2 = j < o_size ? o_numbers[j] : 0;
    long long num = s1 - s2 - k;
    if ( num < 0 )
    {
      num += base;
      k = 1;
    }
    else
      k = 0;
    res_numbers.push_back(num);
    ++j;
  }
  
  if ( k == 1 )
    throw invalid_argument("Fisrt operand should be greater than the second one");
  BigNum* res = new BigNum(base, res_numbers);
  res->trim_zeroes();
  return (*res);
}

BigNum& BigNum::operator*(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<ulong> o_numbers = other.get_numbers();
  long m = numbers.size();
  long n = o_numbers.size();

  vector<ulong> res_numbers;
  for(long i = 0; i < m + n; ++i)
    res_numbers.push_back(0);

  long j = 0;
  while ( j < n )
  {
    if ( o_numbers[j] > 0 )
    {
      long i = 0;
      ulong k = 0;
      while ( i < m )
      {
        ulong t = o_numbers[j] * numbers[i] + res_numbers[i + j] + k;
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

  BigNum* res = new BigNum(base, res_numbers);
  res->trim_zeroes();
  return (*res);
}

BigNum& BigNum::operator/(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<ulong> o_numbers = other.get_numbers();
  long n = o_numbers.size();
  long m = numbers.size() - n;

  vector<ulong> vec_d;
  vec_d.push_back(base / ( *(o_numbers.end() - 1) + 1 ));

  BigNum d(base, vec_d);
  BigNum big_u = (*this) * d;
  BigNum big_v = other * d;

  vector<ulong> u = big_u.get_numbers();
  vector<ulong> v = big_v.get_numbers();

  vector<ulong> q;
  long j = m;
  while ( j >= 0 )
  {
    ulong qs = (u[j + n] * base + u[j + n - 1]) / v[n - 1];
    ulong rs = (u[j + n] * base + u[j + n - 1]) % v[n - 1];
    if ( qs == base && qs * v[n - 2] > base * rs + u[j + n - 2] )
    {
      qs -= 1;
      rs += v[n - 1];
    }
    if ( rs < base && qs * v[n - 2] > base * rs + u[j + n - 2] )
    {
      qs -= 1;
      rs += v[n - 1];
    }
  }
}

BigNum& BigNum::karatsubaMultiply(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  trim_zeroes(); other.trim_zeroes();
  vector<ulong> o_numbers = other.get_numbers();
  if ( numbers.size() == 1 && o_numbers.size() == 1 )
    if ( numbers[0] < 4294967296 && o_numbers[0] < 4294967296 )
    {
      vector<ulong> res_numbers;
      res_numbers.push_back(numbers[0] * o_numbers[0]);

      ulong div = res_numbers[0] / base;
      long i = 0;
      while ( div != 0 )
      {
        res_numbers[i] %= base;
        res_numbers.push_back(div);
        ++i;
        div = res_numbers[i] / base;
      }

      BigNum* res = new BigNum(base, res_numbers);
      return (*res);
    }

  vector<ulong> v_u1, v_u0, v_v1, v_v0;
  long size = max(numbers.size(), o_numbers.size());
  if ( size % 2 == 1 && size != 1 )
    ++size;
  split_number(numbers, v_u1, v_u0, size);
  split_number(o_numbers, v_v1, v_v0, size);

  BigNum u1(base, v_u1);
  BigNum u0(base, v_u0);
  BigNum v1(base, v_v1);
  BigNum v0(base, v_v0);

  vector<ulong> vec_base;
  vec_base.push_back(0);
  vec_base.push_back(1);
  BigNum bp(base, vec_base);
  bp = bp.pow(size / 2);
  BigNum bp2 = bp * bp;

  BigNum& res = u1.karatsubaMultiply(v1) * bp2 + ((u1 + u0).karatsubaMultiply(v1 + v0) - u0.karatsubaMultiply(v0) - u1.karatsubaMultiply(v1)) * bp + u0.karatsubaMultiply(v0);
  return res;
}

void BigNum::split_number(vector<ulong>& src, vector<ulong>& hi_part, vector<ulong>& lo_part, long size)
{
  if ( size == 1 )
  {
    hi_part.push_back( (src[0] >> 32) & 0xFFFF );
    lo_part.push_back( src[0] & 0xFFFF );
  }
  else
  {
    while ( src.size() != size )
      src.push_back(0);
    long middle = src.size() / 2;
    lo_part.insert(hi_part.begin(), src.begin(), src.begin() + middle);
    hi_part.insert(hi_part.begin(), src.begin() + middle, src.end());
  }
}

BigNum& BigNum::pow(long power)
{
  if ( power == 1 )
    return (*this);
  else if ( power % 2 == 0 )
  {
    BigNum res = pow(power / 2);
    return res * res;
  }
  else
  {
    BigNum res = pow(power / 2);
    return res * res * (*this);
  }
}

void BigNum::trim_zeroes()
{
  vector<ulong>::iterator it = numbers.end() - 1;
  while ( (*it) == 0 )
    --it;
  numbers.erase(it + 1, numbers.end());
  if ( numbers.size() == 0 )
    numbers.push_back(0);
}

string BigNum::to_string()
{
  stringstream ss;
  vector<ulong>::reverse_iterator it = numbers.rbegin();
  while ( it != numbers.rend() )
  {
    ss << (*it) << " ";
    ++it;
  }
  string res = ss.str();
  return res;
}
