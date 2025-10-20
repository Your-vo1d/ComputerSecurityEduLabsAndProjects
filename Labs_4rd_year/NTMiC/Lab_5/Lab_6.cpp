#include <bits/stdc++.h>
using namespace std;

class DiscreteLogSolver {
private:
    long long p, g, n;
    long long pow_mod(long long base, long long exp, long long mod) {
        long long res = 1;
        base %= mod;
        if (base < 0) base += mod;
        while (exp > 0) {
            if (exp & 1) res = (res * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        return res;
    }
public:
    DiscreteLogSolver(long long prime, long long generator) : p(prime), g(generator), n(prime - 1) {}
    long long compute_log(long long a) {
        a %= p;
        if (a == 0) return -1; // invalid
        if (a == 1) return 0;
        long long h = ceil(sqrt((double)n));
        long long b = pow_mod(g, h, p);
        map<long long, int> giant;
        long long current = b;
        for (int u = 1; u <= h; ++u) {
            giant[current] = u;
            current = (current * b) % p;
        }
        map<long long, int> baby;
        long long gv = g;
        for (int v = 1; v <= h; ++v) {
            long long val = (a * gv) % p;
            baby[val] = v;
            gv = (gv * g) % p;
        }
        for (auto& pair : baby) {
            long long val = pair.first;
            auto it = giant.find(val);
            if (it != giant.end()) {
                int u = it->second;
                int v = pair.second;
                long long x = ((long long) h * u - v) % n;
                return x;
            }
        }
        return -1; // not found
    }
};

int main() {
    // 1) Группа ℤ*599 = <22>
    DiscreteLogSolver solver1(599, 22);
    cout << "log22 418 = " << solver1.compute_log(418) << endl;
    cout << "log22 183 = " << solver1.compute_log(183) << endl;

    // 2) Группа ℤ*599 = <70>
    DiscreteLogSolver solver2(599, 70);
    cout << "log70 21 = " << solver2.compute_log(21) << endl;
    cout << "log70 269 = " << solver2.compute_log(269) << endl;

    // 3) Группа ℤ*1571 = <52>
    DiscreteLogSolver solver3(1571, 52);
    cout << "log52 647 = " << solver3.compute_log(647) << endl;
    cout << "log52 190 = " << solver3.compute_log(190) << endl;

    // 4) Группа ℤ*1571 = <1003>
    DiscreteLogSolver solver4(1571, 1003);
    cout << "log1003 912 = " << solver4.compute_log(912) << endl;
    cout << "log1003 25 = " << solver4.compute_log(25) << endl;

    // 5) Группа ℤ*10069 = <77>
    DiscreteLogSolver solver5(10069, 77);
    cout << "log77 625 = " << solver5.compute_log(625) << endl;
    cout << "log77 7128 = " << solver5.compute_log(7128) << endl;

    // 6) Группа ℤ*10069 = <8900>
    DiscreteLogSolver solver6(10069, 8900);
    cout << "log8900 9458 = " << solver6.compute_log(9458) << endl;
    cout << "log8900 7330 = " << solver6.compute_log(7330) << endl;

    return 0;
}