#ifndef BIGNUM_H
#define BIGNUM_H

#include <vector>
#include <string>

using namespace std;

typedef unsigned long ulong;
typedef vector<ulong>::iterator IPos;

class BigNum
{
  public:
    BigNum(ulong base);
    BigNum(ulong base, vector<ulong> nums);
    BigNum(ulong base, string str_nums);
    virtual ~BigNum();

    BigNum& operator+(BigNum& other);
    BigNum& operator-(BigNum& other);
    BigNum& operator*(BigNum& other);
    BigNum& operator/(BigNum& other);
    BigNum& karatsubaMultiply(BigNum& other);
    BigNum& pow(long power);

    static vector<ulong>& add(vector<ulong>& numbers, IPos begin, IPos end, vector<ulong>& o_numbers, IPos o_begin, IPos o_end, ulong base);
    static vector<ulong>& sub(vector<ulong>& numbers, IPos begin, IPos end, vector<ulong>& o_numbers, IPos o_begin, IPos o_end, ulong base);
    static vector<ulong>& mul(vector<ulong>& numbers, IPos begin, IPos end, vector<ulong>& o_numbers, IPos o_begin, IPos o_end, ulong base);
    static vector<ulong>& div(vector<ulong>& numbers, IPos begin, IPos end, vector<ulong>& o_numbers, IPos o_begin, IPos o_end, ulong base);
    static vector<ulong>& kmul(vector<ulong>& numbers, IPos begin, IPos end, vector<ulong>& o_numbers, IPos o_begin, IPos o_end, ulong base);
    static void lshift(vector<ulong>* numbers, long count);
    static void split_number(IPos begin, IPos end, vector<ulong>& src, IPos* middle, long count);
    static void trim_num_zeroes(vector<ulong>& nums);

    #define full_add(n, o_n, base)\
      add(n, n.begin(), n.end(), o_n, o_n.begin(), o_n.end(), base);
    #define full_sub(n, o_n, base)\
      sub(n, n.begin(), n.end(), o_n, o_n.begin(), o_n.end(), base);
    #define full_mul(n, o_n, base)\
      mul(n, n.begin(), n.end(), o_n, o_n.begin(), o_n.end(), base);
    #define full_div(n, o_n, base)\
      div(n, n.begin(), n.end(), o_n, o_n.begin(), o_n.end(), base);
    #define full_kmul(n, o_n, base)\
      kmul(n, n.begin(), n.end(), o_n, o_n.begin(), o_n.end(), base);

    string to_string();
    vector<ulong> get_numbers() { return numbers; };
    void set_numbers(vector<ulong>& nums) { numbers.insert(numbers.begin(), nums.begin(), nums.end()); };
    void set_rev_numbers(vector<ulong>& rev_nums) { numbers.insert(numbers.begin(), rev_nums.rbegin(), rev_nums.rend()); };
    ulong get_base() { return base; };

    void trim_zeroes();

  private:
    ulong base;
		vector<ulong> numbers;

};

#endif // BIGNUM_H
