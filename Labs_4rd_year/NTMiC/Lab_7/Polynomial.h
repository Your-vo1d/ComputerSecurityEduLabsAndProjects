#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
using namespace std;

class Polynomial {
public:
    vector<int> coeffs; // коэффициенты (от младшего к старшему)
    int p;               // простое основание поля

    // ------------------------------------------------------------
    // Конструкторы
    // ------------------------------------------------------------
    Polynomial(int prime = 2);
    Polynomial(const vector<int>& c, int prime);

    // ------------------------------------------------------------
    // Основные методы
    // ------------------------------------------------------------
    void normalize();
    int degree() const;
    bool isZero() const;
    int operator[](int i) const;

    // ------------------------------------------------------------
    // Арифметические операции
    // ------------------------------------------------------------
    Polynomial operator+(const Polynomial& b) const;
    Polynomial operator-(const Polynomial& b) const;
    Polynomial operator*(const Polynomial& b) const;

    // Деление с остатком (mod)
    Polynomial modDiv(const Polynomial& B) const;
    Polynomial mulMod(const Polynomial& b, const Polynomial& M) const;
    Polynomial powMod(long long e, const Polynomial& M) const;
    static Polynomial gcd(Polynomial a, Polynomial b);

    // ------------------------------------------------------------
    // Проверка корней
    // ------------------------------------------------------------
    static bool hasLinearRoot(const Polynomial& f);

    // ------------------------------------------------------------
    // Тесты Рабина
    // ------------------------------------------------------------
    static bool test1(const Polynomial& f);
    static bool test2(const Polynomial& f);

    // ------------------------------------------------------------
    // Вспомогательные
    // ------------------------------------------------------------
    int mod(int x) const;
    int modInverse(int a) const;
    static long long ipow(int base, int exp);
};

// Парсер строки в вектор коэффициентов
vector<int> parsePolyString(const string& s);