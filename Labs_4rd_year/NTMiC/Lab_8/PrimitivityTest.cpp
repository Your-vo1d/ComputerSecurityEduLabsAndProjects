#include "PrimitivityTest.h"
#include <cmath>
#include <iostream>

using namespace std;

// Простая факторизация числа
vector<pair<int, int>> PrimalityTest::factorize(int n) {
    vector<pair<int, int>> factors;
    
    if (n <= 1) return factors;
    
    // Проверяем делимость на 2
    if (n % 2 == 0) {
        int count = 0;
        while (n % 2 == 0) {
            n /= 2;
            count++;
        }
        factors.push_back({2, count});
    }
    
    // Проверяем нечетные делители
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            int count = 0;
            while (n % i == 0) {
                n /= i;
                count++;
            }
            factors.push_back({i, count});
        }
    }
    
    // Если остался простой делитель
    if (n > 1) {
        factors.push_back({n, 1});
    }
    
    return factors;
}

vector<pair<int, int>> PrimalityTest::getFactorization(int p, int n) {
    long long number = 1;
    for (int i = 0; i < n; i++) {
        number *= p;
    }
    number -= 1;
    return factorize((int)number);
}

// Функция для вычисления фактического показателя (периода)
int PrimalityTest::computeActualExponent(const Polynomial& f, const vector<pair<int, int>>& factors) {
    int p = f.getMod();
    int n = f.degree();
    
    // Вычисляем p^n - 1
    long long pn_1 = 1;
    for (int i = 0; i < n; i++) {
        pn_1 *= p;
    }
    pn_1 -= 1;
    
    Polynomial x_poly({0, 1}, p);
    Polynomial one({1}, p);
    
    // Начинаем с предположения, что показатель = p^n - 1
    int exponent = pn_1;
    
    // Для каждого простого делителя q
    for (const auto& factor : factors) {
        int q = factor.first;
        int power = factor.second;
        
        // Делим текущий показатель на q столько раз, сколько возможно
        for (int i = 0; i < power; i++) {
            if (exponent % q != 0) break;
            
            // Пробуем разделить показатель на q
            int candidate = exponent / q;
            
            // Проверяем, остается ли x^candidate ≡ 1 mod f(x)
            Polynomial r = x_poly.powMod(candidate, f);
            if (r.getCoeffs() == one.getCoeffs()) {
                exponent = candidate; // показатель можно уменьшить
            } else {
                break; // нельзя дальше делить на этот q
            }
        }
    }
    
    return exponent;
}
bool PrimalityTest::isPrimitive(const Polynomial& f, const vector<pair<int, int>>& factors) {
    int p = f.getMod();
    int n = f.degree();
    
    // Вычисляем p^n - 1
    long long pn_1 = 1;
    for (int i = 0; i < n; i++) {
        pn_1 *= p;
    }
    pn_1 -= 1;
    
    // Проверяем, что полином неприводим
    if (!Polynomial::test1(f)) {
        cout << "  Полином не прошел test1 (неприводимость)" << endl;
        return false;
    }
    if (!Polynomial::test2(f)) {
        cout << "  Полином не прошел test2 (неприводимость)" << endl;
        return false;
    }
    
    // Полином x (для возведения в степень)
    Polynomial x_poly({0, 1}, p);
    Polynomial one({1}, p);
    
    // Тест: для каждого простого делителя q числа p^n - 1
    // проверяем, что x^((p^n-1)/q) ≠ 1 (mod f(x))
    for (const auto& factor : factors) {
        int q = factor.first;
        long long exponent = pn_1 / q;
        
        // Вычисляем x^exponent mod f(x)
        Polynomial r = x_poly.powMod(exponent, f);
        
        // Если результат равен 1, то полином не примитивен
        if (r.getCoeffs() == one.getCoeffs()) {
            return false;
        }
    }
    
    return true;
}

void PrimalityTest::testPolynomial(const Polynomial& f, const string& name) {
    cout << "Тестируем: " << name << endl;
    cout << "  Степень: " << f.degree() << ", модуль: " << f.getMod() << endl;
    
    // Проверка неприводимости
    bool irreducible1 = Polynomial::test1(f);
    bool irreducible2 = Polynomial::test2(f);
    bool irreducible = irreducible1 && irreducible2;
    
    cout << "  Неприводимость: " << (irreducible ? "ДА" : "НЕТ") << endl;
    
    if (!irreducible) {
        cout << "  Результат: Полином приводим" << endl << endl;
        return;
    }
    
    // Получаем разложение p^n - 1
    auto factors = getFactorization(f.getMod(), f.degree());
    
    // Вычисляем фактический показатель
    int actual_exponent = computeActualExponent(f, factors);
    int max_exponent = 1;
    for (int i = 0; i < f.degree(); i++) {
        max_exponent *= f.getMod();
    }
    max_exponent -= 1;
    
    bool primitive = (actual_exponent == max_exponent);
    
    cout << "  Фактический показатель: " << actual_exponent << endl;
    cout << "  Примитивность: " << (primitive ? "ДА" : "НЕТ") << endl;
    
    // Выводим разложение p^n - 1
    cout << "  " << f.getMod() << "^" << f.degree() << " - 1 = " << max_exponent << " = ";
    for (size_t i = 0; i < factors.size(); i++) {
        cout << factors[i].first;
        if (factors[i].second > 1) {
            cout << "^" << factors[i].second;
        }
        if (i < factors.size() - 1) {
            cout << " * ";
        }
    }
    cout << endl << endl;
}