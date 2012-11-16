#ifndef BIGNUM_H
#define BIGNUM_H

#include <vector>
#include <string>

using namespace std;

typedef vector<long long>::iterator IPos;

class BigNum
{
  public:
    BigNum(long long base);
    BigNum(long long base, vector<long long> nums);
    BigNum(long long base, string str_nums);
    virtual ~BigNum();

    BigNum& operator+(BigNum& other);
    BigNum& operator-(BigNum& other);
    BigNum& operator*(BigNum& other);
    BigNum& operator/(BigNum& other);
    BigNum& operator%(BigNum& other);
    BigNum& karatsubaMultiply(BigNum& other);
    BigNum& pow(long power);

    static vector<long long>& add(vector<long long>& numbers, IPos begin, IPos end, vector<long long>& o_numbers, IPos o_begin, IPos o_end, long long base);
    static vector<long long>& sub(vector<long long>& numbers, IPos begin, IPos end, vector<long long>& o_numbers, IPos o_begin, IPos o_end, long long base);
    static vector<long long>& mul(vector<long long>& numbers, IPos begin, IPos end, vector<long long>& o_numbers, IPos o_begin, IPos o_end, long long base);
    static void divmod(vector<long long>& numbers, vector<long long>& o_numbers, vector<long long>& quo, vector<long long>& rem, long long base);
    static void short_divmod(vector<long long>& numbers, long long short_num, vector<long long>& quo, long long* rem, long long base);
    static vector<long long>& kmul(vector<long long>& numbers, IPos begin, IPos end, vector<long long>& o_numbers, IPos o_begin, IPos o_end, long long base);
    static void lshift(vector<long long>* numbers, long count);
    static void split_number(IPos begin, IPos end, vector<long long>& src, IPos* middle, long count);
    static void trim_num_zeroes(vector<long long>& nums);

    #define full_add(n, o_n, base)\
      add(n, n.begin(), n.end(), o_n, o_n.begin(), o_n.end(), base);
    #define full_sub(n, o_n, base)\
      sub(n, n.begin(), n.end(), o_n, o_n.begin(), o_n.end(), base);
    #define full_mul(n, o_n, base)\
      mul(n, n.begin(), n.end(), o_n, o_n.begin(), o_n.end(), base);
    #define full_div(n, o_n, quo, rem, base)\
      divmod(n, o_n, quo, rem, base);
    #define full_kmul(n, o_n, base)\
      kmul(n, n.begin(), n.end(), o_n, o_n.begin(), o_n.end(), base);

    string to_string();
    vector<long long> get_numbers() { return numbers; };
    void set_numbers(vector<long long>& nums) { numbers.insert(numbers.begin(), nums.begin(), nums.end()); };
    void set_rev_numbers(vector<long long>& rev_nums) { numbers.insert(numbers.begin(), rev_nums.rbegin(), rev_nums.rend()); };
    long long get_base() { return base; };

    void trim_zeroes();

  private:
    long long base;
		vector<long long> numbers;

};

#endif // BIGNUM_H
