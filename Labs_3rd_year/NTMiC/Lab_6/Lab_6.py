import random
import math
from collections import Counter
from sympy import factorint


def euler_phi(n, factors):
    """Вычисление функции Эйлера для n с учётом его факторизации."""
    result = n
    for p, _ in factors:
        result = result // p * (p - 1)
    return result

def is_prime_lucas(n, t, factors):
    """Проверка числа n на простоту методом Люка с выводом вероятности ошибки."""
    if n <= 1:
        print(f"{n} не является простым (n ≤ 1)")
        return False
    if n == 2 or n == 3:
        print(f"{n} - простое число")
        return True
    
    # Факторизуем n-1
    n_minus_1 = n - 1
    
    for _ in range(t):
        a = random.randint(2, n - 2)
        
        # Проверка малой теоремы Ферма
        if pow(a, n_minus_1, n) != 1:
            print(f"Тест Ферма не пройден для a = {a}")
            return False
        
        # Проверка условий Люка для каждого простого делителя
        all_conditions_met = True
        for p, _ in factors:
            exponent = n_minus_1 // p
            if pow(a, exponent, n) == 1:
                all_conditions_met = False
                break
        
        if all_conditions_met:
            print(f"Все условия Люка выполнены для a = {a}")
            return True
    
    return False

# Примеры использования:
n = int(input("Введите число:"))
t = int(input("Введите параметр надежности:"))
print(f"\nПроверка числа {n}:")
n_minus_1 = n - 1
factors = factorint(n_minus_1)
factor_list = list(factors.items())
result = is_prime_lucas(n, t, factor_list)
print(f"Результат: {'Простое' if result else 'Составное'}")
if (not(result)):
    phi = euler_phi(n_minus_1, factor_list)
    p = phi / n_minus_1
    error_prob = (1 - p) ** t
    print(f"Вероятность ошибки: {error_prob:.6f}")
