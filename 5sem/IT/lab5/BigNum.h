#ifndef BIGNUM_H
#define BIGNUM_H

#include <vector>
#include <string>

using namespace std;

typedef unsigned long ulong;

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

    BigNum& add(vector<ulong>& o_numbers, vector<ulong>::iterator first, vector<ulong>::iterator last);
    BigNum& sub(vector<ulong>& o_numbers, vector<ulong>::iterator first, vector<ulong>::iterator last);
    BigNum& mul(vector<ulong>& o_numbers, vector<ulong>::iterator first, vector<ulong>::iterator last);
    BigNum& div(vector<ulong>& o_numbers, vector<ulong>::iterator first, vector<ulong>::iterator last);

    string to_string();
    vector<ulong> get_numbers() { return numbers; };
    void set_numbers(vector<ulong>& nums) { numbers.insert(numbers.begin(), nums.begin(), nums.end()); };
    void set_rev_numbers(vector<ulong>& rev_nums) { numbers.insert(numbers.begin(), rev_nums.rbegin(), rev_nums.rend()); };
    ulong get_base() { return base; };

    void trim_zeroes();

  private:
    ulong base;
		vector<ulong> numbers;

    void split_number(vector<ulong>& src, vector<ulong>& hi_part, vector<ulong>& lo_part, long size);
};

#endif // BIGNUM_H
