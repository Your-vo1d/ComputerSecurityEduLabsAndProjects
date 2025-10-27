import math
import random
from typing import List, Tuple, Optional

def is_prime(n: int) -> bool:
    """Проверяет, является ли число простым"""
    if n < 2:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    
    for i in range(3, int(math.sqrt(n)) + 1, 2):
        if n % i == 0:
            return False
    return True

def legendre_symbol(a: int, p: int) -> int:
    """
    Вычисляет символ Лежандра (a/p)
    """
    if p == 2:
        return 1 if a % 2 == 1 else 0
    
    result = pow(a, (p - 1) // 2, p)
    if result == p - 1:
        return -1
    return result

def build_factor_base_with_negative(n: int, k: int) -> List[int]:
    """
    Строит факторную базу с -1 и простыми числами с (n/p) = 1
    """
    print(f"\n--- ПОДРОБНОЕ ФОРМИРОВАНИЕ ФАКТОРНОЙ БАЗЫ ---")
    print(f"Факторная база будет содержать: -1, 2 и {k-2} простых чисел с (n/p) = 1")
    
    factor_base = [-1, 2]  # Всегда включаем -1 и 2
    print(f"\nДобавляем обязательные элементы:")
    print(f"  p₁ = -1 (для учета отрицательных чисел)")
    print(f"  p₂ = 2")
    print(f"  Текущая факторная база: {factor_base}")
    
    p = 3
    tested_primes = 0
    included_primes = 2  # Уже включили -1 и 2
    
    while included_primes < k and tested_primes < 100:
        if is_prime(p):
            tested_primes += 1
            legendre = legendre_symbol(n, p)
            
            print(f"\nПроверка p = {p}:")
            print(f"  Символ Лежандра ({n}/{p}) = {legendre}")
            
            if legendre == 1:
                factor_base.append(p)
                included_primes += 1
                print(f"  ✓ {n} - квадратичный вычет по модулю {p}")
                print(f"  ✓ Добавляем p{included_primes} = {p} в факторную базу")
                print(f"  Текущая факторная база: {factor_base}")
            elif legendre == -1:
                print(f"  ✗ {n} - квадратичный невычет по модулю {p}")
                print(f"  ✗ Пропускаем {p}")
            else:
                print(f"  ! {n} ≡ 0 (mod {p}) - {p} является делителем {n}!")
                return [p]
            
        p += 2
    
    print(f"\n--- ИТОГ ФОРМИРОВАНИЯ ФАКТОРНОЙ БАЗЫ ---")
    print(f"Факторная база: {factor_base}")
    print(f"p₁ = -1, p₂ = 2, " + ", ".join([f"p{i+1} = {factor_base[i]}" for i in range(2, len(factor_base))]))
    return factor_base

def factor_smooth_number_with_negative(b: int, factor_base: List[int]) -> Optional[Tuple[List[int], List[int]]]:
    """
    Факторизация числа b по факторной базе с учетом -1
    """
    if b == 0:
        return None, [0]
    
    factors = [0] * len(factor_base)
    temp = abs(b)
    
    # Обрабатываем знак (первый элемент факторной базы -1)
    if b < 0:
        factors[0] = 1  # Используем -1
    
    # Факторизуем по остальным простым числам
    for i in range(1, len(factor_base)):
        p = factor_base[i]
        count = 0
        while temp % p == 0:
            count += 1
            temp //= p
        factors[i] = count
    
    # Проверяем, что число полностью факторизовано по базе
    if temp != 1:
        extra_factors = []
        temp_check = temp
        d = 2
        while temp_check > 1:
            if temp_check % d == 0:
                if is_prime(d) and d not in factor_base:
                    extra_factors.append(d)
                temp_check //= d
            else:
                d += 1
        return None, extra_factors
    
    return factors, []

def print_factorization_with_negative(b: int, factors: List[int], factor_base: List[int]) -> str:
    """Выводит разложение числа на множители с учетом -1"""
    terms = []
    
    # Обрабатываем знак отдельно
    if factors[0] == 1:  # Использован -1
        terms.append("-1")
    
    # Остальные множители
    for i in range(1, len(factor_base)):
        exp = factors[i]
        p = factor_base[i]
        if exp > 0:
            terms.append(f"{p}^{exp}")
    
    if not terms:
        return "1"
    
    return " × ".join(terms)

def print_vector_formation(x: int, a: int, b: int, factors: List[int], factor_base: List[int], vector_index: int):
    """Подробный вывод формирования вектора для конкретного x"""
    print(f"\n📊 ПОДРОБНОЕ ФОРМИРОВАНИЕ ВЕКТОРА c{vector_index} для x = {x}:")
    print(f"  a = x + m = {x} + {a - x} = {a}")
    print(f"  b = a² - n = {a}² - {a*a - b} = {b}")
    print(f"  Факторизация: {b} = {print_factorization_with_negative(b, factors, factor_base)}")
    
    print(f"  Вектор показателей: {factors}")
    
    # Преобразуем в вектор по модулю 2
    factors_mod2 = [f % 2 for f in factors]
    print(f"  Вектор по модулю 2: {factors_mod2}")
    
    print(f"  Соответствие факторной базе:")
    for i, fb in enumerate(factor_base):
        print(f"    p{i+1} = {fb:2} → показатель: {factors[i]} → mod 2: {factors_mod2[i]}")
    
    return factors_mod2

def generate_smooth_pairs_quadratic_sieve_with_negative(n: int, factor_base: List[int], t: int) -> Tuple[List[List[int]], List[int], List[int]]:
    """
    Генерирует t пар чисел (a_i, b_i) с правильной факторной базой
    """
    matrix = []
    a_values = []
    b_values = []
    x_values_used = []
    
    m = math.isqrt(n)
    print(f"\n--- ШАГ 2: ПОСТРОЕНИЕ {t} ПАР (a_i, b_i) ---")
    print(f"m = [√{n}] = {m}")
    print("Ищем x такие, что b = (x + m)² - n является гладким")
    print("Проверяем x = 0, ±1, ±2, ±3, ...")
    print("-" * 60)
    
    x = 0
    max_x = 100
    found_pairs = 0
    checked_x = set()  # Для отслеживания уже проверенных x
    
    print(f"\nНачинаем поиск с x = 0:")
    
    while found_pairs < t and abs(x) <= max_x:
        # Проверяем только уникальные x
        if x not in checked_x:
            checked_x.add(x)
            
            # a_i = x + m
            a = x + m
            
            # b_i = (x + m)² - n = a² - n
            b = a * a - n
            
            print(f"\n🔍 Проверяем x = {x}:")
            print(f"  a = x + m = {x} + {m} = {a}")
            print(f"  b = a² - n = {a}² - {n} = {a*a} - {n} = {b}")
            
            # Проверяем что b_i является гладким
            result, extra_factors = factor_smooth_number_with_negative(b, factor_base)
            
            if result is not None:
                factors = result
                found_pairs += 1
                
                print(f"  ✅ Пара #{found_pairs} ПРИНЯТА!")
                
                # Подробный вывод формирования вектора
                factors_mod2 = print_vector_formation(x, a, b, factors, factor_base, found_pairs)
                
                print(f"  СТРОГО p_k-гладкое: ДА ✓")
                
                matrix.append(factors_mod2)
                a_values.append(a)
                b_values.append(b)
                x_values_used.append(x)
            else:
                if b == 0:
                    print(f"  ❌ b = 0 (не подходит)")
                else:
                    print(f"  ❌ b имеет дополнительные простые множители: {extra_factors}")
        
        # Переходим к следующему x в порядке: 0, 1, -1, 2, -2, ...
        if x >= 0:
            x = -x - 1
        else:
            x = -x
    
    print(f"\n📊 ИТОГИ ПОИСКА:")
    print(f"✅ Найдено пар: {found_pairs}")
    print(f"🎯 Использованные значения x: {x_values_used}")
    
    return matrix, a_values, b_values

def print_matrix_system_with_negative(matrix: List[List[int]], factor_base: List[int]):
    """Выводит матрицу и систему уравнений с правильной факторной базой"""
    print(f"\n--- ШАГ 3: МАТРИЧНОЕ ПРЕДСТАВЛЕНИЕ ---")
    print(f"Матрица V размера {len(matrix)}×{len(factor_base)}")
    print("Столбцы: " + " ".join([f"p{i+1}" for i in range(len(factor_base))]))
    print("Факторы: " + " ".join([f"{fb:2}" for fb in factor_base]))
    
    print(f"\nМатрица V (показатели по модулю 2):")
    for i, row in enumerate(matrix):
        print(f"c{i+1}: " + " ".join([f" {val:2}" for val in row]))
    
    print(f"\nСИСТЕМА УРАВНЕНИЙ cV ≡ 0 (mod 2):")
    equations_count = 0
    
    for j in range(len(factor_base)):
        equation_terms = []
        for i in range(len(matrix)):
            if matrix[i][j] == 1:
                equation_terms.append(f"c{i+1}")
        
        if equation_terms:
            equations_count += 1
            factor_name = f"p{j+1} ({factor_base[j]})"
            print(f"  Для {factor_name}: { ' + '.join(equation_terms) } ≡ 0")
    
    print(f"\n📐 АНАЛИЗ СИСТЕМЫ:")
    print(f"  Уравнений: {equations_count}, Переменных: {len(matrix)}")
    print(f"  Степень свободы: {len(matrix) - equations_count}")
    
    return equations_count

def solve_mod2_system_corrected(matrix: List[List[int]]) -> List[List[int]]:
    """
    Решает систему линейных уравнений по модулю 2 с исправленной логикой
    """
    if not matrix:
        return []
    
    m = len(matrix)  # строки
    n = len(matrix[0])  # столбцы
    
    print(f"\n--- РЕШЕНИЕ СИСТЕМЫ {m}×{n} ПО МОДУЛЮ 2 ---")
    
    # Транспонируем матрицу
    A = [[matrix[j][i] for j in range(m)] for i in range(n)]
    
    print("Матрица системы (транспонированная):")
    for i in range(n):
        print(f"  {A[i]}")
    
    # Прямой ход метода Гаусса
    row = 0
    col = 0
    pivot_rows = []
    
    while row < n and col < m:
        # Ищем опорный элемент
        pivot_row = -1
        for r in range(row, n):
            if A[r][col] == 1:
                pivot_row = r
                break
        
        if pivot_row == -1:
            col += 1
            continue
        
        # Меняем строки местами
        if pivot_row != row:
            A[row], A[pivot_row] = A[pivot_row], A[row]
        
        # Обнуляем элементы ниже
        for r in range(row + 1, n):
            if A[r][col] == 1:
                for j in range(col, m):
                    A[r][j] ^= A[row][j]
        
        pivot_rows.append(row)
        row += 1
        col += 1
    
    print(f"Матрица после преобразований:")
    for i in range(n):
        print(f"  {A[i]}")
    
    # Находим решения
    solutions = []
    
    # Базисные переменные
    basic_vars = []
    for r in pivot_rows:
        for c in range(m):
            if A[r][c] == 1:
                basic_vars.append(c)
                break
    
    free_vars = [j for j in range(m) if j not in basic_vars]
    
    print(f"Базисные переменные: {basic_vars}")
    print(f"Свободные переменные: {free_vars}")
    
    if not free_vars:
        return solutions
    
    print(f"\n--- ГЕНЕРАЦИЯ РЕШЕНИЙ ---")
    
    # Для каждой свободной переменной
    for free_var in free_vars:
        solution = [0] * m
        solution[free_var] = 1
        
        print(f"\nРешение для c{free_var + 1} = 1:")
        
        # Обратный ход
        for i in range(len(pivot_rows) - 1, -1, -1):
            r = pivot_rows[i]
            pivot_col = -1
            for c in range(m):
                if A[r][c] == 1:
                    pivot_col = c
                    break
            
            if pivot_col == -1:
                continue
            
            sum_val = 0
            for c in range(pivot_col + 1, m):
                sum_val ^= A[r][c] & solution[c]
            solution[pivot_col] = sum_val
            print(f"  c{pivot_col + 1} = {sum_val}")
        
        print(f"  Полное решение: {[f'c{i+1}={val}' for i, val in enumerate(solution)]}")
        solutions.append(solution)
    
    print(f"\n🎯 Найдено решений: {len(solutions)}")
    return solutions

def check_solution_and_find_divisor(n: int, solution: List[int], a_list: List[int], b_list: List[int], factor_base: List[int]):
    """
    Проверяет решение и ищет делитель с правильной обработкой отрицательных чисел
    """
    I = [i for i, val in enumerate(solution) if val == 1]
    
    print(f"Множество I = {[i+1 for i in I]} (индексы cᵢ)")
    
    # Вычисляем x = произведение a_i по i из I mod n
    x = 1
    for i in I:
        x = (x * a_list[i]) % n
    
    # Вычисляем произведение b_i по i из I
    product_b = 1
    for i in I:
        product_b *= b_list[i]
    
    print(f"x = ∏ a_i mod n = {x} mod {n}")
    print(f"∏ b_i = {product_b}")
    
    # Для отрицательных произведений берем модуль
    product_b_abs = abs(product_b)
    sign = -1 if product_b < 0 else 1
    
    # Проверяем что произведение - полный квадрат
    y_abs = math.isqrt(product_b_abs)
    is_square = (y_abs * y_abs == product_b_abs)
    
    print(f"|∏ b_i| = {product_b_abs}")
    print(f"√(|∏ b_i|) = {y_abs}")
    print(f"{y_abs}² = {y_abs * y_abs}")
    print(f"Полный квадрат? {is_square}")
    
    if not is_square:
        print("❌ ∏ b_i не является полным квадратом")
        return None
    
    # Восстанавливаем y с учетом знака
    y = y_abs if sign == 1 else y_abs  # y всегда неотрицательный, знак учитывается в сравнении
    
    print(f"y = {y}")
    
    # Проверяем условие x ≠ ±y (mod n)
    x_mod = x % n
    y_mod = y % n
    neg_y_mod = (n - y_mod) % n
    
    print(f"\nПроверка условия x ≠ ±y (mod n):")
    print(f"  x mod n = {x_mod}")
    print(f"  y mod n = {y_mod}")
    print(f"  -y mod n = {neg_y_mod}")
    print(f"  x ≡ y (mod n)? {x_mod == y_mod}")
    print(f"  x ≡ -y (mod n)? {x_mod == neg_y_mod}")
    
    if x_mod != y_mod and x_mod != neg_y_mod:
        print(f"✅ УСПЕХ: x ≠ ±y (mod n)")
        print(f"\n🎯 ВЫЧИСЛЕНИЕ НОД:")
        
        # Вычисляем оба НОД
        d1 = math.gcd(x + y, n)
        d2 = math.gcd(abs(x - y), n)
        
        print(f"  НОД(x + y, n) = НОД({x + y}, {n}) = {d1}")
        print(f"  НОД(x - y, n) = НОД({abs(x - y)}, {n}) = {d2}")
        
        if d1 != 1 and d1 != n:
            return d1
        elif d2 != 1 and d2 != n:
            return d2
        else:
            print("❌ Оба НОД тривиальные")
    else:
        print("❌ x ≡ ±y (mod n) - тривиальное решение")
    
    return None

def quadratic_sieve_with_negative_base(n: int, k: int) -> int:
    """
    Квадратичное решето с правильной факторной базой [-1, 2, ...]
    """
    print("=== КВАДРАТИЧНОЕ РЕШЕТО ===")
    print(f"Факторизация числа: {n}")
    print(f"Размер факторной базы: k = {k}")
    print(f"Факторная база будет содержать: -1, 2 и простые числа с (n/p) = 1")
    
    if n % 2 == 0:
        return 2
    
    # Проверяем что n не является степенью
    for b in range(2, int(math.log2(n)) + 2):
        a = round(n ** (1/b))
        if a ** b == n:
            return a
    
    t = k + 1
    
    while True:
        print(f"\n{'='*70}")
        print(f"=== ПОПЫТКА С ФАКТОРНОЙ БАЗОЙ ИЗ {k} ЭЛЕМЕНТОВ ===")
        print(f"Требуемое количество пар: t = {t}")
        print(f"{'='*70}")
        
        # 1. Выбираем факторную базу
        print(f"\n--- ШАГ 1: ВЫБОР ФАКТОРНОЙ БАЗЫ ---")
        S = build_factor_base_with_negative(n, k)
        
        if len(S) == 1 and S[0] != -1:
            return S[0]
        
        # 2. Строим t пар чисел (a_i, b_i)
        V, a_list, b_list = generate_smooth_pairs_quadratic_sieve_with_negative(n, S, t)
        
        if len(V) < t:
            print(f"❌ Не удалось найти {t} гладких чисел. Найдено: {len(V)}")
            t += 1
            continue
        
        # 3. Составляем и решаем систему уравнений
        equations_count = print_matrix_system_with_negative(V, S)
        
        solutions = solve_mod2_system_corrected(V)
        
        if not solutions:
            print("❌ Нет нетривиальных решений системы")
            t += 1
            continue
        
        # 4-6. Перебираем решения системы и ищем делитель
        print(f"\n{'='*70}")
        print(f"=== ПОИСК НЕТРИВИАЛЬНОГО ДЕЛИТЕЛЯ ===")
        
        for solution_idx, c in enumerate(solutions):
            print(f"\n--- ПРОВЕРКА РЕШЕНИЯ {solution_idx + 1} ---")
            print(f"Вектор c = {[f'c{i+1}={val}' for i, val in enumerate(c)]}")
            
            divisor = check_solution_and_find_divisor(n, c, a_list, b_list, S)
            if divisor is not None:
                return divisor
        
        print(f"❌ Все {len(solutions)} решений проверены, делитель не найден")
        t += 1
        
        if t > k + 8:
            raise ValueError(f"Не удалось найти делитель для n={n} с k={k}")

def main():
    """Основная функция"""
    print("=== КВАДРАТИЧНОЕ РЕШЕТО ===")
    print("С ПРАВИЛЬНОЙ ФАКТОРНОЙ БАЗОЙ [-1, 2, p₃, ...]")
    print("\nФакторная база включает:")
    print("  - -1 для учета отрицательных чисел b_i")
    print("  - 2 и простые числа p, для которых n - квадратичный вычет mod p")
    print("  - Минимальный размер факторной базы: 3")
    
    try:
        n = int(input("\nВведите нечётное составное число n: "))
        
        if n % 2 == 0:
            print("Число должно быть нечётным!")
            return
        
        if n < 2:
            print("Число должно быть больше 1!")
            return
        
        if is_prime(n):
            print("Число простое!")
            return
        
        k = int(input("Введите размер факторной базы k (минимум 3): "))
        
        if k < 3:
            print("k должен быть не менее 3!")
            return
        
        print(f"\nНачинаем факторизацию числа {n}")
        print(f"С размером факторной базы k = {k}")
        print(f"Факторная база будет содержать: -1, 2 и {k-2} простых чисел с (n/p) = 1")
        
        divisor = quadratic_sieve_with_negative_base(n, k)
        
        print(f"\n{'='*70}")
        print(f"=== РЕЗУЛЬТАТ ===")
        print(f"Найден делитель: {divisor}")
        print(f"{n} = {divisor} × {n // divisor}")
        print(f"Алгоритм успешно завершен!")
        print(f"{'='*70}")
        
    except Exception as e:
        print(f"Ошибка: {e}")

if __name__ == "__main__":
    main()