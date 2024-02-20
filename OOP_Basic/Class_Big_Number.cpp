#include <iostream>


typedef unsigned char BASE;

class big_number {
    int len; //Разряд
    int maxlen; //Выделенная память
    BASE *coef;
public:
    big_number(int ml = 1, int num = 0);
    big_number(const big_number& bn);

};
