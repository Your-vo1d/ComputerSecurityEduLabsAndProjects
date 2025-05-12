import random
import math
import sympy

def is_prime(n, k=5):
    """Проверка числа на простоту с использованием теста Миллера-Рабина."""
    if n <= 1:
        return False
    elif n <= 3:
        return True
    elif n % 2 == 0:
        return False

    # Запись n-1 в виде (2^s * d)
    d = n - 1
    s = 0
    while d % 2 == 0:
        d //= 2
        s += 1

    # Проведение k тестов
    for _ in range(k):
        a = random.randint(2, n - 2)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for __ in range(s - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def generate_strong_prime_1984(bit_length):
    """Генерация сильного простого числа по методу Гордона (1984)."""
    # 1. Генерация s и t — больших простых чисел
    half_bits = bit_length // 2
    s = sympy.randprime(2**(half_bits - 1), 2**half_bits)
    t = sympy.randprime(2**(half_bits - 1), 2**half_bits)

    # 2. Поиск простого r = 2*i*t + 1
    i = random.randint(2**20, 2**30)
    while True:
        r_candidate = 2 * i * t + 1
        if is_prime(r_candidate):
            r = r_candidate
            break
        i += 1

    # 3. Вычисление p0 = 2*(s^{r-2} mod r)*s - 1
    term = pow(s, r - 2, r)
    p0 = 2 * term * s - 1

    # 4. Поиск простого p = 2*j*r*s + p0
    j = random.randint(2**20, 2**30)
    while True:
        p_candidate = 2 * j * r * s + p0
        if is_prime(p_candidate):
            return p_candidate
        j += 1

# Пример использования
if __name__ == "__main__":
    bit_length = int(input("Введите длину:"))
    strong_prime = generate_strong_prime_1984(bit_length)
    print(f"Сгенерированное сильное простое число ({bit_length} бит):\n{strong_prime}")
    print(f"Длина в битах: {strong_prime.bit_length()}")