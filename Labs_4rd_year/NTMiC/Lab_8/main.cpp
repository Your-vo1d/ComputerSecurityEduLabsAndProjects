#include "PrimitivityTest.h"
#include <iostream>
#include <vector>

using namespace std;

void testAllPolynomials() {
    cout << "=== ТЕСТИРОВАНИЕ ПРИМИТИВНОСТИ ПОЛИНОМОВ ===" << endl << endl;
    
    // ==================== Z2[x] ====================
    cout << "КОЛЬЦО Z2[x]:" << endl;
    cout << "==================" << endl;
    
    // Примитивные полиномы над Z2
    vector<vector<int>> primitive_z2 = {
        {1, 0, 1, 1, 1, 0, 0, 0, 1}, // x^8 + x^4 + x^3 + x^2 + 1
        {1, 1, 0, 1, 0, 1, 0, 0, 1}, // x^8 + x^5 + x^3 + x + 1
        {1, 0, 1, 1, 0, 1, 0, 0, 1}, // x^8 + x^5 + x^3 + x^2 + 1
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1}, // x^10 + x^3 + 1
        {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1}  // x^10 + x^4 + x^3 + x + 1
    };
    
    vector<string> names_primitive_z2 = {
        "x^8 + x^4 + x^3 + x^2 + 1",
        "x^8 + x^5 + x^3 + x + 1", 
        "x^8 + x^5 + x^3 + x^2 + 1",
        "x^10 + x^3 + 1",
        "x^10 + x^4 + x^3 + x + 1"
    };
    
    for (size_t i = 0; i < primitive_z2.size(); i++) {
        Polynomial f(primitive_z2[i], 2);
        PrimalityTest::testPolynomial(f, names_primitive_z2[i]);
    }
    
    // Непримитивные полиномы над Z2
    vector<vector<int>> non_primitive_z2 = {
        {1, 1, 0, 1, 1, 0, 0, 0, 1}, // x^8 + x^4 + x^3 + x + 1
        {1, 0, 0, 1, 1, 1, 0, 0, 1}, // x^8 + x^5 + x^4 + x^3 + 1
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1}, // x^10 + x^3 + x^2 + x + 1
        {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1}, // x^10 + x^4 + x^3 + x^2 + 1
        {1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1}  // x^10 + x^5 + x^4 + x^2 + 1
    };
    
    vector<string> names_non_primitive_z2 = {
        "x^8 + x^4 + x^3 + x + 1",
        "x^8 + x^5 + x^4 + x^3 + 1",
        "x^10 + x^3 + x^2 + x + 1", 
        "x^10 + x^4 + x^3 + x^2 + 1",
        "x^10 + x^5 + x^4 + x^2 + 1"
    };
    
    for (size_t i = 0; i < non_primitive_z2.size(); i++) {
        Polynomial f(non_primitive_z2[i], 2);
        PrimalityTest::testPolynomial(f, names_non_primitive_z2[i]);
    }
    
    // ==================== Z3[x] ====================
    cout << "КОЛЬЦО Z3[x]:" << endl;
    cout << "==================" << endl;
    
    // Примитивные полиномы над Z3
    vector<vector<int>> primitive_z3 = {
        {1, 2, 0, 0, 0, 1}, // x^5 + 2x + 1
        {1, 1, 2, 0, 0, 1}, // x^5 + 2x^2 + x + 1
        {2, 1, 0, 0, 0, 0, 1}, // x^6 + x + 2
        {2, 2, 0, 0, 0, 0, 1}  // x^6 + 2x + 2
    };
    
    vector<string> names_primitive_z3 = {
        "x^5 + 2x + 1",
        "x^5 + 2x^2 + x + 1", 
        "x^6 + x + 2",
        "x^6 + 2x + 2"
    };
    
    for (size_t i = 0; i < primitive_z3.size(); i++) {
        Polynomial f(primitive_z3[i], 3);
        PrimalityTest::testPolynomial(f, names_primitive_z3[i]);
    }
    
    // ==================== Z5[x] ====================
    cout << "КОЛЬЦО Z5[x]:" << endl;
    cout << "==================" << endl;
    
    // Примитивные полиномы над Z5
    vector<vector<int>> primitive_z5 = {
        {2, 4, 0, 0, 0, 1}, // x^5 + 4x + 2
        {3, 4, 0, 0, 0, 1}, // x^5 + 4x + 3
        {2, 2, 1, 0, 1},    // x^4 + x^2 + 2x + 2
        {2, 3, 1, 0, 1}     // x^4 + x^2 + 3x + 2
    };
    
    vector<string> names_primitive_z5 = {
        "x^5 + 4x + 2",
        "x^5 + 4x + 3",
        "x^4 + x^2 + 2x + 2", 
        "x^4 + x^2 + 3x + 2"
    };
    
    for (size_t i = 0; i < primitive_z5.size(); i++) {
        Polynomial f(primitive_z5[i], 5);
        PrimalityTest::testPolynomial(f, names_primitive_z5[i]);
    }
}

int main() {
    testAllPolynomials();
    return 0;
}