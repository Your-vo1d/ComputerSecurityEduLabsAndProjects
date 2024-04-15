#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <iostream>
#include <random>
#include <limits>

typedef unsigned char BASE;
typedef unsigned short int DBASE;
#define BASE_SIZE (sizeof(BASE) * 8)

class Big_Number {
public:
    Big_Number(int ml = 1, int pr = 0);
    Big_Number(const Big_Number &bn);
    ~Big_Number();

    Big_Number operator=(const Big_Number &bn);
    bool operator==(const Big_Number &bn);
    bool operator!=(const Big_Number &bn);
    bool operator<(const Big_Number &bn);
    bool operator<=(const Big_Number &bn);
    bool operator>(const Big_Number &bn);
    bool operator>=(const Big_Number &bn);
    Big_Number operator+(const Big_Number &bn);
    Big_Number& operator+=(const Big_Number &bn);

    void input_hex();
    void output_hex();

private:
    int len;
    int maxlen;
    BASE *coef;
};

#endif // BIG_NUMBER_H