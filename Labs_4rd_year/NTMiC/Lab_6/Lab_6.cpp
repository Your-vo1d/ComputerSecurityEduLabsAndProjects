#include <iostream>
#include <random>
#include <tuple>
#include <vector>

class DiscreteLogSolver {
private:
    long long p, g, n;

    // Расширенный алгоритм Евклида
    std::tuple<long long, long long, long long> extended_gcd(long long a, long long b) {
        if (b == 0) {
            return {a, 1, 0};
        }
        auto [gcd_val, x1, y1] = extended_gcd(b, a % b);
        long long x = y1;
        long long y = x1 - (a / b) * y1;
        return {gcd_val, x, y};
    }

    // Функция F для ρ-метода Полларда
    void function_F(long long& x, long long& u, long long& v, long long a) {
        int partition = x % 3;
        
        if (partition == 0) {
            // x ∈ S1: x_{i+1} = a * x_i
            x = (a * x) % p;
            // u не меняется
            v = (v + 1) % n;
        } else if (partition == 1) {
            // x ∈ S2: x_{i+1} = x_i^2
            x = (x * x) % p;
            u = (2 * u) % n;
            v = (2 * v) % n;
        } else {
            // x ∈ S3: x_{i+1} = g * x_i
            x = (g * x) % p;
            u = (u + 1) % n;
            // v не меняется
        }
    }

public:
    DiscreteLogSolver(long long prime, long long generator) 
        : p(prime), g(generator), n(prime - 1) {}

    // Быстрое возведение в степень по модулю (публичный метод)
    long long pow_mod(long long base, long long exp, long long mod) {
        long long result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp & 1) {
                result = (result * base) % mod;
            }
            base = (base * base) % mod;
            exp >>= 1;
        }
        return result;
    }

    long long compute_log(long long a, int max_attempts = 20) {
        a %= p;
        if (a == 0) return -1;
        if (a == 1) return 0;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<long long> dis(0, n - 1);

        for (int attempt = 0; attempt < max_attempts; ++attempt) {
            long long x1, x2, u1, u2, v1, v2;
            
            if (attempt == 0) {
                // Первая попытка с начальными значениями
                x1 = x2 = 1;
                u1 = u2 = v1 = v2 = 0;
            } else {
                // Случайная инициализация при повторных попытках
                u1 = dis(gen);
                v1 = dis(gen);
                x1 = (pow_mod(g, u1, p) * pow_mod(a, v1, p)) % p;
                x2 = x1;
                u2 = u1;
                v2 = v1;
            }

            // Поиск коллизии методом Флойда
            do {
                function_F(x1, u1, v1, a);
                function_F(x2, u2, v2, a);
                function_F(x2, u2, v2, a);
            } while (x1 != x2);

            // Проверка найденной коллизии
            long long r = (v1 - v2) % n;
            if (r < 0) r += n;

            if (r == 0) {
                continue; // Отказ, пробуем снова
            }

            long long u_diff = (u2 - u1) % n;
            if (u_diff < 0) u_diff += n;

            // Решение сравнения r*z ≡ u_diff (mod n)
            auto [d, x_gcd, y_gcd] = extended_gcd(r, n);
            
            if (d <= 0 || d >= n) continue;
            if (u_diff % d != 0) continue;

            // Приведение уравнения
            long long r_reduced = r / d;
            long long u_diff_reduced = u_diff / d;
            long long n_reduced = n / d;

            // Поиск обратного элемента
            auto [gcd_val, inv, _] = extended_gcd(r_reduced, n_reduced);
            if (gcd_val != 1) continue;

            inv = (inv % n_reduced + n_reduced) % n_reduced;
            long long z0 = (u_diff_reduced * inv) % n_reduced;
            if (z0 < 0) z0 += n_reduced;

            // Проверка всех возможных решений
            for (long long i = 0; i < d; ++i) {
                long long zi = (z0 + i * n_reduced) % n;
                if (pow_mod(g, zi, p) == a) {
                    return zi;
                }
            }
        }

        return -1; // Решение не найдено после всех попыток
    }
};

// Функция для проверки решения
void verify_solution(long long p, long long g, long long a, long long result) {
    if (result == -1) {
        std::cout << "Решение не найдено" << std::endl;
        return;
    }
    
    DiscreteLogSolver verifier(p, g);
    long long check = verifier.pow_mod(g, result, p);
    std::cout << result << " (проверка: " << g << "^" << result << " = " << check << " mod " << p;
    if (check == a) {
        std::cout << " ✓)" << std::endl;
    } else {
        std::cout << " ✗, ожидалось: " << a << ")" << std::endl;
    }
}

int main() {
    std::cout << "=== Тестирование ρ-метода Полларда для дискретного логарифмирования ===" << std::endl;
    
    // 1) Группа ℤ*599 = <22>
    std::cout << "\n1) Группа ℤ*599 = <22>:" << std::endl;
    DiscreteLogSolver solver1(599, 22);
    std::cout << "log22 418 = "; 
    verify_solution(599, 22, 418, solver1.compute_log(418));
    std::cout << "log22 183 = ";
    verify_solution(599, 22, 183, solver1.compute_log(183));

    // 2) Группа ℤ*599 = <70>
    std::cout << "\n2) Группа ℤ*599 = <70>:" << std::endl;
    DiscreteLogSolver solver2(599, 70);
    std::cout << "log70 21 = ";
    verify_solution(599, 70, 21, solver2.compute_log(21));
    std::cout << "log70 269 = ";
    verify_solution(599, 70, 269, solver2.compute_log(269));

    // 3) Группа ℤ*1571 = <52>
    std::cout << "\n3) Группа ℤ*1571 = <52>:" << std::endl;
    DiscreteLogSolver solver3(1571, 52);
    std::cout << "log52 647 = ";
    verify_solution(1571, 52, 647, solver3.compute_log(647));
    std::cout << "log52 190 = ";
    verify_solution(1571, 52, 190, solver3.compute_log(190));

    // 4) Группа ℤ*1571 = <1003>
    std::cout << "\n4) Группа ℤ*1571 = <1003>:" << std::endl;
    DiscreteLogSolver solver4(1571, 1003);
    std::cout << "log1003 912 = ";
    verify_solution(1571, 1003, 912, solver4.compute_log(912));
    std::cout << "log1003 25 = ";
    verify_solution(1571, 1003, 25, solver4.compute_log(25));

    // 5) Группа ℤ*10069 = <77>
    std::cout << "\n5) Группа ℤ*10069 = <77>:" << std::endl;
    DiscreteLogSolver solver5(10069, 77);
    std::cout << "log77 625 = ";
    verify_solution(10069, 77, 625, solver5.compute_log(625));
    std::cout << "log77 7128 = ";
    verify_solution(10069, 77, 7128, solver5.compute_log(7128));

    // 6) Группа ℤ*10069 = <8900>
    std::cout << "\n6) Группа ℤ*10069 = <8900>:" << std::endl;
    DiscreteLogSolver solver6(10069, 8900);
    std::cout << "log8900 9458 = ";
    verify_solution(10069, 8900, 9458, solver6.compute_log(9458));
    std::cout << "log8900 7330 = ";
    verify_solution(10069, 8900, 7330, solver6.compute_log(7330));

    return 0;
}