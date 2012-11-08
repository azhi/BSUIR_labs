#ifndef BIGNUM_H
#define BIGNUM_H

#include <vector>
#include <string>

using namespace std;

class BigNum
{
  public:
    BigNum(long base);
    BigNum(long base, vector<long> nums);
    virtual ~BigNum();

    BigNum operator+(BigNum& other);
    BigNum operator*(BigNum& other);
    BigNum operator/(BigNum& other);
    BigNum karatsubaMultiply(BigNum& other);

    string to_string();
    vector<long> get_numbers() { return numbers; };
    void set_numbers(vector<long> nums) { numbers.insert(numbers.begin(), nums.begin(), nums.end()); };
    long get_base() { return base; };

    void trim_zeroes();

  private:
    long base;
		vector<long> numbers;
};

#endif // BIGNUM_H
