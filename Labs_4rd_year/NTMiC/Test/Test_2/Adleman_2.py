import random
import math
from typing import List, Tuple, Optional, Dict
import sympy

class AdlemanAlgorithm:
    def __init__(self, g: int, a: int, n: int, group_order: int):
        """
        Инициализация алгоритма Адлемана
        
        Args:
            g: генератор группы
            a: элемент, для которого ищем логарифм
            n: модуль (простое число)
            group_order: порядок группы (n-1 для группы Z_p*)
        """
        self.g = g % n
        self.a = a % n
        self.n = n
        self.group_order = group_order
        self.factor_base = []
        self.logs = {}  # словарь для хранения log_g(p_i)
        self.k_counter = 1  # счетчик для последовательного выбора k
        
    def select_factor_base(self, t: int) -> List[int]:
        """
        Выбор факторной базы из t первых простых чисел
        
        Args:
            t: размер факторной базы
            
        Returns:
            Список простых чисел факторной базы
        """
        primes = []
        i = 2
        while len(primes) < t:
            if sympy.isprime(i):
                primes.append(i)
            i += 1
        self.factor_base = primes
        return primes
    
    def factor_in_base(self, num: int) -> Optional[Dict[int, int]]:
        """
        Разложение числа в факторной базе
        
        Args:
            num: число для разложения
            
        Returns:
            Словарь {простое: степень} или None, если нельзя разложить
        """
        if num <= 1:
            return None
            
        factors = {}
        temp = num
        
        for p in self.factor_base:
            if temp % p == 0:
                count = 0
                while temp % p == 0:
                    temp //= p
                    count += 1
                factors[p] = count
                
        if temp == 1:
            return factors
        return None
    
    def modular_pow(self, base: int, exponent: int, modulus: int) -> int:
        """Быстрое возведение в степень по модулю"""
        result = 1
        base = base % modulus
        
        while exponent > 0:
            if exponent % 2 == 1:
                result = (result * base) % modulus
            exponent = exponent >> 1
            base = (base * base) % modulus
            
        return result
    
    def build_linear_system(self, t: int) -> Tuple[List[List[int]], List[int], List[Dict]]:
        """
        Построение системы линейных уравнений
        
        Args:
            t: размер факторной базы
            
        Returns:
            Матрица коэффициентов, вектор правых частей и список разложений
        """
        equations = []
        right_sides = []
        factorizations = []  # сохраняем разложения для вывода
        k_values = []  # сохраняем значения k
        
        print(f"Построение системы уравнений...")
        print(f"Факторная база S = {self.factor_base}")
        print("-" * 60)
        
        # Сбрасываем счетчик k
        self.k_counter = 1
        
        while len(equations) < t:
            k = self.k_counter
            self.k_counter += 1
            
            # Вычисляем b = g^k mod n
            b = self.modular_pow(self.g, k, self.n)
            print(f"Попытка {len(equations)+1}: k = {k}")
            print(f"  Вычисляем b = g^k mod n = {self.g}^{k} mod {self.n}")
            print(f"  b = {b}")
            
            factors = self.factor_in_base(b)
            if factors:
                # Сохраняем информацию для вывода
                k_values.append(k)
                factorizations.append((b, factors))
                
                # Создаем уравнение: k ≡ Σ e_i * log_g(p_i) mod (n-1)
                coeffs = [0] * t
                
                for i, p in enumerate(self.factor_base):
                    if p in factors:
                        coeffs[i] = factors[p]
                
                equations.append(coeffs)
                right_sides.append(k % self.group_order)
                
                print(f"  ✓ Успех! b = {b} разлагается в факторной базе:")
                print(f"    {b} = ", end="")
                factors_str = " * ".join([f"{p}^{e}" for p, e in factors.items()])
                print(factors_str)
                print(f"    Получаем уравнение: {k} ≡ ", end="")
                
                # Формируем строку уравнения
                eq_parts = []
                for p, e in factors.items():
                    if e == 1:
                        eq_parts.append(f"log_g({p})")
                    else:
                        eq_parts.append(f"{e}·log_g({p})")
                print(" + ".join(eq_parts) + f" (mod {self.group_order})")
                
                print(f"    Коэффициенты: {coeffs}")
            else:
                print(f"  ✗ b = {b} не разлагается в факторной базе")
            
            print("-" * 40)
        
        # Выводим сводную информацию
        print("\n" + "=" * 60)
        print("СВОДНАЯ ИНФОРМАЦИЯ ПО УРАВНЕНИЯМ:")
        print("=" * 60)
        for i in range(len(equations)):
            k = k_values[i]
            b, factors = factorizations[i]
            print(f"\nУравнение {i+1}:")
            print(f"  k = {k}")
            print(f"  g^k = {self.g}^{k} = {b} mod {self.n}")
            print(f"  Разложение: {b} = ", end="")
            factors_str = " * ".join([f"{p}^{e}" for p, e in factors.items()])
            print(factors_str)
            print(f"  Уравнение: {k} ≡ ", end="")
            
            eq_parts = []
            for j, p in enumerate(self.factor_base):
                if equations[i][j] != 0:
                    e = equations[i][j]
                    if e == 1:
                        eq_parts.append(f"log_g({p})")
                    else:
                        eq_parts.append(f"{e}·log_g({p})")
            print(" + ".join(eq_parts) + f" (mod {self.group_order})")
        
        return equations, right_sides, factorizations
    
    def solve_linear_system(self, equations: List[List[int]], right_sides: List[int]) -> Dict[int, int]:
        """
        Решение системы линейных уравнений в поле Z_{group_order}
        
        Args:
            equations: матрица коэффициентов
            right_sides: вектор правых частей
            
        Returns:
            Словарь log_g(p_i)
        """
        t = len(self.factor_base)
        mod = self.group_order
        
        print("\n" + "=" * 60)
        print("РЕШЕНИЕ СИСТЕМЫ УРАВНЕНИЙ МЕТОДОМ ГАУССА:")
        print("=" * 60)
        
        # Создаем расширенную матрицу
        augmented = []
        for i in range(t):
            row = equations[i][:] + [right_sides[i]]
            augmented.append(row)
        
        print("\nИсходная расширенная матрица:")
        self.print_matrix(augmented, mod)
        
        # Прямой ход метода Гаусса
        for col in range(t):
            print(f"\n--- Шаг {col+1}: Обработка столбца {col+1} (переменная log_g(p_{col+1})) ---")
            
            # Ищем ненулевой элемент в текущем столбце, начиная с текущей строки
            pivot_row = -1
            for row in range(col, t):
                if augmented[row][col] % mod != 0:
                    pivot_row = row
                    break
            
            if pivot_row == -1:
                print(f"  Столбец {col+1} нулевой ниже диагонали, пропускаем")
                continue
            
            # Если pivot не на диагонали, меняем строки местами
            if pivot_row != col:
                print(f"  Меняем строки {col+1} и {pivot_row+1} местами")
                augmented[col], augmented[pivot_row] = augmented[pivot_row], augmented[col]
                print("  Матрица после перестановки:")
                self.print_matrix(augmented, mod)
            
            # Нормализуем ведущую строку
            pivot_val = augmented[col][col]
            print(f"  Ведущий элемент: {pivot_val}")
            
            # Находим обратный элемент по модулю
            if math.gcd(pivot_val, mod) != 1:
                print(f"  Внимание: {pivot_val} и {mod} не взаимно просты!")
                # Для простоты предполагаем, что это не произойдет в нашей задаче
                inv_pivot = 1
            else:
                inv_pivot = pow(pivot_val, -1, mod)
                print(f"  Обратный элемент к {pivot_val} mod {mod}: {inv_pivot}")
            
            # Умножаем ведущую строку на обратный элемент
            if inv_pivot != 1:
                print(f"  Умножаем строку {col+1} на {inv_pivot} mod {mod}")
                for j in range(col, t + 1):
                    augmented[col][j] = (augmented[col][j] * inv_pivot) % mod
                print("  Матрица после нормализации:")
                self.print_matrix(augmented, mod)
            
            # Обнуляем столбец в других строках
            for row in range(t):
                if row != col and augmented[row][col] != 0:
                    factor = augmented[row][col]
                    print(f"  Вычитаем из строки {row+1} строку {col+1}, умноженную на {factor}")
                    
                    for j in range(col, t + 1):
                        augmented[row][j] = (augmented[row][j] - factor * augmented[col][j]) % mod
                    
                    print(f"  После обнуления элемента в строке {row+1}, столбце {col+1}:")
                    self.print_matrix(augmented, mod)
        
        print("\n" + "=" * 60)
        print("МАТРИЦА В ПРИВЕДЕННОМ СТУПЕНЧАТОМ ВИДЕ:")
        print("=" * 60)
        self.print_matrix(augmented, mod)
        
        # Извлекаем решение
        logs = {}
        print("\n" + "=" * 60)
        print("РЕШЕНИЕ СИСТЕМЫ:")
        print("=" * 60)
        
        for i in range(t):
            log_val = augmented[i][t] % mod
            prime = self.factor_base[i]
            logs[prime] = log_val
            print(f"  log_g({prime}) = {log_val} mod {mod}")
            
            # Проверяем
            if self.modular_pow(self.g, log_val, self.n) != prime % self.n:
                print(f"    Проверка: g^{log_val} = {self.modular_pow(self.g, log_val, self.n)} ≠ {prime} mod {self.n}")
            else:
                print(f"    Проверка: g^{log_val} = {self.modular_pow(self.g, log_val, self.n)} = {prime} mod {self.n}")
        
        self.logs = logs
        return logs
    
    def print_matrix(self, matrix: List[List[int]], mod: int):
        """Печать матрицы в удобном формате"""
        t = len(matrix)
        for i in range(t):
            row_str = "  ["
            for j in range(t):
                row_str += f"{matrix[i][j]:3d} "
            row_str += "|"
            row_str += f"{matrix[i][t]:5d}"
            row_str += "]"
            
            # Добавляем комментарий с уравнением
            if i < len(self.factor_base):
                eq_parts = []
                for j in range(t):
                    if matrix[i][j] != 0:
                        e = matrix[i][j]
                        if e == 1:
                            eq_parts.append(f"log_g({self.factor_base[j]})")
                        else:
                            eq_parts.append(f"{e}·log_g({self.factor_base[j]})")
                if eq_parts:
                    row_str += f"   {matrix[i][t]} ≡ " + " + ".join(eq_parts) + f" (mod {mod})"
            
            print(row_str)
    
    def find_logarithm(self, max_attempts: int = 1000) -> Optional[int]:
        """
        Нахождение дискретного логарифма a по основанию g
        
        Args:
            max_attempts: максимальное количество попыток
            
        Returns:
            x = log_g(a) или None при неудаче
        """
        print(f"\n" + "=" * 60)
        print(f"ПОИСК log_{self.g}({self.a}) mod {self.n}:")
        print("=" * 60)
        
        # Сбрасываем счетчик k
        self.k_counter = 0
        
        attempts = 0
        while attempts < max_attempts:
            k = self.k_counter
            self.k_counter += 1
            
            # Вычисляем b = a * g^k mod n
            g_pow_k = self.modular_pow(self.g, k, self.n)
            b = (self.a * g_pow_k) % self.n
            
            print(f"\nПопытка {attempts+1}: k = {k}")
            print(f"  Вычисляем b = a * g^k mod n")
            print(f"  b = {self.a} * {self.g}^{k} mod {self.n}")
            print(f"  b = {self.a} * {g_pow_k} mod {self.n}")
            print(f"  b = {b}")
            
            factors = self.factor_in_base(b)
            if factors:
                print(f"  ✓ Успех! b = {b} разлагается в факторной базе:")
                print(f"    {b} = ", end="")
                factors_str = " * ".join([f"{p}^{e}" for p, e in factors.items()])
                print(factors_str)
                
                # Вычисляем x = Σ e_i * log_g(p_i) - k mod (n-1)
                print(f"  Вычисляем x = Σ e_i·log_g(p_i) - k (mod {self.group_order})")
                print(f"  x = (", end="")
                
                sum_logs = 0
                sum_parts = []
                for p, e in factors.items():
                    log_val = self.logs[p]
                    term = (e * log_val) % self.group_order
                    sum_logs = (sum_logs + term) % self.group_order
                    if e == 1:
                        sum_parts.append(f"{log_val}")
                    else:
                        sum_parts.append(f"{e}·{log_val}")
                
                print(" + ".join(sum_parts) + f") - {k} mod {self.group_order}")
                print(f"  x = {sum_logs} - {k} mod {self.group_order}")
                
                x = (sum_logs - k) % self.group_order
                print(f"  x = {x}")
                
                # Проверяем результат
                check = self.modular_pow(self.g, x, self.n)
                print(f"  Проверка: g^x = {self.g}^{x} mod {self.n} = {check}")
                print(f"  Ожидалось: a = {self.a}")
                
                if check == self.a % self.n:
                    print(f"  ✓ Проверка пройдена!")
                    return x
                else:
                    print(f"  ✗ Проверка не пройдена, продолжаем поиск...")
            else:
                print(f"  ✗ b = {b} не разлагается в факторной базе")
            
            attempts += 1
        
        print(f"\n✗ Не удалось найти логарифм за {max_attempts} попыток")
        return None
    
    def run(self, t: int = None) -> Optional[int]:
        """
        Запуск полного алгоритма Адлемана
        
        Args:
            t: размер факторной базы (если None, выбирается автоматически)
            
        Returns:
            Найденный дискретный логарифм
        """
        if t is None:
            # Эвристика для выбора размера факторной базы
            t = max(5, int(math.log(self.n, 2) // 4))
            t = min(t, 20)  # Ограничиваем для производительности
        
        print("=" * 80)
        print("АЛГОРИТМ АДЛЕМАНА ДЛЯ ДИСКРЕТНОГО ЛОГАРИФМИРОВАНИЯ")
        print("=" * 80)
        print(f"Группа: Z_{self.n}* (порядок {self.group_order})")
        print(f"Генератор: g = {self.g}")
        print(f"Элемент: a = {self.a}")
        print(f"Размер факторной базы: t = {t}")
        print("=" * 80)
        
        # Шаг 1: Выбор факторной базы
        print("\n" + "=" * 80)
        print("ШАГ 1: ВЫБОР ФАКТОРНОЙ БАЗЫ")
        print("=" * 80)
        self.select_factor_base(t)
        print(f"Факторная база S состоит из {t} первых простых чисел:")
        print(f"S = {self.factor_base}")
        
        # Шаг 2: Построение системы уравнений
        print("\n" + "=" * 80)
        print("ШАГ 2: ПОСТРОЕНИЕ СИСТЕМЫ ЛИНЕЙНЫХ УРАВНЕНИЙ")
        print("=" * 80)
        print("Ищем значения k такие, что g^k разлагается в факторной базе")
        equations, right_sides, factorizations = self.build_linear_system(t)
        
        # Шаг 3: Решение системы
        print("\n" + "=" * 80)
        print("ШАГ 3: РЕШЕНИЕ СИСТЕМЫ УРАВНЕНИЙ")
        print("=" * 80)
        print("Находим логарифмы элементов факторной базы:")
        logs = self.solve_linear_system(equations, right_sides)
        
        # Шаг 4: Нахождение искомого логарифма
        print("\n" + "=" * 80)
        print("ШАГ 4: НАХОЖДЕНИЕ log_g(a)")
        print("=" * 80)
        print(f"Ищем k такое, что a·g^k разлагается в факторной базе")
        result = self.find_logarithm(max_attempts=100)
        
        if result is not None:
            print("\n" + "=" * 80)
            print("РЕЗУЛЬТАТ:")
            print("=" * 80)
            print(f"✓ log_{self.g}({self.a}) = {result} mod {self.n}")
            print(f"  Проверка: {self.g}^{result} mod {self.n} = {self.modular_pow(self.g, result, self.n)}")
            print(f"  Ожидалось: {self.a}")
        else:
            print("\n" + "=" * 80)
            print("РЕЗУЛЬТАТ:")
            print("=" * 80)
            print(f"✗ Не удалось найти логарифм")
        
        return result


def main():
    """Основная функция с вводом параметров"""
    print("АЛГОРИТМ АДЛЕМАНА ДЛЯ ДИСКРЕТНОГО ЛОГАРИФМИРОВАНИЯ")
    print("=" * 80)
    
    # Ввод параметров
    try:
        n = int(input("Введите простое число p (модуль): "))
        if not sympy.isprime(n):
            print(f"Ошибка: {n} не является простым числом!")
            return
        
        g = int(input(f"Введите генератор g группы Z_{n}*: "))
        a = int(input(f"Введите элемент a из Z_{n}*: "))
        
        # Для группы Z_p* порядок равен p-1
        group_order = n - 1
        
        t_input = input(f"Введите размер факторной базы t (рекомендуется {max(5, int(math.log(n, 2)//4))}): ")
        t = int(t_input) if t_input.strip() else None
        
        # Запуск алгоритма
        algorithm = AdlemanAlgorithm(g, a, n, group_order)
        result = algorithm.run(t)
        
    except ValueError as e:
        print(f"Ошибка ввода: {e}")
    except Exception as e:
        print(f"Произошла ошибка: {e}")


def example_small():
    """Пример с маленькими числами для демонстрации"""
    print("\n" + "=" * 80)
    print("ПРИМЕР: МАЛЕНЬКИЕ ЧИСЛА ДЛЯ ДЕМОНСТРАЦИИ")
    print("=" * 80)
    
    # Простое число 101
    n = 101
    g = 2  # 2 - генератор Z_101*
    a = 13  # Ищем log_2(13) mod 101
    
    print(f"Параметры:")
    print(f"  p = {n} (простое)")
    print(f"  g = {g} (генератор Z_{n}*)")
    print(f"  a = {a}")
    print(f"  Порядок группы: {n-1}")
    print()
    
    algorithm = AdlemanAlgorithm(g, a, n, n-1)
    result = algorithm.run(t=8)
    
    if result is not None:
        print(f"\n✓ Найденный результат: log_{g}({a}) = {result} mod {n}")
        print(f"  Проверка: {g}^{result} mod {n} = {pow(g, result, n)}")
        print(f"  Ожидалось: {a}")
        
        # Дополнительная проверка
        if pow(g, result, n) == a % n:
            print("  ✓ Результат верный!")
        else:
            print("  ✗ Результат неверный!")


def example_medium():
    """Пример с числами среднего размера"""
    print("\n" + "=" * 80)
    print("ПРИМЕР: ЧИСЛА СРЕДНЕГО РАЗМЕРА")
    print("=" * 80)
    
    # Простое число 1009
    n = 1009
    g = 11  # 11 - генератор Z_1009*
    a = 23  # Ищем log_11(23) mod 1009
    
    print(f"Параметры:")
    print(f"  p = {n} (простое)")
    print(f"  g = {g} (генератор Z_{n}*)")
    print(f"  a = {a}")
    print(f"  Порядок группы: {n-1}")
    print()
    
    algorithm = AdlemanAlgorithm(g, a, n, n-1)
    result = algorithm.run(t=12)
    
    if result is not None:
        print(f"\n✓ Найденный результат: log_{g}({a}) = {result} mod {n}")
        print(f"  Проверка: {g}^{result} mod {n} = {pow(g, result, n)}")
        print(f"  Ожидалось: {a}")


if __name__ == "__main__":
    print("ВЫБЕРИТЕ РЕЖИМ:")
    print("1. Интерактивный ввод")
    print("2. Пример с маленькими числами (p=101)")
    print("3. Пример со средними числами (p=1009)")
    
    choice = input("\nВаш выбор (1-3): ").strip()
    
    if choice == "1":
        main()
    elif choice == "2":
        example_small()
    elif choice == "3":
        example_medium()
    else:
        print("Неверный выбор, запускаю интерактивный режим...")
        main()