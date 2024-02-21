#include <iostream>
#include <time.h>
#include <stdlib.h>

typedef unsigned char BASE;

using namespace std;
class Big_Number {
    int len; // Разряд
    int maxlen; // Выделенная память
    BASE *coef;
public:
    Big_Number(int ml = 1, int pr = 0); // Конструктор по умолчанию/параметрам
    Big_Number(const Big_Number& bn); // Конструктор копирования
    ~Big_Number() { if (coef) delete[] coef; coef = NULL; len = 0; maxlen = 0; };; // Деструктор
    Big_Number operator = (const Big_Number& bn ); // Перегрузка оператора присвоения

    void input_hex(); // Ввод большого числа в 16-ом виде
    void output_hex(); // Вывод большого числа в 16-ом виде

    bool operator == (const Big_Number& bn); // Перегрузка оператора ==
    bool operator != (const Big_Number& bn); // Перегрузка оператора !=
    bool operator < (const Big_Number& bn); // Перегрузка оператора <
    bool operator <= (const Big_Number& bn); // Перегрузка оператора <=
    bool operator > (const Big_Number& bn); // Перегрузка оператора >
    bool operator >= (const Big_Number& bn);  // Перегрузка оператора >=
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
            if (p == 0) { // Если параметр 0, то заполняем все цифры 0
                for (int i = 0; i < ml; i++)
                    coef[i] = 0;
            }
            else { // Иначе заполняем рандомными цифрами
                for (int i = 0; i < ml; i++)
                    coef[i] = rand() % 256;
            }
        }
    }
}

// Конструктор копирования
Big_Number::Big_Number(const Big_Number& bn) {
    len = bn.len; // Копирование значений
    maxlen = bn.maxlen; // Копирование значений
    coef = new BASE[maxlen]; // Выделение динамической памяти для массива цифр
    if (coef) { // Проверка выделения динамической памяти
        for (int i = 0; i < bn.maxlen; i++) {
            coef[i] = bn.coef[i]; // Копирование цифр
        }
    }
}

// Перегрузка оператора =
Big_Number Big_Number::operator=(const Big_Number& bn) {
    if (&bn == this) // Проверка самоприсвоения
        return *this; // Возвращение самого объекта
    
    len = bn.len; // Копирование значений
    maxlen = bn.maxlen; // Копирование значений

    if (coef) // Проверка на NULL
        delete coef; // Освобождение динамической памяти
    coef = NULL; // Присвоение NULL
    coef = new BASE[maxlen]; // Выделение динамической памяти
    if (coef) { // Проверка выделения памяти
        for (int i = 0; i < bn.maxlen; i++)
            coef[i] = bn.coef[i]; // Копирование значений
    }

    return *this; // Возвращение самого объекта
}

bool Big_Number::operator == (const Big_Number& bn) {
    if (this == &bn) {
        return true;
    }

    for (size_t i = len - 1; i >= 0; i--) {
        if (coef[i] != bn.coef[i])
            return false;
    }
    return true;
}


bool Big_Number::operator != (const Big_Number& bn) {
    if (this == &bn) {
        return false;
    }
    for (size_t i = len - 1; i >= 0; i--) {
        if (coef[i] != bn.coef[i])
            return false;
    }
    return true;
}