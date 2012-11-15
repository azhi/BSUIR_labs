#include "BigNum.h"

#include <math.h>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <time.h>

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

vector<ulong>& BigNum::add(vector<ulong>& numbers, IPos begin, IPos end, vector<ulong>& o_numbers, IPos o_begin, IPos o_end, ulong base)
{
  vector<ulong>* res_numbers = new vector<ulong>;
  long j = 0; char k = 0;
  IPos m_pos = begin;
  IPos o_pos = o_begin;
  long n = max(end - begin, o_end - o_begin);

  while ( j < n )
  {
    ulong s1 = m_pos < end ? (*(m_pos++)) : 0;
    ulong s2 = o_pos < o_end ? (*(o_pos++)) : 0;
    res_numbers->push_back(s1 + s2 + k);
    if ( (*res_numbers)[j] >= base )
    {
      (*res_numbers)[j] -= base;
      k = 1;
    }
    else
      k = 0;
    ++j;
  }
  res_numbers->push_back(k);
  return (*res_numbers);
}

vector<ulong>& BigNum::sub(vector<ulong>& numbers, IPos begin, IPos end, vector<ulong>& o_numbers, IPos o_begin, IPos o_end, ulong base)
{
  vector<ulong>* res_numbers = new vector<ulong>;
  long j = 0; char k = 0;
  IPos m_pos = begin;
  IPos o_pos = o_begin;
  long n = max(end - begin, o_end - o_begin);

  while ( j < n )
  {
    long long s1 = m_pos < end ? (*(m_pos++)) : 0;
    long long s2 = o_pos < o_end ? (*(o_pos++)) : 0;
    long long num = s1 - s2 - k;
    if ( num < 0 )
    {
      num += base;
      k = 1;
    }
    else
      k = 0;
    res_numbers->push_back(num);
    ++j;
  }
  
  if ( k == 1 )
    throw invalid_argument("First operand should be greater than the second one");

  return (*res_numbers);
}

vector<ulong>& BigNum::mul(vector<ulong>& numbers, IPos begin, IPos end, vector<ulong>& o_numbers, IPos o_begin, IPos o_end, ulong base)
{
  long start_i = begin - numbers.begin();
  long start_j = o_begin - o_numbers.begin();
  long m = end - begin;
  long n = o_end - o_begin;

  vector<ulong>* res_numbers = new vector<ulong>;
  for(long i = 0; i < m + n; ++i)
    res_numbers->push_back(0);

  long j = start_j;
  while ( j < start_j + n )
  {
    if ( o_numbers[j] > 0 )
    {
      long i = start_i;
      ulong k = 0;
      while ( i < start_i + m )
      {
        long res_index = i + j - start_i - start_j;
        ulong t = o_numbers[j] * numbers[i] + (*res_numbers)[res_index] + k;
        (*res_numbers)[res_index] = t % base;
        k = t / base;
        ++i;
      }
      (*res_numbers)[j + m - start_j] = k;
    }
    else
      (*res_numbers)[j + m - start_j] = 0;
    ++j;
  }

  return (*res_numbers);
}

vector<ulong>& BigNum::div(vector<ulong>& numbers, IPos begin, IPos end, vector<ulong>& o_numbers, IPos o_begin, IPos o_end, ulong base)
{

}

vector<ulong>& BigNum::kmul(vector<ulong>& numbers, IPos begin, IPos end, vector<ulong>& o_numbers, IPos o_begin, IPos o_end, ulong base)
{
  // printf("%d - %d * %d - %d\n", *begin, *(end - 1), *o_begin, *(o_end - 1));
  if ( (end - begin) <= 1 && (o_end - o_begin) <= 1 )
  {
    vector<ulong>* res_numbers = new vector<ulong>;
    ulong m1 = (end - begin) == 1 ? (*begin) : 0;
    ulong m2 = (o_end - o_begin) == 1 ? (*o_begin) : 0;
    res_numbers->push_back(m1 * m2);

    ulong div = (*res_numbers)[0] / base;
    long i = 0;
    while ( div != 0 )
    {
      (*res_numbers)[i] %= base;
      res_numbers->push_back(div);
      ++i;
      div = (*res_numbers)[i] / base;
    }

    return (*res_numbers);
  }

  long size = max(end - begin, o_end - o_begin);
  long count = size / 2;
  if ( size % 2 == 1 )
    count++;
  // printf("size1 %d, size2 %d\n", end-begin, o_end-o_begin);
  // printf("size %d, count %d\n", size, count);
  IPos middle, o_middle;
  split_number(begin, end, numbers, &middle, count);
  split_number(o_begin, o_end, o_numbers, &o_middle, count);

  clock_t t1 = clock();
  vector<ulong> u1mv1 = kmul(numbers, middle, end, o_numbers, o_middle, o_end, base);
  clock_t t2 = clock();
  vector<ulong> u0mv0 = kmul(numbers, begin, middle, o_numbers, o_begin, o_middle, base);
  clock_t t3 = clock();
  vector<ulong> u1au0 = add(numbers, middle, end, numbers, begin, middle, base);
  trim_num_zeroes(u1au0);
  vector<ulong> v1av0 = add(o_numbers, o_middle, o_end, o_numbers, o_begin, o_middle, base);
  trim_num_zeroes(v1av0);
  clock_t t4 = clock();
  vector<ulong> umv = full_kmul(u1au0, v1av0, base);
  clock_t t5 = clock();

  vector<ulong> sum2 = full_sub(umv, u0mv0, base);
  sum2 = full_sub(sum2, u1mv1, base);
  clock_t t6 = clock();

  lshift(&u1mv1, count * 2);
  lshift(&sum2, count);

  vector<ulong>& res = full_add(u1mv1, sum2, base);
  res = full_add(res, u0mv0, base);
  trim_num_zeroes(res);
  // if ( t2 - t1 > 0 )
  //   cout << "1-2: " << t2 - t1 << endl;
  // if ( t3 - t2 > 0 )
  //   cout << "2-3: " << t3 - t2 << endl;
  // if ( t4 - t3 > 0 )
  //   cout << "3-4: " << t4 - t3 << endl;
  // if ( t5 - t4 > 0 )
  //   cout << "4-5: " << t5 - t4 << endl;
  // if ( t6 - t5 > 0 )
  //   cout << "5-6: " << t6 - t5 << endl;
  return res;
}

void BigNum::lshift(vector<ulong>* numbers, long count)
{
  vector<ulong> zeroes;
  zeroes.reserve(count);
  for ( long i = 0; i < count; ++i )
    zeroes.push_back(0);
  numbers->insert(numbers->begin(), zeroes.begin(), zeroes.end());
}
void BigNum::split_number(IPos begin, IPos end, vector<ulong>& src, IPos* middle, long count)
{
  if ( end - begin <= count )
    (*middle) = end;
  else
    (*middle) = begin + count;
}

BigNum& BigNum::operator+(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<ulong> o_numbers = other.get_numbers();
  vector<ulong>& res_numbers = full_add(numbers, o_numbers, base);
  BigNum* res = new BigNum(base, res_numbers);
  res->trim_zeroes();
  delete (&res_numbers);
  return (*res);
}

BigNum& BigNum::operator-(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<ulong> o_numbers = other.get_numbers();
  vector<ulong>& res_numbers = full_sub(numbers, o_numbers, base);
  BigNum* res = new BigNum(base, res_numbers);
  res->trim_zeroes();
  delete (&res_numbers);
  return (*res);
}

BigNum& BigNum::operator*(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<ulong> o_numbers = other.get_numbers();
  vector<ulong>& res_numbers = full_mul(numbers, o_numbers, base);
  BigNum* res = new BigNum(base, res_numbers);
  res->trim_zeroes();
  delete (&res_numbers);
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
  vector<ulong>& res_numbers = full_kmul(numbers, o_numbers, base);
  BigNum* res = new BigNum(base, res_numbers);
  res->trim_zeroes();
  delete (&res_numbers);
  return (*res);
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

void BigNum::trim_num_zeroes(vector<ulong>& nums)
{
  vector<ulong>::iterator it = nums.end() - 1;
  while ( (*it) == 0 )
    --it;
  nums.erase(it + 1, nums.end());
  if ( nums.size() == 0 )
    nums.push_back(0);
}

void BigNum::trim_zeroes()
{
  trim_num_zeroes(numbers);
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
