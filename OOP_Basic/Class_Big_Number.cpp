#include <iostream>
#include <time.h>
#include <stdlib.h>

typedef unsigned char BASE;

class Big_Number {
    int len; // Разряд
    int maxlen; // Выделенная память
    BASE *coef;
public:
    Big_Number(int ml = 1, int pr = 0); // Конструктор по умолчанию/параметрам
    Big_Number(const Big_Number& bn); // Конструктор копирования
    Big_Number operator = (const Big_Number& bn ); // Перегрузка оператора присвоения
    ~Big_Number(); // Деструктор


};


// Конструктор по умолчанию/параметрам
Big_Number::Big_Number(int ml, int p) {
    if(ml <= 1) { // Проверка параметра максимальной длины
        this->maxlen = 1; // Инициализация количества байт для числа
        this->len = 1; // Инициализация количества цифр числа
        
        coef = new BASE[1]; //Выделение динамической памяти
        if (coef) { // Проверка выделения памяти
            coef[0] = 0; //Инциализация числа нулем
        }
        else { // Иначе выход из программы
            exit(1); // Выход из программы
        }
    }
    else { // Максимальное количество байт > 0
        this->maxlen = ml; // Инициализация количества байт для числа
        this->len = ml; // Инициализация количества цифр числа
        coef = new BASE[ml]; //Выделение динамической памяти
        if (coef) { // Проверка выделения памяти
            srand(time(NULL)); // Инициализация генератора псевдослучайных чисел
            if (p == 0) {
                for (int i = 0; i < ml; i++)
                    coef = 0;
            }
            else {
                for (int i = 0; i < ml; i++)
                    coef[i] = rand() % 256;
            }
        }
    }
}