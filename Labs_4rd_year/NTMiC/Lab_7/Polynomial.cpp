#include "Polynomial.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
using namespace std;



static int mod_static(int x, int p) {
    x %= p;
    if (x < 0) x += p;
    return x;
}


// ------------------------------------------------------------
// Статическая вспомогательная функция mod
// ------------------------------------------------------------
int Polynomial::mod(int x) const {
    return mod_static(x, p);
}

int Polynomial::modInverse(int a) const {
    a = mod(a);
    if (a == 0) throw runtime_error("No inverse for zero");
    int res = 1, e = p - 2;
    while (e) {
        if (e & 1) res = (1LL * res * a) % p;
        a = (1LL * a * a) % p;
        e >>= 1;
    }
    return res;
}

long long Polynomial::ipow(int base, int exp) {
    long long res = 1;
    while (exp--) res *= base;
    return res;
}

// ------------------------------------------------------------
// Конструкторы и нормализация
// ------------------------------------------------------------
Polynomial::Polynomial(int prime) : p(prime) {}

Polynomial::Polynomial(const vector<int>& c, int prime)
    : coeffs(c), p(prime) { normalize(); }

void Polynomial::normalize() {
    while (!coeffs.empty() && coeffs.back() == 0)
        coeffs.pop_back();
    for (int& x : coeffs) x = mod(x);
}

int Polynomial::degree() const {
    return coeffs.empty() ? -1 : (int)coeffs.size() - 1;
}

bool Polynomial::isZero() const { return coeffs.empty(); }

int Polynomial::operator[](int i) const {
    return i < (int)coeffs.size() ? coeffs[i] : 0;
}

// ------------------------------------------------------------
// Арифметика
// ------------------------------------------------------------
Polynomial Polynomial::operator+(const Polynomial& b) const {
    int n = max((int)coeffs.size(), (int)b.coeffs.size());
    vector<int> r(n);
    for (int i = 0; i < n; ++i)
        r[i] = mod((*this)[i] + b[i]);
    return Polynomial(r, p);
}

Polynomial Polynomial::operator-(const Polynomial& b) const {
    int n = max((int)coeffs.size(), (int)b.coeffs.size());
    vector<int> r(n);
    for (int i = 0; i < n; ++i)
        r[i] = mod((*this)[i] - b[i]);
    return Polynomial(r, p);
}

Polynomial Polynomial::operator*(const Polynomial& b) const {
    if (isZero() || b.isZero()) return Polynomial({}, p);
    vector<int> r(coeffs.size() + b.coeffs.size() - 1, 0);
    for (size_t i = 0; i < coeffs.size(); ++i)
        for (size_t j = 0; j < b.coeffs.size(); ++j)
            r[i + j] = mod(r[i + j] + coeffs[i] * b.coeffs[j]);
    return Polynomial(r, p);
}

Polynomial Polynomial::modDiv(const Polynomial& B) const {
    Polynomial A = *this;
    A.normalize();
    Polynomial BB = B;
    BB.normalize();
    if (BB.isZero()) throw std::runtime_error("Division by zero polynomial");
    if (A.isZero()) return A;

    std::vector<int> rem = A.coeffs;
    int db = BB.degree();
    int invLead = modInverse(BB.coeffs.back());

    while (!rem.empty() && (int)rem.size() - 1 >= db) {
        int da = (int)rem.size() - 1;
        int lead = rem.back();
        int q = mod(lead * invLead);
        int diff = da - db;

        for (size_t i = 0; i < BB.coeffs.size(); ++i) {
            if (i + diff < rem.size()) {
                rem[i + diff] = mod(rem[i + diff] - q * BB.coeffs[i]);
            }
        }

        while (!rem.empty() && rem.back() == 0)
            rem.pop_back();
    }

    Polynomial result(rem, p);
    result.normalize();
    return result;
}

Polynomial Polynomial::mulMod(const Polynomial& b, const Polynomial& M) const {
    return ((*this) * b).modDiv(M);
}

Polynomial Polynomial::powMod(long long e, const Polynomial& M) const {
    Polynomial base = *this;
    Polynomial res({1}, p);
    base = base.modDiv(M);
    while (e > 0) {
        if (e & 1) res = res.mulMod(base, M);
        base = base.mulMod(base, M);
        e >>= 1;
    }
    return res;
}

Polynomial Polynomial::gcd(Polynomial a, Polynomial b) {
    a.normalize(); b.normalize();
    int prime = a.p;

    while (!b.isZero()) {
        Polynomial r = a.modDiv(b);
        a = std::move(b);
        b = std::move(r);
    }

    if (!a.isZero()) {
        int inv = a.modInverse(a.coeffs.back());
        for (int& x : a.coeffs) {
            x = mod_static(x * inv, prime);
        }
    }
    return a;
}

// ------------------------------------------------------------
// Проверка линейных корней
// ------------------------------------------------------------
bool Polynomial::hasLinearRoot(const Polynomial& f) {
    for (int x = 0; x < f.p; ++x) {
        long long val = 0, power = 1;
        for (int c : f.coeffs) {
            val = (val + 1LL * c * power) % f.p;
            power = (power * x) % f.p;
        }
        if (val == 0) return true;
    }
    return false;
}
bool Polynomial::test1(const Polynomial& f) {
    if (f.degree() <= 0) return false;
    if (hasLinearRoot(f)) return false;

    int n = f.degree();
    Polynomial xpoly({0, 1}, f.p);

    // ← НАЧИНАЕМ С x^p mod f (i=1)
    Polynomial u = xpoly.powMod(f.p, f);

    // Проверяем i=1, i=2, ..., floor(n/2)
    for (int i = 1; i <= n / 2; ++i) {
        Polynomial diff = u - xpoly;
        diff.normalize();
        Polynomial d = gcd(f, diff);
        if (d.degree() > 0 && !d.isZero()) return false;

        // Переходим к следующему i: u = u^p = x^(p^{i+1}) mod f
        if (i < n / 2) {
            u = u.powMod(f.p, f);
        }
    }
    return true;
}

bool Polynomial::test2(const Polynomial& f) {
    if (f.degree() <= 0) return false;
    if (hasLinearRoot(f)) return false;

    int n = f.degree(), p = f.p;
    Polynomial xpoly({0, 1}, p);
    long long full_exp = ipow(p, n);
    Polynomial u = xpoly.powMod(full_exp, f);

    if (!(u - xpoly).isZero()) return false;

    int m = n;
    vector<int> factors;
    for (int i = 2; i * i <= m; ++i) {
        if (m % i == 0) {
            factors.push_back(i);
            while (m % i == 0) m /= i;
        }
    }
    if (m > 1) factors.push_back(m);

    for (int q : factors) {
        long long exp = ipow(p, n / q);
        Polynomial v = xpoly.powMod(exp, f);
        Polynomial diff = v - xpoly;
        diff.normalize();
        Polynomial d = gcd(f, diff);
        if (d.degree() > 0 && !d.isZero()) return false;
    }
    return true;
}

// ------------------------------------------------------------
// Парсер строки
// ------------------------------------------------------------
vector<int> parsePolyString(const string& s) {
    vector<int> a;
    for (int i = (int)s.size() - 1; i >= 0; --i) {
        int d = s[i] - '0';
        if (d < 0 || d > 9)
            throw runtime_error("Invalid character in polynomial string");
        a.push_back(d);
    }
    return a;
}