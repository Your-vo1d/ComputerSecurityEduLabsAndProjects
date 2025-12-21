#include "Polynomial.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    struct Test { int p; string coeffs; bool expected; };
    vector<Test> tests = {
        {2,"100011101",true}, {2,"100101011",true}, {2,"100101101",true},
        {2,"10000001001",true}, {2,"10000011011",true}, {2,"100011011",true},
        {2,"100111001",true}, {2,"10000001111",true}, {2,"10000011101",true}, {2,"10000110101",true},
        {2,"100111011",false}, {2,"100011111",false}, {2,"100010011",false}, {2,"100111101",false},
        {2,"100100011",false}, {2,"1000001011",false}, {2,"1000001101",false}, {2,"1000111101",false},
        {2,"1000101111",false}, {2,"1000110111",false},

        {3,"100021",true},{3,"100211",true},{3,"1000012",true},{3,"1000022",true},{3,"100022",true},
        {3,"100112",true},{3,"102122",true},{3,"1000111",true},{3,"1000121",true},{3,"1000201",true},
        {3,"100012",false},{3,"100212",false},{3,"100221",false},{3,"101202",false},{3,"102102",false},
        {3,"1000011",false},{3,"1000122",false},{3,"1000112",false},{3,"1000211",false},{3,"1000212",false},

        {5,"100042",true},{5,"100043",true},{5,"10122",true},{5,"10132",true},{5,"100041",true},
        {5,"100044",true},{5,"100124",true},{5,"10002",true},{5,"10014",true},{5,"10102",true},
        {5,"100031",false},{5,"100034",false},{5,"100021",false},{5,"100112",false},{5,"100123",false},
        {5,"10004",false},     
        {5,"10012",false},{5,"10013",false},{5,"10112",false},{5,"10124",false},
    };

    int passed = 0;
    for (auto& t : tests) {
        vector<int> a = parsePolyString(t.coeffs);
        Polynomial f(a, t.p);
        bool irr = Polynomial::test1(f);
        bool ok = (irr == t.expected);
        cout << "ℤ" << t.p << "[x]: f(x)=" << t.coeffs
             << " => " << (irr ? "неприводим" : "приводим")
             << (ok ? " ✅" : " ❌") << "\n";
        if (ok) ++passed;
    }

    cout << "\nПройдено тестов: " << passed << " из " << tests.size() << "\n";

    passed = 0;
    for (auto& t : tests) {
        vector<int> a = parsePolyString(t.coeffs);
        Polynomial f(a, t.p);
        bool irr = Polynomial::test2(f);
        bool ok = (irr == t.expected);
        cout << "ℤ" << t.p << "[x]: f(x)=" << t.coeffs
             << " => " << (irr ? "неприводим" : "приводим")
             << (ok ? " ✅" : " ❌") << "\n";
        if (ok) ++passed;
    }

    cout << "\nПройдено тестов: " << passed << " из " << tests.size() << "\n";
    return 0;
}