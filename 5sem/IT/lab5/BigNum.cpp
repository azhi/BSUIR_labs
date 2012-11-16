#include "BigNum.h"

#include <math.h>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <time.h>

BigNum::BigNum(long long base) : base(base) {}

BigNum::BigNum(long long base, vector<long long> nums) : base(base)
{
  set_numbers(nums);
}

BigNum::BigNum(long long base, string str_nums) : base(base)
{
  string::iterator sym = str_nums.begin();
  vector<long long> rev_nums;
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

vector<long long>& BigNum::add(vector<long long>& numbers, IPos begin, IPos end, vector<long long>& o_numbers, IPos o_begin, IPos o_end, long long base)
{
  vector<long long>* res_numbers = new vector<long long>;
  long j = 0; char k = 0;
  IPos m_pos = begin;
  IPos o_pos = o_begin;
  long n = max(end - begin, o_end - o_begin);

  while ( j < n )
  {
    long long s1 = m_pos < end ? (*(m_pos++)) : 0;
    long long s2 = o_pos < o_end ? (*(o_pos++)) : 0;
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

vector<long long>& BigNum::sub(vector<long long>& numbers, IPos begin, IPos end, vector<long long>& o_numbers, IPos o_begin, IPos o_end, long long base)
{
  vector<long long>* res_numbers = new vector<long long>;
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

vector<long long>& BigNum::mul(vector<long long>& numbers, IPos begin, IPos end, vector<long long>& o_numbers, IPos o_begin, IPos o_end, long long base)
{
  long start_i = begin - numbers.begin();
  long start_j = o_begin - o_numbers.begin();
  long m = end - begin;
  long n = o_end - o_begin;

  vector<long long>* res_numbers = new vector<long long>;
  for(long i = 0; i < m + n; ++i)
    res_numbers->push_back(0);

  long j = start_j;
  while ( j < start_j + n )
  {
    if ( o_numbers[j] > 0 )
    {
      long i = start_i;
      long long k = 0;
      while ( i < start_i + m )
      {
        long res_index = i + j - start_i - start_j;
        long long t = o_numbers[j] * numbers[i] + (*res_numbers)[res_index] + k;
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

void BigNum::divmod(vector<long long>& numbers, vector<long long>& o_numbers, vector<long long>& quo, vector<long long>& rem, long long base)
{
  long n = o_numbers.size();
  long m = numbers.size() - n;

  vector<long long> vec_d;
  vec_d.push_back(base / ( *(o_numbers.end() - 1) + 1 ));

  vector<long long> u = full_mul(numbers, vec_d, base);
  vector<long long> v = full_mul(o_numbers, vec_d, base);

  vector<long long> bn1;
  bn1.push_back(1);
  lshift(&bn1, n+1);

  quo.reserve(m+1);
  for ( int i = 0; i < m+1; ++i )
    quo.push_back(0);
  long j = m;
  while ( j >= 0 )
  {
    long long qs = (u[j + n] * base + u[j + n - 1]) / v[n - 1];
    long long rs = (u[j + n] * base + u[j + n - 1]) % v[n - 1];
    while ( rs < base )
      if ( qs == base || qs * v[n - 2] > base * rs + u[j + n - 2] )
      {
        qs -= 1;
        rs += v[n - 1];
      }
      else
        break;
    vector<long long> vec_qs;
    vec_qs.push_back(qs);

    vector<long long> part_u;
    vector<long long> summand = full_mul(v, vec_qs, base);
    part_u = add(u, u.begin() + j, u.begin() + j + n + 1, bn1, bn1.begin(), bn1.end(), base);
    part_u = full_sub(part_u, summand, base);
    trim_num_zeroes(part_u);

    quo[j] = qs;
    bool neg_flag;
    if ( part_u.size() > n + 1 )
    {
      neg_flag = false;
      part_u = full_sub(part_u, bn1, base);
    }
    else
      neg_flag = true;

    if ( neg_flag )
    {
      --quo[j];
      part_u = full_add(part_u, v, base);
    }
    for ( int i = j; i < j + n; ++i )
      u[i] = part_u[i - j];
    --j;
  }

  for ( long j = m + n - 1; j > n - 1; --j )
    u[j] = 0;

  long long chk;
  rem.reserve(m + n);
  for ( long i = 0; i < n; ++i )
    rem.push_back(0);
  short_divmod(u, vec_d[0], rem, &chk, base);
  if ( chk != 0 )
    throw invalid_argument("error while scaling back to 1/d");
}

void BigNum::short_divmod(vector<long long>& numbers, long long short_num, vector<long long>& quo, long long* rem, long long base)
{
  trim_num_zeroes(numbers);
  long n = numbers.size();

  long j = n - 1;
  long long div = 0;
  while ( j > -1 )
  {
    div += numbers[j];
    if ( div < short_num )
      quo[j] = 0;
    else
    {
      quo[j] = div / short_num;
      div %= short_num;
    }
    div *= base;
    --j;
  }

  if ( rem != NULL )
    (*rem) = div / base;
}

vector<long long>& BigNum::kmul(vector<long long>& numbers, IPos begin, IPos end, vector<long long>& o_numbers, IPos o_begin, IPos o_end, long long base)
{
  if ( (end - begin) <= 1 && (o_end - o_begin) <= 1 )
  {
    vector<long long>* res_numbers = new vector<long long>;
    long long m1 = (end - begin) == 1 ? (*begin) : 0;
    long long m2 = (o_end - o_begin) == 1 ? (*o_begin) : 0;
    res_numbers->push_back(m1 * m2);

    long long div = (*res_numbers)[0] / base;
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
  IPos middle, o_middle;
  split_number(begin, end, numbers, &middle, count);
  split_number(o_begin, o_end, o_numbers, &o_middle, count);

  clock_t t1 = clock();
  vector<long long> u1mv1 = kmul(numbers, middle, end, o_numbers, o_middle, o_end, base);
  clock_t t2 = clock();
  vector<long long> u0mv0 = kmul(numbers, begin, middle, o_numbers, o_begin, o_middle, base);
  clock_t t3 = clock();
  vector<long long> u1au0 = add(numbers, middle, end, numbers, begin, middle, base);
  trim_num_zeroes(u1au0);
  vector<long long> v1av0 = add(o_numbers, o_middle, o_end, o_numbers, o_begin, o_middle, base);
  trim_num_zeroes(v1av0);
  clock_t t4 = clock();
  vector<long long> umv = full_kmul(u1au0, v1av0, base);
  clock_t t5 = clock();

  vector<long long> sum2 = full_sub(umv, u0mv0, base);
  sum2 = full_sub(sum2, u1mv1, base);
  clock_t t6 = clock();

  lshift(&u1mv1, count * 2);
  lshift(&sum2, count);

  vector<long long>& res = full_add(u1mv1, sum2, base);
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

void BigNum::lshift(vector<long long>* numbers, long count)
{
  vector<long long> zeroes;
  zeroes.reserve(count);
  for ( long i = 0; i < count; ++i )
    zeroes.push_back(0);
  numbers->insert(numbers->begin(), zeroes.begin(), zeroes.end());
}

void BigNum::split_number(IPos begin, IPos end, vector<long long>& src, IPos* middle, long count)
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

  vector<long long> o_numbers = other.get_numbers();
  vector<long long>& res_numbers = full_add(numbers, o_numbers, base);
  BigNum* res = new BigNum(base, res_numbers);
  res->trim_zeroes();
  delete (&res_numbers);
  return (*res);
}

BigNum& BigNum::operator-(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<long long> o_numbers = other.get_numbers();
  vector<long long>& res_numbers = full_sub(numbers, o_numbers, base);
  BigNum* res = new BigNum(base, res_numbers);
  res->trim_zeroes();
  delete (&res_numbers);
  return (*res);
}

BigNum& BigNum::operator*(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<long long> o_numbers = other.get_numbers();
  vector<long long>& res_numbers = full_mul(numbers, o_numbers, base);
  BigNum* res = new BigNum(base, res_numbers);
  res->trim_zeroes();
  delete (&res_numbers);
  return (*res);
}

BigNum& BigNum::operator/(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<long long> o_numbers = other.get_numbers();
  if ( numbers.size() < o_numbers.size() )
  {
    vector<long long> q;
    q.push_back(0);
    BigNum* big_q = new BigNum(base, q);
    return (*big_q);
  }

  vector<long long> q;
  vector<long long> r;
  full_div(numbers, o_numbers, q, r, base);

  BigNum* big_q = new BigNum(base, q);
  big_q->trim_zeroes();
  return (*big_q);
}

BigNum& BigNum::operator%(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  vector<long long> o_numbers = other.get_numbers();
  if ( numbers.size() < o_numbers.size() )
    return (*this);

  vector<long long> q;
  vector<long long> r;
  full_div(numbers, o_numbers, q, r, base);

  BigNum* big_r = new BigNum(base, r);
  big_r->trim_zeroes();
  return (*big_r);
}

BigNum& BigNum::karatsubaMultiply(BigNum& other)
{
  if ( base != other.get_base() )
    throw invalid_argument("Both operands should be in one base");

  trim_zeroes(); other.trim_zeroes();
  vector<long long> o_numbers = other.get_numbers();
  vector<long long>& res_numbers = full_kmul(numbers, o_numbers, base);
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

void BigNum::trim_num_zeroes(vector<long long>& nums)
{
  vector<long long>::iterator it = nums.end() - 1;
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
  vector<long long> quo;
  quo.insert(quo.begin(), numbers.begin(), numbers.end());
  long long rem;

  short_divmod(quo, 10, quo, &rem, base);
  trim_num_zeroes(quo);
  while ( quo[0] != 0 || quo.size() > 1 )
  {
    ss << rem;
    short_divmod(quo, 10, quo, &rem, base);
    trim_num_zeroes(quo);
  }
  ss << rem;
  string rev_res = ss.str();
  string res;
  string::reverse_iterator sym;
  for ( sym = rev_res.rbegin(); sym < rev_res.rend(); sym++ )
    res += (*sym);
  return res;
}
