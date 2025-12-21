#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <string>

class Polynomial {
private:
    std::vector<int> coeffs; // coeffs[i] — коэффициент при x^i
    int p;              // модуль (простое число)

    int mod(int x) const;
    int modInverse(int a) const;
    static long long ipow(int base, int exp);

public:
    Polynomial(int prime = 2);
    Polynomial(const std::vector<int>& c, int prime);

    void normalize();
    int degree() const;
    bool isZero() const;
    int operator[](int i) const;

    Polynomial operator+(const Polynomial& b) const;
    Polynomial operator-(const Polynomial& b) const;
    Polynomial operator*(const Polynomial& b) const;

    Polynomial modDiv(Polynomial B) const;
    Polynomial mulMod(const Polynomial& b, const Polynomial& M) const;
    Polynomial powMod(long long e, const Polynomial& M) const;

    static Polynomial gcd(Polynomial a, Polynomial b);

    static bool test1(const Polynomial& f);
    static bool test2(const Polynomial& f);

    int getMod() const { return p; }
    const std::vector<int>& getCoeffs() const { return coeffs; }
};

std::vector<int> parsePolyString(const std::string& s);

#endif // POLYNOMIAL_H