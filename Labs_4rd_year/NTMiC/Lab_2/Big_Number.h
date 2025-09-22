#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <algorithm>
#include <vector>

typedef unsigned int BASE;
typedef unsigned long long DBASE;
#define BASE_SIZE (sizeof(BASE) * 8)

class Big_Number
{
    BASE *digits;
    int length;
    int maxlen;

public:
    Big_Number(unsigned long long num = 0ULL, int alloc_size = 1);
    Big_Number(const Big_Number &);
    ~Big_Number();
    
    Big_Number operator=(const Big_Number &);
    bool operator==(const Big_Number &) const;
    bool operator<=(const Big_Number &) const;
    bool operator>=(const Big_Number &) const;
    bool operator<(const Big_Number &) const;
    bool operator>(const Big_Number &) const;
    bool operator!=(const Big_Number &) const;

    Big_Number operator+(const Big_Number &) const;
    Big_Number &operator+=(const Big_Number &);

    Big_Number operator-(const Big_Number &) const;
    Big_Number &operator-=(const Big_Number &);

    Big_Number operator*(const BASE &) const;
    Big_Number &operator*=(const BASE &);

    Big_Number operator*(const Big_Number &) const;
    Big_Number &operator*=(const Big_Number &);

    Big_Number operator/(const BASE &) const;
    Big_Number operator%(const BASE &) const;

    Big_Number &operator/=(const BASE &);
    Big_Number &operator%=(const BASE &);

    Big_Number operator/(const Big_Number &) const;
    Big_Number operator%(const Big_Number &) const;

    Big_Number &operator/=(const Big_Number &);
    Big_Number &operator%=(const Big_Number &);

    void cout_10();
    void cin_10();
    
    friend std::ostream &operator<<(std::ostream &, const Big_Number &);
    friend std::istream &operator>>(std::istream &, Big_Number &);

    Big_Number pow(const Big_Number& exponent) const; 
    Big_Number pow_mod (const Big_Number& exponent, const Big_Number& modulus) const; 
    Big_Number sqrt() const; 

    static Big_Number random(int bits); 
    static Big_Number random_range(const Big_Number& min, const Big_Number& max); 

    bool isPrime(int iterations = 2) const;

    std::vector<Big_Number> factorize() const;
    Big_Number method_Olvea();
    Big_Number nth_sqrt(const Big_Number& n) const;
};

#endif 