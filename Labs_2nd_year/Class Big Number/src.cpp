#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <algorithm>
typedef unsigned int BASE;
typedef unsigned long DBASE;
#define BASE_SIZE (sizeof(BASE) * 8);
using namespace std;

class Big_Number
{
    BASE *digits;
    int length;
    int maxlen;

public:
    Big_Number(int ml = 1, int d = 0);
    Big_Number(const Big_Number &);
    ~Big_Number()
    {
        if (digits)
            delete[] digits;
        digits = NULL;
    };
    Big_Number operator=(const Big_Number &);
    bool operator==(const Big_Number &);
    bool operator<=(const Big_Number &);
    bool operator>=(const Big_Number &);
    bool operator<(const Big_Number &);
    bool operator>(const Big_Number &);
    bool operator!=(const Big_Number &);

    Big_Number operator+(const Big_Number &);

    Big_Number &operator+=(const Big_Number &);

    Big_Number operator-(const Big_Number &);

    Big_Number &operator-=(const Big_Number &);

    Big_Number operator*(const BASE &);
    Big_Number &operator*=(const BASE &);

    Big_Number operator*(const Big_Number &);
    Big_Number &operator*=(const Big_Number &);

    Big_Number operator/(const BASE &);
    Big_Number operator%(const BASE &);

    Big_Number &operator/=(const BASE &);
    Big_Number &operator%=(const BASE &);

    Big_Number operator/(const Big_Number &);
    Big_Number operator%(const Big_Number &);

    Big_Number &operator/=(const Big_Number &);
    Big_Number &operator%=(const Big_Number &);

    void cout_10();
    void cin_10();
    friend ostream &operator<<(ostream &, const Big_Number &);
    friend istream &operator>>(istream &, Big_Number &);
};

Big_Number::Big_Number(int ml, int d)
{
    digits = new BASE[ml];
    int size = BASE_SIZE;
    maxlen = ml;
    length = ml;
    for (int i = 0; i < maxlen; i++)
        digits[i] = 0;
    if (d != 0)
    {
        for (int i = 0; i < maxlen; i++)
        {
            digits[i] = rand();
            if (size > 12)
                for (int j = 1; j < ceil(size / 12); j++)
                    digits[i] = digits[i] << (j * 12) | rand();
        }
        while (length > 1 && digits[length - 1] == 0)
            length--;
    }
}

Big_Number::Big_Number(const Big_Number &num)
{
    maxlen = num.maxlen;
    length = num.length;
    digits = new BASE[maxlen];
    for (int i = 0; i < maxlen; i++)
        digits[i] = num.digits[i];
}

Big_Number Big_Number::operator=(const Big_Number &num)
{
    if (this != &num)
    {
        delete[] digits;
        maxlen = num.maxlen;
        length = num.length;
        digits = new BASE[maxlen];
        for (int i = 0; i < maxlen; i++)
            digits[i] = num.digits[i];
    }
    return *this;
}

bool Big_Number::operator==(const Big_Number &num)
{
    if (length != num.length)
    {
        return false;
    }
    for (int i = 0; i < length; i++)
    {
        if (digits[i] != num.digits[i])
        {
            return false;
        }
    }
    return true;
}

bool Big_Number::operator!=(const Big_Number &d)
{
    if (length != d.length)
    {
        return true;
    }
    for (int i = 0; i < length; i++)
    {
        if (digits[i] != d.digits[i])
        {
            return true;
        }
    }
    return false;
}

bool Big_Number::operator<(const Big_Number &d)
{
    if (length < d.length)
    {
        return true;
    }
    else if (length > d.length)
    {
        return false;
    }
    for (int i = length - 1; i > -1; i--)
    {
        if (digits[i] > d.digits[i])
        {
            return false;
        }
        else if (digits[i] < d.digits[i])
        {
            return true;
        }
    }
    return false;
}

bool Big_Number::operator>(const Big_Number &d)
{
    if (length > d.length)
    {
        return true;
    }
    else if (length < d.length)
    {
        return false;
    }
    for (int i = length - 1; i > -1; i--)
    {
        if (digits[i] < d.digits[i])
        {
            return false;
        }
        else if (digits[i] > d.digits[i])
        {
            return true;
        }
    }
    return false;
}

bool Big_Number::operator>=(const Big_Number &d)
{
    if (length > d.length)
    {
        return true;
    }
    else if (length < d.length)
    {
        return false;
    }
    for (int i = length - 1; i > -1; i--)
    {
        if (digits[i] < d.digits[i])
        {
            return false;
        }
        else if (digits[i] > d.digits[i])
        {
            return true;
        }
    }
    return true;
}

bool Big_Number::operator<=(const Big_Number &d)
{
    if (length < d.length)
    {
        return true;
    }
    else if (length > d.length)
    {
        return false;
    }
    for (int i = length - 1; i > -1; i--)
    {
        if (digits[i] > d.digits[i])
        {
            return false;
        }
        else if (digits[i] < d.digits[i])
        {
            return true;
        }
    }
    return true;
}

Big_Number Big_Number::operator+(const Big_Number &bNum)
{
    int maxLength = std::max(length, bNum.length) + 1; // Максимальная длина результата
    int commonLength = std::min(length, bNum.length);  // Количество общих разрядов у чисел

    Big_Number resultNum(maxLength); // Результат сложения
    DBASE tempSum;                   // Временная сумма для хранения промежуточных значений

    DBASE carry = 0;        // Перенос между разрядами
    int index = 0;        // Индекс для прохода по общим разрядам
    int base = BASE_SIZE; // Размер базового типа

    while (index < commonLength)
    {
        // Суммируем текущие разряды и перенос
        tempSum = (DBASE)digits[index] + (DBASE)bNum.digits[index] + (DBASE)carry;
        resultNum.digits[index] = (BASE)tempSum;
        carry = (BASE)(tempSum >> base); // Проверяем наличие переноса в старших разрядах
        index++;
    }

    // Суммируем оставшиеся разряды первого числа с переносом
    while (index < length)
    {
        tempSum = (DBASE)digits[index] + (DBASE)carry;
        resultNum.digits[index] = (BASE)tempSum;
        carry = (BASE)(tempSum >> base);
        index++;
    }

    // Суммируем оставшиеся разряды второго числа с переносом
    while (index < bNum.length)
    {
        tempSum = (DBASE)bNum.digits[index] + (DBASE)carry;
        resultNum.digits[index] = tempSum;
        carry = (BASE)(tempSum >> base);
        index++;
    }
    // Последний разряд равен оставшемуся переносу
    resultNum.digits[index] = carry;
    // Обновляем длину результата
    resultNum.length = (carry == 0) ? index : index + 1;

    return resultNum;
}

Big_Number &Big_Number::operator+=(const Big_Number &bNum)
{
    int maxLength = std::max(length, bNum.length) + 1; // Максимальная длина результата
    int commonLength = std::min(length, bNum.length);  // Количество общих разрядов у чисел

    BASE *tempDigits = new BASE[maxLength]; // Временный массив для хранения промежуточных значений

    DBASE carry = 0;        // Перенос между разрядами
    int index = 0;        // Индекс для прохода по общим разрядам
    int base = BASE_SIZE; // Размер базового типа

    while (index < commonLength)
    {
        // Суммируем текущие разряды и перенос
        DBASE tempSum = (DBASE)digits[index] + (DBASE)bNum.digits[index] + (DBASE)carry;
        tempDigits[index] = (BASE)tempSum;
        carry = (BASE)(tempSum >> base); // Проверяем наличие переноса в старших разрядах
        index++;
    }

    // Суммируем оставшиеся разряды первого числа с переносом
    while (index < length)
    {
        DBASE tempSum = (DBASE)digits[index] + (DBASE)carry;
        tempDigits[index] = (BASE)tempSum;
        carry = (BASE)(tempSum >> base);
        index++;
    }

    // Суммируем оставшиеся разряды второго числа с переносом
    while (index < bNum.length)
    {
        DBASE tempSum = (DBASE)bNum.digits[index] + (DBASE)carry;
        tempDigits[index] = (BASE)tempSum;
        carry = (BASE)(tempSum >> base);
        index++;
    }
    // Последний разряд равен оставшемуся переносу
    tempDigits[index] = carry;

    // Освобождаем память из текущего массива digits
    delete[] digits;
    // Присваиваем новый массив временных значений текущему объекту
    digits = tempDigits;
    // Обновляем длину результата
    length = (carry == 0) ? index : index + 1;

    return *this;
}

Big_Number Big_Number::operator-(const Big_Number &subtracting)
{
    if (*this < subtracting) {
        throw invalid_argument("Invalid argument"); // Можно ли их отнять, чтобы не было отрицательного значения?
    }

    DBASE borrow = 0; // Коэффициент заимствования
    int base_size = BASE_SIZE;
    DBASE tmp_difference;
    int index = 0;

    Big_Number result(length);

    while (index < subtracting.length) {
        // Учитываем заимствование
        tmp_difference = ((DBASE)1 << base_size) | (DBASE)digits[index];
        tmp_difference = (DBASE)tmp_difference - (DBASE)subtracting.digits[index] - (DBASE)borrow;

        result.digits[index] = (BASE)tmp_difference; // Записываем значение разности
        borrow = (DBASE)!(tmp_difference >> base_size); // Вычисляем заимствование

        index++;
    }

    while (index < length) {
        // Забираем заимствование
        tmp_difference = ((DBASE)1 << base_size) | (DBASE)digits[index];
        tmp_difference -= (DBASE)borrow; // Вычитаем заимствование

        result.digits[index] = (BASE)tmp_difference;
        borrow = (DBASE)!(tmp_difference >> base_size);

        index++;
    }

    result.length = length;
    while (result.length > 1 && result.digits[result.length - 1] == 0) {
        // Приводим фактическую длину
        result.length--;
    }

    return result;
}

Big_Number& Big_Number::operator-=(const Big_Number &subtracting)
{
    if (*this < subtracting) {
        throw invalid_argument("Invalid argument"); // Можно ли их отнять, чтобы не было отрицательного значения?
    }

    DBASE borrow = 0; // Коэффициент заимствования
    int base_size = BASE_SIZE;
    DBASE tmp_difference;
    int index = 0;

    BASE *tempDigits = new BASE[length]; // Создаем временный массив для хранения промежуточных значений

    while (index < subtracting.length) {
        // Учитываем заимствование
        tmp_difference = ((DBASE)1 << base_size) | (DBASE)digits[index];
        tmp_difference = tmp_difference - subtracting.digits[index] - borrow;

        tempDigits[index] = (BASE)tmp_difference; // Записываем значение разности
        borrow = !(tmp_difference >> base_size); // Вычисляем заимствование

        index++;
    }

    while (index < length) {
        // Забираем заимствование
        tmp_difference = (1 << base_size) | digits[index];
        tmp_difference -= borrow; // Вычитаем заимствование

        tempDigits[index] = (BASE)tmp_difference;
        borrow = !(tmp_difference >> base_size);

        index++;
    }

    // Присваиваем значения временного массива текущему объекту
    delete[] digits;
    digits = tempDigits;
    length = length;
    while (length > 1 && digits[length - 1] == 0) {
        // Приводим фактическую длину
        length--;
    }

    return *this;
}

Big_Number Big_Number::operator*(const BASE &multiplier) {
    int index = 0;
    BASE carry = 0;
    Big_Number result(length + 1);
    DBASE tmp;
    int base_size = BASE_SIZE;
    
    while (index < length) {
        tmp = (DBASE)digits[index] * (DBASE)multiplier + (DBASE)carry;
        result.digits[index] = (BASE)tmp;
        carry = (BASE)(tmp >> base_size);
        index++;
    }

    result.digits[index] = carry;
    result.length = length + 1; // Установили длину

    while (result.length > 1 && result.digits[result.length - 1] == 0) {
        result.length--; // Нормализуем длину
    }

    return result;
}

Big_Number& Big_Number::operator*=(const BASE &multiplier) {
    int index = 0;
    BASE carry = 0;
    DBASE tmp;
    int base_size = BASE_SIZE;

    // Создаем временный массив для хранения промежуточных значений
    BASE *tempDigits = new BASE[length + 1];

    while (index < length) {
        tmp = (DBASE)digits[index] * (DBASE)multiplier + (DBASE)carry;
        tempDigits[index] = (BASE)tmp;
        carry = (BASE)(tmp >> base_size);
        index++;
    }

    tempDigits[index] = carry;
    int new_length = length + 1;

    // Нормализуем длину временного массива
    while (new_length > 1 && tempDigits[new_length - 1] == 0) {
        new_length--;
    }

    // Освобождаем старую память
    delete[] digits;

    // Переопределяем значения текущего объекта
    digits = tempDigits;
    length = new_length;

    return *this;
}

Big_Number Big_Number::operator*(const Big_Number &bNum)
{
    int j = 0;

    Big_Number newNum(length + bNum.length);
    DBASE tmp;

    int b = BASE_SIZE;

    while (j < bNum.length)
    {
        if (bNum.digits[j] != 0)
        {
            BASE k = 0;
            int i = 0;
            while (i < length)
            {
                tmp = ((DBASE)digits[i] * bNum.digits[j] + (DBASE)newNum.digits[i + j] + (DBASE)k);
                newNum.digits[i + j] = (BASE)tmp;
                k = (BASE)(tmp >> b);
                i++;
            }
            newNum.digits[length + j] = k;
        }
        j++;
    }
    newNum.length = length + bNum.length;
    while (newNum.length > 1 && newNum.digits[newNum.length - 1] == 0)
    {
        newNum.length--;
    }

    return newNum;
}

Big_Number& Big_Number::operator*=(const Big_Number &multiplier) {
    // Создаем временный массив для хранения промежуточных значений
    BASE *tempDigits = new BASE[length + multiplier.length]();

    DBASE tmp;
    int base_size = BASE_SIZE;

    for (int index_multiplier = 0; index_multiplier < multiplier.length; ++index_multiplier) {
        if (multiplier.digits[index_multiplier] != 0) {
            BASE carry = 0;
            for (int index_current = 0; index_current < length; ++index_current) {
                tmp = ((DBASE)digits[index_current] * multiplier.digits[index_multiplier] + (DBASE)tempDigits[index_current + index_multiplier] + (DBASE)carry);
                tempDigits[index_current + index_multiplier] = (BASE)tmp;
                carry = (BASE)(tmp >> base_size);
            }
            tempDigits[length + index_multiplier] = carry;
        }
    }

    // Освобождаем старую память текущего объекта
    delete[] digits;

    // Переопределяем значения текущего объекта на значения временного массива
    length = length + multiplier.length;
    digits = tempDigits;

    // Нормализуем длину
    while (length > 1 && digits[length - 1] == 0) {
        length--;
    }

    return *this;
}

Big_Number Big_Number::operator/(const BASE &divisor) {
    if (divisor == 0)
        throw std::runtime_error("Division by zero");
    int index = 0;
    DBASE remainder = 0;
    Big_Number result(length);

    int base_size = BASE_SIZE;

    while (index < length) {
        DBASE tmp = (((DBASE)remainder << base_size) + (DBASE)digits[length - 1 - index]);
        result.digits[length - 1 - index] = (BASE)(tmp / (DBASE)divisor);
        remainder = (BASE)(tmp % (DBASE)divisor);
        index++;
    }

    result.length = length;

    while (result.length > 1 && result.digits[result.length - 1] == 0) {
        result.length--;
    }

    return result;
}

Big_Number Big_Number::operator%(const BASE &divisor) {
    if (divisor == 0)
        throw std::runtime_error("Division by zero");
    int index = 0;
    DBASE remainder = 0;
    BASE quotient = 0;
    int base_size = BASE_SIZE;

    Big_Number result(1);

    while (index < length) {
        DBASE tmp = (((DBASE)remainder << base_size) + (DBASE)digits[length - 1 - index]);
        remainder = (BASE)(tmp % (DBASE)divisor);
        index++;
    }

    result.digits[0] = remainder;

    return result;
}

Big_Number& Big_Number::operator/=(const BASE &divisor) {
    if (divisor == 0)
        throw std::runtime_error("Division by zero");
    int index = 0;
    DBASE remainder = 0;
    BASE quotient = 0;
    int base_size = BASE_SIZE;

    // Создаем временный массив для хранения промежуточных значений
    BASE *tempDigits = new BASE[length]();

    while (index < length) {
        DBASE tmp = (((DBASE)remainder << base_size) + (DBASE)digits[length - 1 - index]);
        tempDigits[length - 1 - index] = (BASE)(tmp / (DBASE)divisor);
        remainder = (BASE)(tmp % (DBASE)divisor);
        index++;
    }

    // Освобождаем старую память текущего объекта
    delete[] digits;

    // Переопределяем значения текущего объекта на значения временного массива
    length = length;

    digits = tempDigits;

    // Нормализуем длину
    while (length > 1 && digits[length - 1] == 0) {
        length--;
    }

    return *this;
}

Big_Number& Big_Number::operator%=(const BASE &divisor) {
    if (divisor == 0)
        throw std::runtime_error("Division by zero");
    int index = 0;
    DBASE remainder = 0;
    BASE quotient = 0;
    int base_size = BASE_SIZE;

    while (index < length) {
        DBASE tmp = (((DBASE)remainder << base_size) + (DBASE)digits[length - 1 - index]);
        remainder = (BASE)(tmp % (DBASE)divisor);
        index++;
    }
    delete digits;
    digits = new BASE(1);
    // Переопределяем значения текущего объекта на остаток от деления
    length = 1;
    digits[0] = remainder;

    return *this;
}

void Big_Number::cout_10()
{
    Big_Number newNum = *this;
    Big_Number zero;
    string s;
    while (newNum != zero)
    {
        Big_Number t = newNum % 10;
        s.push_back(t.digits[0] + '0');
        newNum = newNum / 10;
    }
    reverse(s.begin(), s.end());
    cout << "Base10= " << s << endl;
}

void Big_Number::cin_10()
{
    int j = 0;
    string s;
    cout << "Base10: ";
    getline(cin, s);
    int k = s.length(); /// длина строки пользователя
    BASE t = 0;

    int b = BASE_SIZE;

    Big_Number bNum;
        Big_Number newNum;
    while (j < k)
    {
        if ('0' > s[j] || s[j] > '9')
        {
            throw invalid_argument("Invalid arguments");
        }
        t = s[j] - '0'; /// преобразование из строки в число
        bNum = bNum * 10;

        newNum.digits[0] = (BASE)t;
        bNum += newNum;
        j++;
    }

    bNum.length = bNum.maxlen;
    while (bNum.length > 1 && bNum.digits[bNum.length - 1] == 0)
    {
        bNum.length--;
    }
    *this = bNum;
}

Big_Number Big_Number::operator/(const Big_Number &num)
{

    if (num.length == 1 && num.digits[0] == 0)
    {
        throw invalid_argument("Invalid arguments1.");
    }
    if (*this < num)
    { /// маленькое делим на большое
        Big_Number finNum(1);
        return finNum;
    }

    if (num.length == 1)
    {
        return *this / num.digits[0];
    }

    int m = length - num.length; /// длина ответа
    int base_size = BASE_SIZE;
    DBASE b = ((DBASE)1 << base_size);                           /// основание числа
    DBASE d = b / (DBASE)(num.digits[num.length - 1] + (BASE)1); /// для нормализации D1
    int j = m;
    int k = 0; /// перенос

    Big_Number newNum = *this;
    newNum *= d;
    Big_Number delNum = num;
    delNum *= d;

    Big_Number finNum(m + 1);

    if (newNum.length == length)
    { /// сравниваем длину нормализованного с исходным D1
        newNum.maxlen++;
        newNum.length = maxlen;
        delete[] newNum.digits;
        newNum.digits = new BASE[maxlen];
        for (int i = 0; i < length; i++)
        {
            newNum.digits[i] = digits[i];
        }
        newNum *= d;
        newNum.length++;
        newNum.digits[newNum.length - 1] = 0;
    }

    while (j > -1)
    { // D3
        DBASE q = (DBASE)((DBASE)((DBASE)((DBASE)(newNum.digits[j + delNum.length]) * (DBASE)(b)) + (DBASE)(newNum.digits[j + delNum.length - 1])) / (DBASE)(delNum.digits[delNum.length - 1]));
        DBASE r = (DBASE)((DBASE)((DBASE)((DBASE)(newNum.digits[j + delNum.length]) * (DBASE)(b)) + (DBASE)(newNum.digits[j + delNum.length - 1])) % (DBASE)(delNum.digits[delNum.length - 1]));

        if (q == b || (DBASE)((DBASE)(q) * (DBASE)(delNum.digits[delNum.length - 2])) > (DBASE)((DBASE)((DBASE)(b) * (DBASE)(r)) + (DBASE)(newNum.digits[j + delNum.length - 2])))
        {
            q--;
            r = (DBASE)(r) + (DBASE)(delNum.digits[delNum.length - 1]);
            if ((DBASE)(r) < b)
            {
                if (q == b || (DBASE)((DBASE)(q) * (DBASE)(delNum.digits[delNum.length - 2])) > (DBASE)((DBASE)((DBASE)(b) * (DBASE)(r)) + (DBASE)(newNum.digits[j + delNum.length - 2])))
                {
                    q--;
                }
            }
        }
    //
        Big_Number u(delNum.length + 1);
        for (int i = 0; i < delNum.length + 1; i++)
        {
            u.digits[i] = newNum.digits[j + i]; /// кусок делимого длины делителя
        }

        if (u < delNum * (BASE)(q))
        { /// проверка
            q--;
        }

        u = u - delNum * (BASE)(q); /// отнимаем делитель на q
        finNum.digits[j] = (BASE)(q);

        for (int i = 0; i < delNum.length + 1; i++)
        {
            newNum.digits[j + i] = u.digits[i];
        }

        j--;
    }

    while (finNum.length > 1 && finNum.digits[finNum.length - 1] == 0)
    {
        finNum.length--;
    }

    return finNum;
}

Big_Number Big_Number::operator%(const Big_Number &num)
{
    if (num.length == 1 && num.digits[0] == 0)
    {
        throw invalid_argument("Invalid arguments.");
    }
    if (*this < num)
    {
        return *this;
    }

    if (num.length == 1)
    {
        return *this % num.digits[0];
    }

    int m = length - num.length;
    int base_size = BASE_SIZE;
    DBASE b = ((DBASE)(1) << (base_size));
    BASE d = (BASE)((DBASE)(b) / (DBASE)((num.digits[num.length - 1]) + (BASE)(1)));
    int j = m;
    int k = 0;

    Big_Number newNum = *this;
    newNum *= d;
    Big_Number delNum = num;
    delNum *= d;
    if (newNum.length == length)
    {
        newNum.maxlen++;
        newNum.length = maxlen;
        newNum.digits = new BASE[maxlen];
        for (int i = 0; i < length; i++)
        {
            newNum.digits[i] = digits[i];
        }
        newNum *= d;
        newNum.length++;
        newNum.digits[newNum.length - 1] = 0;
    }

    while (j > -1)
    {
        DBASE q = (DBASE)((DBASE)((DBASE)((DBASE)(newNum.digits[j + delNum.length]) * (DBASE)(b)) + (DBASE)(newNum.digits[j + delNum.length - 1])) / (DBASE)(delNum.digits[delNum.length - 1]));
        DBASE r = (DBASE)((DBASE)((DBASE)((DBASE)(newNum.digits[j + delNum.length]) * (DBASE)(b)) + (DBASE)(newNum.digits[j + delNum.length - 1])) % (DBASE)(delNum.digits[delNum.length - 1]));
        if (q == b || (DBASE)((DBASE)(q) * (DBASE)(delNum.digits[delNum.length - 2])) > (DBASE)((DBASE)((DBASE)(b) * (DBASE)(r)) + (DBASE)(newNum.digits[j + delNum.length - 2])))
        {
            q--;
            r = (DBASE)(r) + (DBASE)(delNum.digits[delNum.length - 1]);
            if ((DBASE)(r) < b)
            {
                if (q == b || (DBASE)((DBASE)(q) * (DBASE)(delNum.digits[delNum.length - 2])) > (DBASE)((DBASE)((DBASE)(b) * (DBASE)(r)) + (DBASE)(newNum.digits[j + delNum.length - 2])))
                {
                    q--;
                }
            }
        }
        Big_Number u(delNum.length + 1);
        for (int i = 0; i < delNum.length + 1; i++)
        {
            u.digits[i] = newNum.digits[j + i];
        }

        if (u < delNum * (BASE)(q)) //
        {

            q--;
        }

        u = u - (delNum * (BASE)(q));

        for (int i = 0; i < delNum.length + 1; i++)
        {
            newNum.digits[j + i] = u.digits[i];
        }

        j--;
    }
    while (newNum.length > 1 && newNum.digits[newNum.length - 1] == 0)
    {
        newNum.length--;
    }

    return newNum / d;
}


Big_Number& Big_Number::operator/=(const Big_Number &divisor) {
    if (divisor.length == 1 && divisor.digits[0] == 0) {
        throw invalid_argument("Invalid divisor.");
    }
    if (*this < divisor) {
        *this = Big_Number(1);
        return *this;
    }

    if (divisor.length == 1) {
        *this /= divisor.digits[0];
        return *this;
    }

    int result_length = length - divisor.length;
    int base_size = BASE_SIZE;
    DBASE base = ((DBASE)1 << base_size);
    DBASE norm_factor = base / (DBASE)(divisor.digits[divisor.length - 1] + (BASE)1);
    int j = result_length;
    int carry = 0;

    Big_Number dividend = *this;
    dividend *= norm_factor;
    Big_Number divisor_normalized = divisor;
    divisor_normalized *= norm_factor;

    Big_Number quotient(result_length + 1);
    quotient.length = result_length + 1;

    if (dividend.length == length) {
        dividend.maxlen++;
        dividend.length = dividend.maxlen;
        delete[] dividend.digits;
        dividend.digits = new BASE[dividend.maxlen];
        for (int i = 0; i < length; i++) {
            dividend.digits[i] = digits[i];
        }
        dividend *= norm_factor;
        dividend.length++;
        dividend.digits[dividend.length - 1] = 0;
    }

    while (j > -1) {
        DBASE q = (DBASE)((DBASE)((DBASE)((DBASE)(dividend.digits[j + divisor_normalized.length]) * (DBASE)(base)) +
                     (DBASE)(dividend.digits[j + divisor_normalized.length - 1])) / (DBASE)(divisor_normalized.digits[divisor_normalized.length - 1]));
        DBASE r = (DBASE)((DBASE)((DBASE)((DBASE)(dividend.digits[j + divisor_normalized.length]) * (DBASE)(base)) +
                     (DBASE)(dividend.digits[j + divisor_normalized.length - 1])) % (DBASE)(divisor_normalized.digits[divisor_normalized.length - 1]));

        if (q == base || (DBASE)((DBASE)(q) * (DBASE)(divisor_normalized.digits[divisor_normalized.length - 2])) >
                          (DBASE)((DBASE)((DBASE)(base) * (DBASE)(r)) + (DBASE)(dividend.digits[j + divisor_normalized.length - 2]))) {
            q--;
            r = (DBASE)(r) + (DBASE)(divisor_normalized.digits[divisor_normalized.length - 1]);
            if ((DBASE)(r) < base) {
                if (q == base || (DBASE)((DBASE)(q) * (DBASE)(divisor_normalized.digits[divisor_normalized.length - 2])) >
                                  (DBASE)((DBASE)((DBASE)(base) * (DBASE)(r)) + (DBASE)(dividend.digits[j + divisor_normalized.length - 2]))) {
                    q--;
                }
            }
        }

        Big_Number u(divisor_normalized.length + 1);
        u.length = divisor_normalized.length + 1;
        for (int i = 0; i < divisor_normalized.length + 1; i++) {
            u.digits[i] = dividend.digits[j + i];
        }

        if (u < divisor_normalized * (BASE)(q)) {
            q--;
        }

        u = u - divisor_normalized * (BASE)(q);
        quotient.digits[j] = (BASE)(q);

        for (int i = 0; i < divisor_normalized.length + 1; i++) {
            dividend.digits[j + i] = u.digits[i];
        }

        j--;
    }

    while (quotient.length > 1 && quotient.digits[quotient.length - 1] == 0) {
        quotient.length--;
    }

    *this = quotient;
    return *this;
}

Big_Number& Big_Number::operator%=(const Big_Number &divisor) {
    if (divisor.length == 1 && divisor.digits[0] == 0) {
        throw invalid_argument("Invalid divisor.");
    }
    if (*this < divisor) {
        return *this;
    }

    if (divisor.length == 1) {
        *this %= divisor.digits[0];
        return *this;
    }

    int result_length = length - divisor.length;
    int base_size = BASE_SIZE;
    DBASE base = ((DBASE)(1) << (base_size));
    BASE norm_factor = (BASE)((DBASE)(base) / (DBASE)((divisor.digits[divisor.length - 1]) + (BASE)(1)));
    int j = result_length;
    int carry = 0;

    Big_Number dividend = *this;
    dividend *= norm_factor;
    Big_Number divisor_normalized = divisor;
    divisor_normalized *= norm_factor;
    if (dividend.length == length) {
        dividend.maxlen++;
        dividend.length = dividend.maxlen;
        dividend.digits = new BASE[dividend.maxlen];
        for (int i = 0; i < length; i++) {
            dividend.digits[i] = digits[i];
        }
        dividend *= norm_factor;
        dividend.length++;
        dividend.digits[dividend.length - 1] = 0;
    }

    while (j > -1) {
        DBASE q = (DBASE)((DBASE)((DBASE)((DBASE)(dividend.digits[j + divisor_normalized.length]) * (DBASE)(base)) +
                     (DBASE)(dividend.digits[j + divisor_normalized.length - 1])) / (DBASE)(divisor_normalized.digits[divisor_normalized.length - 1]));
        DBASE r = (DBASE)((DBASE)((DBASE)((DBASE)(dividend.digits[j + divisor_normalized.length]) * (DBASE)(base)) +
                     (DBASE)(dividend.digits[j + divisor_normalized.length - 1])) % (DBASE)(divisor_normalized.digits[divisor_normalized.length - 1]));
        if (q == base || (DBASE)((DBASE)(q) * (DBASE)(divisor_normalized.digits[divisor_normalized.length - 2])) >
                          (DBASE)((DBASE)((DBASE)(base) * (DBASE)(r)) + (DBASE)(dividend.digits[j + divisor_normalized.length - 2]))) {
            q--;
            r = (DBASE)(r) + (DBASE)(divisor_normalized.digits[divisor_normalized.length - 1]);
            if ((DBASE)(r) < base) {
                if (q == base || (DBASE)((DBASE)(q) * (DBASE)(divisor_normalized.digits[divisor_normalized.length - 2])) >
                                  (DBASE)((DBASE)((DBASE)(base) * (DBASE)(r)) + (DBASE)(dividend.digits[j + divisor_normalized.length - 2]))) {
                    q--;
                }
            }
        }
        Big_Number u(divisor_normalized.length + 1);
        u.length = divisor_normalized.length + 1;
        for (int i = 0; i < divisor_normalized.length + 1; i++) {
            u.digits[i] = dividend.digits[j + i];
        }

        if (u < divisor_normalized * (BASE)(q)) {
            q--;
        }

        u = u - (divisor_normalized * (BASE)(q));

        for (int i = 0; i < divisor_normalized.length + 1; i++) {
            dividend.digits[j + i] = u.digits[i];
        }

        j--;
    }
    while (dividend.length > 1 && dividend.digits[dividend.length - 1] == 0) {
        dividend.length--;
    }

    *this = dividend;
    return *this;
}


istream &operator>>(istream &in, Big_Number &bNum)
{
    char *s = new char[1000];
    int b = BASE_SIZE int j = 0;
    int k = 0;
    unsigned int tmp = 0;

    in.getline(s, 1000);
    bNum.length = (strlen(s) - 1) / (b / 4) + 1;
    bNum.maxlen = bNum.length;
    bNum.digits = new BASE[bNum.maxlen];
    for (int i = 0; i < bNum.length; i++)
    {
        bNum.digits[i] = 0;
    }
    for (int i = strlen(s) - 1; i > -1; i--)
    {
        if ('0' <= s[i] && s[i] <= '9')
        {
            tmp = s[i] - '0';
        }
        else if ('a' <= s[i] && s[i] <= 'f')
        {
            tmp = s[i] - 'a' + 10;
        }
        else if ('A' <= s[i] && s[i] <= 'F')
        {
            tmp = s[i] - 'A' + 10;
        }
        else
        {
            throw invalid_argument("Invalid arguments.");
        }

        bNum.digits[j] |= tmp << (k * 4);
        k++;
        if (k >= b / 4)
        {
            k = 0;
            j++;
        }
    }
    return in;
}

ostream &operator<<(ostream &st, const Big_Number &num)
{
    int size = BASE_SIZE;
    for (int j = num.length - 1; j >= 0; j--)
    {
        st.width(size / 4); // вычисляем сколько может влезть в коэфициент (4 бита)
        st.fill('0');       // заполняем нолями пустые места
        st << hex << (int)num.digits[j];
    }
    st << dec;
    if (num.length == 0)
    {
        st << '0' <<endl;
    }
    return st;
}



void test() {
    int M = 1000;
    int T = 1000;

    Big_Number A;
    Big_Number B;
    Big_Number C;
    Big_Number D;
    do {
        int n = rand() % M + 1;
        int m = rand() % M + 1;
        Big_Number E(n, 1);
        Big_Number G(m, 1);
        A = E;
        B = G;
        C = A / B;
        D = A % B;
        cout << "m: " << m << " ";
        cout << "n: " << n << " ";
        cout << "T: " << T << endl;
    } while (A == C * B + D && A - D == C * B && D < B && --T >= 0);
}



int main()
{
    test();
    return 0;
}