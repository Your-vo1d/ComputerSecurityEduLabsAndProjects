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

def jacobi_symbol(a: int, n: int) -> int:
    """Вычисляет символ Якоби (a/n)"""
    if n <= 0 or n % 2 == 0:
        raise ValueError("n должно быть положительным нечётным числом")
    
    a = a % n
    result = 1
    
    while a != 0:
        # Убираем степени двойки
        while a % 2 == 0:
            a //= 2
            if n % 8 in [3, 5]:
                result = -result
        
        # Взаимность
        a, n = n, a
        if a % 4 == 3 and n % 4 == 3:
            result = -result
        a = a % n
    
    return result if n == 1 else 0

def build_factor_base(n: int, k: int) -> List[int]:
    """
    Строит факторную базу для метода цепных дробей
    p1 = -1, p2 = 2, остальные - простые с (n/p) = 1
    """
    factor_base = [-1, 2]
    
    p = 3
    while len(factor_base) < k:
        if is_prime(p) and jacobi_symbol(n, p) == 1:
            factor_base.append(p)
        p += 2
    
    return factor_base

def continued_fraction_convergents(n: int, count: int) -> List[Tuple[int, int]]:
    """
    Вычисляет подходящие дроби для √n
    Возвращает список пар (P_j, Q_j)
    """
    sqrt_n = math.isqrt(n)
    
    print(f"\nВычисление подходящих дробей для √{n}:")
    print(f"√{n} ≈ {sqrt_n} + (√{n} - {sqrt_n})")
    
    convergents = []
    
    # Начальные значения
    a0 = sqrt_n
    m0 = 0
    d0 = 1
    a = a0
    
    # Числители и знаменатели подходящих дробей
    P_prev2, P_prev1 = 1, a0
    Q_prev2, Q_prev1 = 0, 1
    
    print(f"\na₀ = {a0}")
    print(f"P₀ = {P_prev1}, Q₀ = {Q_prev1}")
    print(f"Первая подходящая дробь: {P_prev1}/{Q_prev1}")
    
    m = m0
    d = d0
    
    for i in range(1, count + 1):
        # Вычисляем следующие коэффициенты
        m = d * a - m
        d = (n - m * m) // d
        a = (sqrt_n + m) // d
        
        # Вычисляем следующую подходящую дробь
        P_current = a * P_prev1 + P_prev2
        Q_current = a * Q_prev1 + Q_prev2
        
        convergents.append((P_current, Q_current))
        
        print(f"\nШаг {i}:")
        print(f"  m_{i} = {d} * {a} - {m} = {m}")
        print(f"  d_{i} = ({n} - {m}²) / {d} = {d}")
        print(f"  a_{i} = ({sqrt_n} + {m}) / {d} = {a}")
        print(f"  P_{i} = {a} * {P_prev1} + {P_prev2} = {P_current}")
        print(f"  Q_{i} = {a} * {Q_prev1} + {Q_prev2} = {Q_current}")
        print(f"  Подходящая дробь: {P_current}/{Q_current}")
        
        # Обновляем для следующей итерации
        P_prev2, P_prev1 = P_prev1, P_current
        Q_prev2, Q_prev1 = Q_prev1, Q_current
    
    return convergents

def factor_smooth_number(b: int, factor_base: List[int]) -> Optional[Tuple[List[int], int]]:
    """
    Факторизует число b по факторной базе
    Возвращает (вектор показателей, знак) или None если число не гладкое
    """
    if b == 0:
        return None
    
    # Определяем знак
    sign = 1 if b > 0 else -1
    temp = abs(b)
    
    factors = [0] * len(factor_base)
    
    for i, p in enumerate(factor_base):
        if p == -1:
            factors[i] = 0 if sign == 1 else 1
            continue
        
        count = 0
        while temp % p == 0:
            count += 1
            temp //= p
        factors[i] = count
    
    if temp != 1:
        return None
    
    return (factors, sign)

def print_factorization(b: int, factors: List[int], factor_base: List[int]) -> str:
    """Выводит разложение числа на множители"""
    terms = []
    for i, (exp, p) in enumerate(zip(factors, factor_base)):
        if p == -1:
            if exp == 1:
                terms.append("-1")
        elif exp > 0:
            terms.append(f"{p}^{exp}")
    return " × ".join(terms) if terms else "1"

def generate_smooth_pairs_cf(n: int, factor_base: List[int], t: int) -> Tuple[List[List[int]], List[int], List[int]]:
    """
    Генерирует t пар чисел (a_i, b_i) для метода цепных дробей
    """
    matrix = []
    a_values = []
    b_values = []
    
    print(f"\n--- ШАГ 2: Построение {t} пар (a_i, b_i) ---")
    print("Требования:")
    print("а) a_i = P_j mod n, где P_j/Q_j - подходящие дроби для √n")
    print("б) b_i = P_j² - nQ_j²") 
    print("в) b_i является p_k-гладким")
    print("-" * 50)
    
    # Получаем подходящие дроби
    convergents = continued_fraction_convergents(n, t * 3)  # Берем с запасом
    
    for i, (P_j, Q_j) in enumerate(convergents):
        if len(matrix) >= t:
            break
            
        # 2а: a_i = P_j mod n
        a_i = P_j % n
        
        # 2б: b_i = P_j² - nQ_j²
        b_i = P_j * P_j - n * Q_j * Q_j
        
        print(f"\nРассматриваем подходящую дробь #{i+1}:")
        print(f"  P_{i} = {P_j}, Q_{i} = {Q_j}")
        print(f"  a_{len(matrix)} = P_{i} mod {n} = {P_j} mod {n} = {a_i}")
        print(f"  b_{len(matrix)} = P_{i}² - nQ_{i}² = {P_j}² - {n}×{Q_j}² = {P_j*P_j} - {n*Q_j*Q_j} = {b_i}")
        
        # Проверяем вычисления
        check_b = P_j * P_j - n * Q_j * Q_j
        print(f"  Проверка: {P_j}² - {n}×{Q_j}² = {P_j*P_j} - {n*Q_j*Q_j} = {check_b} ✓")
        
        # 2в: проверяем что b_i является гладким
        result = factor_smooth_number(b_i, factor_base)
        
        if result is not None:
            factors, sign = result
            
            print(f"  Разложение: {b_i} = {print_factorization(b_i, factors, factor_base)}")
            print(f"  p_k-гладкое: ДА ✓")
            
            # Приводим показатели по модулю 2
            factors_mod2 = [f % 2 for f in factors]
            matrix.append(factors_mod2)
            a_values.append(a_i)
            b_values.append(b_i)
        else:
            print(f"  p_k-гладкое: НЕТ ✗")
    
    print(f"\nИтого сгенерировано пар: {len(matrix)}")
    return matrix, a_values, b_values

def print_matrix_system(matrix: List[List[int]], factor_base: List[int]):
    """Выводит матрицу и систему уравнений"""
    print(f"\n--- ШАГ 3: Матрица V размера {len(matrix)}×{len(factor_base)} ---")
    print("Матрица V (показатели по модулю 2):")
    
    # Заголовок с обозначениями факторов
    headers = []
    for p in factor_base:
        if p == -1:
            headers.append(" -1")
        else:
            headers.append(f" {p}")
    print("    " + " ".join(headers))
    
    for i, row in enumerate(matrix):
        print(f"v{i}: " + " ".join(f" {val} " for val in row))
    
    print(f"\nФакторная база: {factor_base}")
    
    print(f"\nВектор булевых переменных c = (c₁, c₂, ..., c_{len(matrix)})")
    print("где c_i ∈ {0, 1}")
    
    print(f"\nСистема уравнений cV = O (по модулю 2):")
    equations_found = 0
    for j in range(len(factor_base)):
        equation = []
        for i in range(len(matrix)):
            if matrix[i][j] == 1:
                equation.append(f"c{i}")
        if equation:
            print(f"  { ' ⊕ '.join(equation) } = 0")
            equations_found += 1
    
    print(f"\nВсего уравнений: {equations_found}, переменных: {len(matrix)}")
    return equations_found

def solve_mod2_system(matrix: List[List[int]]) -> List[List[int]]:
    """
    Решает систему линейных уравнений по модулю 2
    """
    if not matrix:
        return []
    
    m = len(matrix)  # количество строк (уравнений)
    n = len(matrix[0])  # количество столбцов (переменных)
    
    print(f"\n--- Решение системы {m}×{n} по модулю 2 ---")
    print(f"Переменные: c₀, c₁, ..., c_{m-1}")
    print(f"Уравнения (столбцы): {n} факторов")
    
    # Транспонируем матрицу для удобства
    A = [[matrix[j][i] for j in range(m)] for i in range(n)]
    
    print("Матрица системы (транспонированная):")
    for i in range(n):
        print(f"  Ур-ние {i}: {A[i]}")
    
    # Прямой ход метода Гаусса
    print("\n--- Прямой ход метода Гаусса ---")
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
        
        print(f"Столбец {col}: опорный элемент в строке {pivot_row}")
        
        # Меняем строки местами
        if pivot_row != row:
            A[row], A[pivot_row] = A[pivot_row], A[row]
            print(f"  Поменяли строки {row} и {pivot_row}")
        
        # Обнуляем элементы ниже
        for r in range(row + 1, n):
            if A[r][col] == 1:
                for j in range(col, m):
                    A[r][j] ^= A[row][j]
                print(f"  Обнулили строку {r} используя строку {row}")
        
        pivot_rows.append(row)
        print(f"  Текущее состояние:")
        for i, r in enumerate(A):
            if i < n:
                print(f"    {r}" + (" *" if i == row else ""))
        
        row += 1
        col += 1
    
    print(f"\nМатрица после прямого хода:")
    for i in range(n):
        print(f"  {A[i]}")
    print(f"Строки с ведущими единицами: {pivot_rows}")
    
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
    
    print(f"Базисные переменные (столбцы): {basic_vars}")
    print(f"Свободные переменные (столбцы): {free_vars}")
    
    if not free_vars:
        print("Нет свободных переменных - только тривиальное решение")
        return solutions
    
    print(f"\n--- Генерация решений ---")
    
    # Для каждой свободной переменной
    for free_var in free_vars:
        solution = [0] * m
        solution[free_var] = 1
        
        print(f"\nРешение для свободной переменной c{free_var} = 1:")
        
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
            print(f"  c{pivot_col} = {sum_val}")
        
        # Проверяем решение
        valid = True
        for r in range(n):
            if r not in pivot_rows:
                continue
            sum_val = 0
            for c in range(m):
                sum_val ^= A[r][c] & solution[c]
            if sum_val != 0:
                valid = False
                break
        
        if valid:
            print(f"  Полное решение: {solution} ✓")
            solutions.append(solution)
        else:
            print(f"  Решение невалидно: {solution} ✗")
    
    print(f"\nВсего найдено решений: {len(solutions)}")
    return solutions

def continued_fractions_method(n: int, k: int = 10) -> int:
    """
    Реализация метода цепных дробей для факторизации
    """
    print("=== МЕТОД ЦЕПНЫХ ДРОБЕЙ ===")
    print(f"Факторизация числа: {n}")
    
    # Проверяем что n нечётное и составное
    if n % 2 == 0:
        return 2
    
    # Проверяем что n не является степенью
    for b in range(2, int(math.log2(n)) + 2):
        a = round(n ** (1/b))
        if a ** b == n:
            return a
    
    t = k + 1
    
    while True:
        print(f"\n{'='*60}")
        print(f"=== ПОПЫТКА С k={k}, t={t} ===")
        print(f"{'='*60}")
        
        # 1. Выбираем факторную базу
        print(f"\n--- ШАГ 1: Выбор факторной базы ---")
        S = build_factor_base(n, k)
        print(f"Факторная база S: {S}")
        print(f"p_k = {S[-1]}")
        print(f"m = [√{n}] = {math.isqrt(n)}")
        print(f"Условие: для всех p ∈ S (кроме -1, 2) символ Якоби ({n}/p) = 1")
        
        # Проверяем условие для простых чисел
        for p in S[2:]:
            jacobi = jacobi_symbol(n, p)
            print(f"  ({n}/{p}) = {jacobi} {'✓' if jacobi == 1 else '✗'}")
        
        # 2. Строим t пар чисел (a_i, b_i)
        V, a_list, b_list = generate_smooth_pairs_cf(n, S, t)
        
        if len(V) < t:
            print(f"Не удалось найти {t} гладких чисел. Найдено: {len(V)}")
            t += 1
            continue
        
        # 3. Составляем и решаем систему уравнений
        equations_count = print_matrix_system(V, S)
        
        if equations_count < len(S):
            print(f"Предупреждение: уравнений ({equations_count}) меньше чем факторов ({len(S)})")
        
        solutions = solve_mod2_system(V)
        
        if not solutions:
            print("Нет нетривиальных решений системы")
            t += 1
            continue
        
        # 4-6. Перебираем решения системы и ищем делитель
        print(f"\n{'='*60}")
        print(f"=== ШАГ 4-6: Поиск нетривиального делителя ===")
        print(f"{'='*60}")
        
        for solution_idx, c in enumerate(solutions):
            print(f"\n--- Проверка решения {solution_idx + 1} ---")
            print(f"Вектор c = {c}")
            
            # I = {i : c_i = 1}
            I = [i for i, val in enumerate(c) if val == 1]
            
            if not I:
                print("Пустое множество I - пропускаем")
                continue
            
            print(f"Множество I = {I}")
            
            # Вычисляем x = произведение a_i по i из I mod n
            print(f"\nВычисление x = ∏ a_i mod n для i ∈ I:")
            x = 1
            for i in I:
                print(f"  a_{i} = {a_list[i]}")
                x = (x * a_list[i]) % n
            print(f"  x = {x} mod {n}")
            
            # Вычисляем произведение b_i по i из I
            print(f"\nВычисление ∏ b_i для i ∈ I:")
            product_b = 1
            for i in I:
                print(f"  b_{i} = {b_list[i]}")
                product_b *= b_list[i]
            print(f"  ∏ b_i = {product_b}")
            
            # Проверяем что произведение - полный квадрат
            print(f"\nПроверка что ∏ b_i - полный квадрат:")
            y = math.isqrt(product_b)
            print(f"  √({product_b}) = {y}")
            print(f"  {y}² = {y*y}")
            print(f"  {y*y} == {product_b} ? {y*y == product_b}")
            
            if y * y != product_b:
                print("  ∏ b_i не является полным квадратом - пропускаем")
                continue
            
            print(f"\nПроверка условия x ≠ ±y (mod n):")
            x_mod = x % n
            y_mod = y % n
            neg_y_mod = (n - y_mod) % n
            
            print(f"  x mod n = {x_mod}")
            print(f"  y mod n = {y_mod}")
            print(f"  -y mod n = {neg_y_mod}")
            print(f"  x ≡ y (mod n)? {x_mod == y_mod}")
            print(f"  x ≡ -y (mod n)? {x_mod == neg_y_mod}")
            
            if x_mod != y_mod and x_mod != neg_y_mod:
                print(f"\n✓ УСПЕХ: x ≠ ±y (mod n)")
                print(f"\nВычисление НОД:")
                
                # Вычисляем оба НОД
                d1 = math.gcd(x + y, n)
                d2 = math.gcd(abs(x - y), n)
                
                print(f"  НОД(x + y, n) = НОД({x + y}, {n}) = {d1}")
                print(f"  НОД(x - y, n) = НОД({abs(x - y)}, {n}) = {d2}")
                
                if d1 != 1 and d1 != n:
                    print(f"\n🎉 НАЙДЕН НЕТРИВИАЛЬНЫЙ ДЕЛИТЕЛЬ: {d1}")
                    return d1
                elif d2 != 1 and d2 != n:
                    print(f"\n🎉 НАЙДЕН НЕТРИВИАЛЬНЫЙ ДЕЛИТЕЛЬ: {d2}")
                    return d2
                else:
                    print("  Оба НОД тривиальные - продолжаем поиск")
            else:
                print("  x ≡ ±y (mod n) - тривиальное решение")
        
        # Если все решения перебрали и не получили результата
        print(f"\nВсе {len(solutions)} решений проверены, делитель не найден")
        print(f"Увеличиваем t с {t} до {t + 1}")
        t += 1
        
        if t > 30:
            k += 2
            t = k + 1
            print(f"Увеличиваем факторную базу до k = {k}")
        
        if k > 20:
            raise ValueError("Не удалось найти делитель за разумное время")

def main():
    """Основная функция для ввода с клавиатуры"""
    print("=== МЕТОД ЦЕПНЫХ ДРОБЕЙ ===")
    print("Поиск нетривиального делителя числа")
    
    try:
        # Ввод числа с клавиатуры
        n = int(input("Введите нечётное составное число n: "))
        
        # Проверки
        if n % 2 == 0:
            print("Число должно быть нечётным!")
            return
        
        if n < 2:
            print("Число должно быть больше 1!")
            return
        
        # Простая проверка на простоту
        if is_prime(n):
            print("Число простое!")
            return
        
        # Ввод параметра k
        k = int(input("Введите размер факторной базы k (рекомендуется 8-15): ") or "10")
        
        print(f"\nНачинаем факторизацию числа {n}")
        print("Это может занять некоторое время...")
        
        # Запускаем метод цепных дробей
        divisor = continued_fractions_method(n, k)
        
        print(f"\n{'='*60}")
        print(f"=== РЕЗУЛЬТАТ ===")
        print(f"{'='*60}")
        print(f"Найден нетривиальный делитель: {divisor}")
        print(f"{n} = {divisor} × {n // divisor}")
        print(f"{'='*60}")
        
    except ValueError as e:
        print(f"Ошибка: {e}")
    except KeyboardInterrupt:
        print("\nПрервано пользователем")
    except Exception as e:
        print(f"Неожиданная ошибка: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    main()