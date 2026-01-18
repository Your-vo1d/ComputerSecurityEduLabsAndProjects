#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
ФАКТОРИЗАЦИЯ ПОЛИНОМОВ НАД КОНЕЧНЫМ ПОЛЕМ ℤp

Реализация алгоритмов:
1. Освобождение от квадратов (Square-Free)
2. Разложение на неприводимые множители (Berlekamp)

Автор: Криптографический анализ
"""

import numpy as np
from typing import List, Tuple

class PolynomialFactorizer:
    """Факторизация полиномов над ℤp"""
    
    def __init__(self, p: int):
        """
        Инициализация
        p: характеристика поля ℤp
        """
        if not isinstance(p, int) or p < 2:
            raise ValueError("p должно быть целым числом >= 2")
        self.p = p
    
    def log(self, msg: str, level: int = 0) -> None:
        """Вывод с форматированием"""
        indent = "  " * level
        print(f"{indent}{msg}")
    
    def normalize_poly(self, poly: List[int]) -> List[int]:
        """
        Нормализация полинома: удаление нулевых старших коэффициентов
        """
        while len(poly) > 1 and poly[-1] % self.p == 0:
            poly = poly[:-1]
        return poly if poly else [0]
    
    def poly_to_str(self, poly: List[int]) -> str:
        """
        Представление полинома в виде строки
        Пример: [1, 0, 3, 1] → "x^3 + 3x^2 + 1"
        """
        if len(poly) == 1 and poly[0] == 0:
            return "0"
        
        terms = []
        for i, coeff in enumerate(poly):
            coeff = coeff % self.p
            if coeff == 0:
                continue
            
            if i == 0:
                terms.append(str(coeff))
            elif i == 1:
                terms.append(f"{coeff}x" if coeff != 1 else "x")
            else:
                terms.append(f"{coeff}x^{i}" if coeff != 1 else f"x^{i}")
        
        return " + ".join(reversed(terms)) if terms else "0"
    
    def poly_degree(self, poly: List[int]) -> int:
        """Вычисление степени полинома"""
        poly = self.normalize_poly(poly)
        return len(poly) - 1
    
    def poly_add(self, p1: List[int], p2: List[int]) -> List[int]:
        """
        Сложение двух полиномов в ℤp[x]
        """
        max_len = max(len(p1), len(p2))
        result = [0] * max_len
        for i in range(len(p1)):
            result[i] += p1[i]
        for i in range(len(p2)):
            result[i] += p2[i]
        return self.normalize_poly([x % self.p for x in result])
    
    def poly_mult(self, p1: List[int], p2: List[int]) -> List[int]:
        """
        Умножение двух полиномов в ℤp[x]
        """
        if any((p == [0] or (len(p) == 1 and p[0] == 0)) for p in [p1, p2]):
            return [0]
        
        result = [0] * (len(p1) + len(p2) - 1)
        for i in range(len(p1)):
            for j in range(len(p2)):
                result[i + j] = (result[i + j] + p1[i] * p2[j]) % self.p
        return self.normalize_poly(result)
    
    def poly_mod(self, dividend: List[int], divisor: List[int]) -> List[int]:
        """
        Остаток от деления двух полиномов
        """
        divisor = self.normalize_poly(divisor)
        if len(divisor) == 1 and divisor[0] == 0:
            raise ValueError("Деление на нулевой полином")
        
        remainder = list(dividend)
        while len(remainder) >= len(divisor):
            inv = pow(divisor[-1] % self.p, -1, self.p)
            coeff = (remainder[-1] * inv) % self.p
            for i in range(len(divisor)):
                remainder[-(i+1)] = (remainder[-(i+1)] - coeff * divisor[-(i+1)]) % self.p
            remainder.pop()
        
        return self.normalize_poly(remainder)
    
    def poly_divmod(self, dividend: List[int], divisor: List[int]) -> Tuple[List[int], List[int]]:
        """
        Деление с остатком: возвращает (quotient, remainder)
        """
        divisor = self.normalize_poly(divisor)
        if len(divisor) == 1 and divisor[0] == 0:
            raise ValueError("Деление на нулевой полином")
        
        quotient = []
        remainder = list(dividend)
        
        while len(remainder) >= len(divisor):
            inv = pow(divisor[-1] % self.p, -1, self.p)
            coeff = (remainder[-1] * inv) % self.p
            quotient.append(coeff)
            
            for i in range(len(divisor)):
                remainder[-(i+1)] = (remainder[-(i+1)] - coeff * divisor[-(i+1)]) % self.p
            remainder.pop()
        
        return (list(reversed(quotient)) if quotient else [0], self.normalize_poly(remainder))
    
    def poly_gcd(self, p1: List[int], p2: List[int]) -> List[int]:
        """
        НОД двух полиномов (алгоритм Евклида)
        """
        p1 = self.normalize_poly(p1)
        p2 = self.normalize_poly(p2)
        
        while not (len(p2) == 1 and p2[0] == 0):
            p1, p2 = p2, self.poly_mod(p1, p2)
        
        return p1
    
    def poly_derivative(self, poly: List[int]) -> List[int]:
        """
        Формальная производная полинома
        f(x) = a_n*x^n + ... + a_1*x + a_0 → f'(x) = n*a_n*x^(n-1) + ... + a_1
        """
        if len(poly) <= 1:
            return [0]
        
        return self.normalize_poly([(i * poly[i]) % self.p for i in range(1, len(poly))])
    
    def square_free(self, poly: List[int]) -> List[Tuple[List[int], int]]:
        """
        Алгоритм освобождения от квадратов (Square-Free)
        
        Возвращает список кортежей (полином, степень)
        такой что f(x) = h_1(x)^e_1 * ... * h_k(x)^e_k
        где каждый h_i свободен от квадратов
        """
        self.log("=" * 70)
        self.log("АЛГОРИТМ ОСВОБОЖДЕНИЯ ОТ КВАДРАТОВ (SQUARE-FREE)")
        self.log("=" * 70)
        
        poly = self.normalize_poly(poly)
        self.log(f"Исходный полином: f(x) = {self.poly_to_str(poly)}")
        self.log(f"Степень: deg(f) = {self.poly_degree(poly)}")
        self.log("")
        
        return self._square_free_recursive(poly, 1)
    
    def _square_free_recursive(self, f: List[int], exponent: int) -> List[Tuple[List[int], int]]:
        """
        Рекурсивная часть алгоритма освобождения от квадратов
        """
        self.log(f"Square-Free(f, exp={exponent})")
        self.log(f"  f(x) = {self.poly_to_str(f)}", 1)
        
        # Шаг 1: проверка степени
        if self.poly_degree(f) < 2:
            self.log(f"  deg(f) < 2, возврат", 1)
            return [(f, exponent)]
        
        # Шаг 2: вычисление производной
        g = self.poly_derivative(f)
        self.log(f"  f'(x) = {self.poly_to_str(g)}", 1)
        
        # Шаг 3: проверка нулевой производной
        if len(g) == 1 and g[0] == 0:
            self.log(f"  f'(x) = 0 ⟹ f(x) = (v(x))^{self.p}", 1)
            v = self.normalize_poly([f[i] for i in range(0, len(f), self.p)])
            self.log(f"  v(x) = {self.poly_to_str(v)}", 1)
            
            return self._square_free_recursive(v, exponent * self.p)
        
        # Шаг 4: вычисление НОД
        d = self.poly_gcd(g, f)
        self.log(f"  НОД(f'(x), f(x)) = {self.poly_to_str(d)}", 1)
        
        # Шаг 5: если НОД = 1
        if len(d) == 1 and d[0] == 1:
            self.log(f"  НОД = 1, полином свободен от квадратов", 1)
            return [(f, exponent)]
        
        # Иначе: разложение
        self.log(f"  НОД ≠ 1, выполняем деление f(x) / НОД", 1)
        h, _ = self.poly_divmod(f, d)
        h = self.normalize_poly(h)
        self.log(f"  h(x) = f(x) / НОД = {self.poly_to_str(h)}", 1)
        
        # Рекурсивные вызовы
        d_result = self._square_free_recursive(d, exponent)
        h_result = [(h, exponent)]
        
        result = d_result + h_result
        self.log(f"  Результат: {[(self.poly_to_str(p), e) for p, e in result]}", 1)
        
        return result
    
    def berlekamp(self, poly: List[int]) -> List[List[int]]:
        """
        Алгоритм Берлекэмпа для разложения свободного от квадратов полинома
        на неприводимые множители
        
        Возвращает список неприводимых полиномов
        """
        self.log("\n" + "=" * 70)
        self.log("АЛГОРИТМ БЕРЛЕКЭМПА")
        self.log("=" * 70)
        
        poly = self.normalize_poly(poly)
        n = self.poly_degree(poly)
        
        self.log(f"Исходный полином: f(x) = {self.poly_to_str(poly)}")
        self.log(f"Степень: n = {n}")
        self.log(f"Поле: ℤ_{self.p}")
        self.log("")
        
        # Проверка, свободен ли от квадратов
        g = self.poly_derivative(poly)
        d = self.poly_gcd(poly, g)
        if not (len(d) == 1 and d[0] == 1):
            self.log("ОШИБКА: Полином не свободен от квадратов!")
            self.log("Примените сначала Square-Free")
            return None
        
        # Шаг 1: Построение матрицы Q
        self.log("Шаг 1: Построение матрицы Q")
        self.log("-" * 60)
        
        powers = []
        current = [1]  # x^0
        
        for j in range(n):
            self.log(f"j = {j}: вычислить x^({self.p}·{j}) mod f(x)", 1)
            
            if j == 0:
                power_mod = [1]
            else:
                power_mod = current
                for _ in range(self.p - 1):
                    power_mod = self.poly_mult(power_mod, current)
                    power_mod = self.poly_mod(power_mod, poly)
                
                if j < n - 1:
                    current = self.poly_mult(power_mod, [0, 1])  # Умножить на x
                    current = self.poly_mod(current, poly)
            
            powers.append(power_mod)
            coeffs = list(power_mod) + [0] * (n - len(power_mod))
            self.log(f"  x^({self.p}·{j}) ≡ {self.poly_to_str(power_mod)} (mod f)", 2)
            self.log(f"  коэффициенты: {coeffs[:n]}", 2)
        
        # Создать матрицу Q
        Q = []
        for power_poly in powers:
            coeffs = list(power_poly) + [0] * (n - len(power_poly))
            Q.append(coeffs[:n])
        
        Q = np.array(Q, dtype=int)
        
        self.log("\nМатрица Q:")
        for i, row in enumerate(Q):
            self.log(f"Q[{i}] = {list(row)}", 1)
        
        # Шаг 2: Вычисление Q - E
        self.log("\nШаг 2: Вычисление матрицы (Q - E) mod p")
        self.log("-" * 60)
        
        E = np.eye(n, dtype=int)
        Q_minus_E = (Q - E) % self.p
        
        self.log("Матрица (Q - E) mod p:")
        for i, row in enumerate(Q_minus_E):
            self.log(f"[{i}] = {list(row)}", 1)
        
        # Шаг 3: Поиск ядра
        self.log("\nШаг 3: Поиск базиса нулевого пространства (ядро Q - E)")
        self.log("-" * 60)
        
        kernel = self._find_kernel_mod_p(Q_minus_E)
        k = len(kernel)
        
        self.log(f"Размерность ядра: k = {k}")
        self.log(f"Базис ядра (решения h(x)):")
        
        h_polynomials = []
        for idx, sol in enumerate(kernel):
            poly_h = self.normalize_poly(list(sol))
            h_polynomials.append(poly_h)
            self.log(f"h^({idx+1})(x) = {self.poly_to_str(poly_h)}", 1)
        
        if k == 1:
            self.log("\nk = 1, полином неразложимый (неприводимый)")
            return [poly]
        
        # Шаг 4: Поиск делителей
        self.log("\nШаг 4: Поиск делителей полинома")
        self.log("-" * 60)
        
        factors = []
        
        for i in range(1, k):
            self.log(f"\nОбработка h^({i+1})(x) = {self.poly_to_str(h_polynomials[i])}", 1)
            
            for a in range(self.p):
                h_minus_a = self.poly_add(h_polynomials[i], [-a])
                divisor = self.poly_gcd(poly, h_minus_a)
                divisor = self.normalize_poly(divisor)
                
                if self.poly_degree(divisor) > 0:
                    if not (len(divisor) == 1 and divisor[0] == 1):
                        is_new = True
                        for existing in factors:
                            if self.polys_equal(divisor, existing):
                                is_new = False
                                break
                        
                        if is_new:
                            factors.append(divisor)
                            self.log(f"  a = {a}: НОД = {self.poly_to_str(divisor)}", 2)
        
        if len(factors) == 0:
            factors = [poly]
        
        self.log(f"\nНайдено делителей: {len(factors)}")
        for idx, factor in enumerate(factors):
            self.log(f"f_{idx+1}(x) = {self.poly_to_str(factor)}", 1)
        
        return factors
    
    def polys_equal(self, p1: List[int], p2: List[int]) -> bool:
        """Проверка равенства двух полиномов"""
        p1 = self.normalize_poly(p1)
        p2 = self.normalize_poly(p2)
        
        if len(p1) != len(p2):
            return False
        
        for i in range(len(p1)):
            if p1[i] % self.p != p2[i] % self.p:
                return False
        
        return True
    
    def _find_kernel_mod_p(self, matrix: np.ndarray) -> List[np.ndarray]:
        """
        Найти базис ядра матрицы над ℤ_p
        Решаем X * A = 0 (mod p)
        """
        A = np.array(matrix, dtype=int) % self.p
        m, n = A.shape
        
        # Гауссово исключение
        M = A.copy()
        pivot_row = 0
        pivot_cols = []
        
        for col in range(n):
            # Найти ненулевой элемент в этом столбце
            found = False
            for row in range(pivot_row, m):
                if M[row, col] % self.p != 0:
                    M[[pivot_row, row]] = M[[row, pivot_row]]
                    found = True
                    break
            
            if not found:
                continue
            
            pivot_cols.append(col)
            
            # Сделать диагональный элемент равным 1
            inv = pow(int(M[pivot_row, col]), -1, self.p)
            M[pivot_row] = (M[pivot_row] * inv) % self.p
            
            # Исключить другие элементы в этом столбце
            for row in range(m):
                if row != pivot_row and M[row, col] % self.p != 0:
                    factor = M[row, col]
                    M[row] = (M[row] - factor * M[pivot_row]) % self.p
            
            pivot_row += 1
        
        # Свободные переменные
        free_vars = [i for i in range(n) if i not in pivot_cols]
        
        # Построить базис ядра
        kernel_basis = []
        
        for free_var in free_vars:
            solution = np.zeros(n, dtype=int)
            solution[free_var] = 1
            
            for i, pivot_col in enumerate(pivot_cols):
                if i < m:
                    solution[pivot_col] = (-M[i, free_var]) % self.p
            
            kernel_basis.append(solution % self.p)
        
        # Добавить h^(1) = 1 в начало
        h1 = np.zeros(n, dtype=int)
        h1[0] = 1
        kernel_basis.insert(0, h1)
        
        return kernel_basis


def demo():
    """Демонстрация алгоритмов на примерах из учебника"""
    
    print("\n" + "╔" + "="*68 + "╗")
    print("║" + " ФАКТОРИЗАЦИЯ ПОЛИНОМОВ НАД КОНЕЧНЫМ ПОЛЕМ ℤp".center(68) + "║")
    print("║" + " Примеры из учебника ".center(68) + "║")
    print("╚" + "="*68 + "╝\n")
    
    # ПРИМЕР 1
    print("\n" + "="*70)
    print("ПРИМЕР 1: x⁷ + x⁶ + x⁵ + x⁴ + x² + 1 в ℤ₂[x]")
    print("="*70 + "\n")
    
    factorizer = PolynomialFactorizer(2)
    poly1 = [1, 0, 1, 0, 1, 1, 1, 1]  # коэффициенты от x^0 к x^n
    
    result1 = factorizer.square_free(poly1)
    print("\n" + "─"*70)
    print("ИТОГОВОЕ РАЗЛОЖЕНИЕ:")
    print("─"*70)
    formula = " · ".join(f"({factorizer.poly_to_str(p)})^{e}" for p, e in result1)
    print(f"f(x) = {formula}\n")
    
    
    # ПРИМЕР 2
    print("\n" + "="*70)
    print("ПРИМЕР 2: x⁵ + 5x³ + 4x² + 2x + 4 в ℤ₇[x]")
    print("="*70 + "\n")
    
    factorizer = PolynomialFactorizer(7)
    poly2 = [4, 2, 4, 5, 0, 1]  # f(x) = x^5 + 5x^3 + 4x^2 + 2x + 4
    
    result2 = factorizer.square_free(poly2)
    print("\n" + "─"*70)
    print("ИТОГОВОЕ РАЗЛОЖЕНИЕ:")
    print("─"*70)
    formula = " · ".join(f"({factorizer.poly_to_str(p)})^{e}" for p, e in result2)
    print(f"f(x) = {formula}\n")


def interactive_mode():
    """Интерактивный режим"""
    
    print("\n" + "╔" + "="*68 + "╗")
    print("║" + " ИНТЕРАКТИВНЫЙ РЕЖИМ ".center(68) + "║")
    print("╚" + "="*68 + "╝\n")
    
    while True:
        try:
            p = int(input("Введите характеристику поля p (или 0 для выхода): "))
            if p == 0:
                break
            
            factorizer = PolynomialFactorizer(p)
            
            print("\nВведите коэффициенты полинома f(x) (от x^0 к x^n)")
            print("Пример: для x^3 + 2x + 1 введите: 1 2 0 1")
            coeffs_str = input("Коэффициенты: ")
            poly = list(map(int, coeffs_str.split()))
            
            print("\n1. Освобождение от квадратов (Square-Free)")
            print("2. Разложение на неприводимые множители (Berlekamp)")
            choice = input("Выберите (1 или 2): ")
            
            if choice == "1":
                result = factorizer.square_free(poly)
                print("\n" + "─"*70)
                print("РЕЗУЛЬТАТ:")
                print("─"*70)
                formula = " · ".join(f"({factorizer.poly_to_str(p)})^{e}" for p, e in result)
                print(f"f(x) = {formula}\n")
            
            elif choice == "2":
                # Сначала освободить от квадратов
                print("\nСначала применим Square-Free...")
                sf_result = factorizer.square_free(poly)
                
                print("\n" + "="*70)
                print("ТЕПЕРЬ ПРИМЕНИМ BERLEKAMP К СВОБОДНЫМ ОТ КВАДРАТОВ КОМПОНЕНТАМ")
                print("="*70)
                
                for sf_poly, exp in sf_result:
                    print(f"\n\nОбработка компоненты: ({factorizer.poly_to_str(sf_poly)})^{exp}")
                    
                    if factorizer.poly_degree(sf_poly) <= 1:
                        print("Это линейный полином, не требует разложения")
                        continue
                    
                    factors = factorizer.berlekamp(sf_poly)
                    if factors:
                        print("\n" + "─"*70)
                        print("НЕПРИВОДИМЫЕ МНОЖИТЕЛИ:")
                        print("─"*70)
                        for i, factor in enumerate(factors, 1):
                            print(f"f_{i}(x) = {factorizer.poly_to_str(factor)}")
        
        except ValueError as e:
            print(f"Ошибка: {e}")
        except Exception as e:
            print(f"Ошибка: {e}")
        
        print("\n")


if __name__ == "__main__":
    import sys
    
    if len(sys.argv) > 1 and sys.argv[1] == "demo":
        demo()
    else:
        demo()
        print("\n" + "="*70)
        print("Для интерактивного режима запустите: python poly_factorizer.py interactive")
        print("="*70)
