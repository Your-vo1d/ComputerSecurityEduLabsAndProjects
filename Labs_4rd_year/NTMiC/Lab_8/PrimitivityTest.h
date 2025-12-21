#ifndef PRIMITIVITY_TEST_H
#define PRIMITIVITY_TEST_H

#include "Polynomial.h"
#include <vector>
#include <utility>

class PrimalityTest {
private:
    static std::vector<std::pair<int, int>> factorize(int n);
    static int computeActualExponent(const Polynomial& f, const std::vector<std::pair<int, int>>& factors);
    
public:
    static bool isPrimitive(const Polynomial& f, const std::vector<std::pair<int, int>>& factors);
    static std::vector<std::pair<int, int>> getFactorization(int p, int n);
    static void testPolynomial(const Polynomial& f, const std::string& name);
};

#endif // PRIMITIVITY_TEST_H