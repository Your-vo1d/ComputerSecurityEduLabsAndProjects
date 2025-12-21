#include "Polynomial.h"
#include <stdexcept>
#include <iostream>

using namespace std;

// ------------------------------------------------------------
// Вспомогательные функции
// ------------------------------------------------------------

int Polynomial::mod(int x) const {
    x %= p;
    if (x < 0) x += p;
    return x;
}

int Polynomial::modInverse(int a) const {
    int res = 1, e = p - 2;
    a = mod(a);
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
    : coeffs(c), p(prime) {
    normalize();
}

void Polynomial::normalize() {
    while (!coeffs.empty() && coeffs.back() == 0)
        coeffs.pop_back();
    for (int& x : coeffs) x = mod(x);
}

int Polynomial::degree() const { return (int)coeffs.size() - 1; }

bool Polynomial::isZero() const { return coeffs.empty(); }

int Polynomial::operator[](int i) const {
    return i < (int)coeffs.size() ? coeffs[i] : 0;
}

// ------------------------------------------------------------
// Арифметика
// ------------------------------------------------------------

Polynomial Polynomial::operator+(const Polynomial& b) const {
    int n = max(coeffs.size(), b.coeffs.size());
    vector<int> r(n);
    for (int i = 0; i < n; ++i)
        r[i] = mod((*this)[i] + b[i]);
    return Polynomial(r, p);
}

Polynomial Polynomial::operator-(const Polynomial& b) const {
    int n = max(coeffs.size(), b.coeffs.size());
    vector<int> r(n);
    for (int i = 0; i < n; ++i)
        r[i] = mod((*this)[i] - b[i]);
    return Polynomial(r, p);
}

Polynomial Polynomial::operator*(const Polynomial& b) const {
    if (isZero() || b.isZero()) return Polynomial({}, p);
    vector<int> r(coeffs.size() + b.coeffs.size() - 1);
    for (int i = 0; i < (int)coeffs.size(); ++i)
        for (int j = 0; j < (int)b.coeffs.size(); ++j)
            r[i + j] = mod(r[i + j] + coeffs[i] * b.coeffs[j]);
    return Polynomial(r, p);
}

// ------------------------------------------------------------
// Деление по модулю и операции над полиномами
// ------------------------------------------------------------

Polynomial Polynomial::modDiv(Polynomial B) const {
    Polynomial A = *this;
    A.normalize(); B.normalize();
    if (B.isZero()) throw runtime_error("Division by zero polynomial");
    int db = B.degree();
    int invLead = modInverse(B.coeffs.back());
    while (A.degree() >= db && !A.isZero()) {
        int diff = A.degree() - db;
        int coef = mod(A.coeffs.back() * invLead);
        for (int i = 0; i <= db; ++i)
            A.coeffs[i + diff] = mod(A[i + diff] - coef * B[i]);
        A.normalize();
    }
    return A;
}

Polynomial Polynomial::mulMod(const Polynomial& b, const Polynomial& M) const {
    Polynomial res = (*this) * b;
    return res.modDiv(M);
}

Polynomial Polynomial::powMod(long long e, const Polynomial& M) const {
    Polynomial base = *this;
    Polynomial res({1}, p);
    while (e) {
        if (e & 1) res = res.mulMod(base, M);
        base = base.mulMod(base, M);
        e >>= 1;
    }
    return res;
}

Polynomial Polynomial::gcd(Polynomial a, Polynomial b) {
    a.normalize(); b.normalize();
    while (!b.isZero()) {
        Polynomial r = a.modDiv(b);
        a = b;
        b = r;
    }
    if (!a.isZero()) {
        int invLead = a.modInverse(a.coeffs.back());
        for (int& x : a.coeffs) x = (x * invLead) % a.p;
    }
    return a;
}

// ------------------------------------------------------------
// Тесты неприводимости (Рабин)
// ------------------------------------------------------------

bool Polynomial::test1(const Polynomial& f) {
    int n = f.degree(), p = f.p;
    Polynomial u({0, 1}, p); // u(x)=x
    for (int i = 1; i <= n / 2; ++i) {
        u = u.powMod(p, f);
        Polynomial d = gcd(f, u - Polynomial({0, 1}, p));
        if (d.degree() > 0) return false;
    }
    return true;
}

bool Polynomial::test2(const Polynomial& f) {
    int n = f.degree(), p = f.p;
    Polynomial xpoly({0, 1}, p);
    Polynomial u = xpoly.powMod(ipow(p, n), f);

    if (!(u - xpoly).isZero()) return false;

    vector<int> primes;
    int m = n;
    for (int i = 2; i * i <= m; ++i)
        if (m % i == 0) {
            primes.push_back(i);
            while (m % i == 0) m /= i;
        }
    if (m > 1) primes.push_back(m);

    for (int q : primes) {
        long long exp = ipow(p, n / q);
        Polynomial v = xpoly.powMod(exp, f);
        Polynomial d = gcd(f, v - xpoly);
        d.normalize();
        if (d.degree() > 0) return false;
    }
    return true;
}

// ------------------------------------------------------------
// Парсер строки
// ------------------------------------------------------------

vector<int> parsePolyString(const string& s) {
    vector<int> a;
    for (int i = (int)s.size() - 1; i >= 0; --i)
        a.push_back(s[i] - '0');
    return a;
}