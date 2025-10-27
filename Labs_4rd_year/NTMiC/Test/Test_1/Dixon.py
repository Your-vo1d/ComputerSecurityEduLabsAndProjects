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

def build_small_factor_base() -> List[int]:
    """
    Строит маленькую факторную базу из 3 первых простых чисел
    """
    return [2, 3, 5]

def factor_smooth_number_strict(b: int, factor_base: List[int]) -> Optional[Tuple[List[int], List[int]]]:
    """
    СТРОГАЯ факторизация числа b по факторной базе
    Возвращает вектор показателей только если b делится ТОЛЬКО на числа из факторной базы
    Числа 1 и -1 НЕ подходят!
    """
    if b == 0:
        return None, [0]
    
    # Числа 1 и -1 НЕ подходят - они не раскладываются на множители из базы
    if abs(b) == 1:
        return None, ["±1"]  # 1 и -1 недопустимы
    
    factors = [0] * len(factor_base)
    temp = abs(b)
    
    # Сначала факторизуем по факторной базе
    for i, p in enumerate(factor_base):
        count = 0
        while temp % p == 0:
            count += 1
            temp //= p
        factors[i] = count
    
    # СТРОГАЯ ПРОВЕРКА: если остался множитель не равный 1, то число НЕ подходит
    if temp != 1:
        # Найдем, какие еще простые множители есть
        extra_factors = []
        temp_check = temp
        d = 2
        while temp_check > 1:
            if temp_check % d == 0:
                # Проверяем, является ли этот делитель простым и не входит ли в факторную базу
                if is_prime(d) and d not in factor_base:
                    extra_factors.append(d)
                temp_check //= d
            else:
                d += 1
        
        return None, extra_factors  # Возвращаем None и список лишних множителей
    
    return factors, []  # Возвращаем факторизацию и пустой список лишних множителей

def print_factorization(b: int, factors: List[int], factor_base: List[int]) -> str:
    """Выводит разложение числа на множители"""
    terms = []
    for i, (exp, p) in enumerate(zip(factors, factor_base)):
        if exp > 0:
            terms.append(f"{p}^{exp}")
    return " × ".join(terms) if terms else "1"

def generate_smooth_pairs_dixon_strict(n: int, factor_base: List[int], t: int) -> Tuple[List[List[int]], List[int], List[int]]:
    """
    Генерирует t пар чисел (a_i, b_i) с СТРОГОЙ проверкой гладкости
    """
    matrix = []
    a_values = []
    b_values = []
    
    # Начинаем с a = ⌊√n⌋ + 1
    start_a = math.isqrt(n) + 1
    current_a = start_a
    
    print(f"\n--- ШАГ 2: Построение {t} пар (a_i, b_i) ---")
    print(f"Начальное значение a = ⌊√{n}⌋ + 1 = {start_a}")
    print(f"Факторная база: {factor_base}")
    print("СТРОГИЕ Требования:")
    print("а) a_i = ⌊√n⌋ + 1, ⌊√n⌋ + 2, ... (последовательные значения)")
    print("б) b_i = a_i² mod n") 
    print("в) b_i является СТРОГО p_k-гладким (делится ТОЛЬКО на 2, 3, 5)")
    print("   Числа 1 и -1 НЕ подходят!")
    print("-" * 50)
    
    attempts = 0
    max_attempts = t * 100
    rejected_count = 0
    
    while len(matrix) < t and attempts < max_attempts:
        attempts += 1
        
        # 2а: a_i - последовательные значения начиная с √n + 1
        a = current_a
        current_a += 1
        
        # 2б: b_i = a_i^2 mod n
        b = (a * a) % n
        
        # 2в: СТРОГАЯ проверка что b_i является гладким (делится ТОЛЬКО на 2, 3, 5)
        result, extra_factors = factor_smooth_number_strict(b, factor_base)
        
        if result is not None:
            factors = result
            
            print(f"\n✅ Пара #{len(matrix) + 1} ПРИНЯТА:")
            print(f"  a_{len(matrix)} = {a} (⌊√{n}⌋ + {a - start_a})")
            print(f"  b_{len(matrix)} = {a}² mod {n} = {a*a} mod {n} = {b}")
            
            print(f"  Разложение: {b} = {print_factorization(b, factors, factor_base)}")
            print(f"  СТРОГО p_k-гладкое: ДА ✓ (делится ТОЛЬКО на 2, 3, 5)")
            print(f"  |b_i| = {abs(b)}")
            
            # Приводим показатели по модулю 2
            factors_mod2 = [f % 2 for f in factors]
            matrix.append(factors_mod2)
            a_values.append(a)
            b_values.append(b)
        else:
            rejected_count += 1
            # Показываем ВСЕ отклоненные пары для отладки
            print(f"\n❌ Пара ОТКЛОНЕНА (попытка #{attempts}):")
            print(f"  a = {a} (⌊√{n}⌋ + {a - start_a})")
            print(f"  b = {a}² mod {n} = {a*a} mod {n} = {b}")
            if b == 0:
                print(f"  b = 0 (не подходит)")
            elif abs(b) == 1:
                print(f"  b = {b} (числа 1 и -1 не подходят!)")
            else:
                print(f"  b имеет дополнительные простые множители: {extra_factors}")
                print(f"  Полное разложение b: ", end="")
                temp = abs(b)
                factors_full = []
                d = 2
                while temp > 1:
                    if temp % d == 0:
                        count = 0
                        while temp % d == 0:
                            count += 1
                            temp //= d
                        factors_full.append(f"{d}^{count}")
                    else:
                        d += 1
                print(" × ".join(factors_full))
    
    print(f"\n📊 ИТОГИ ПОИСКА:")
    print(f"✅ Принято пар: {len(matrix)}")
    print(f"❌ Отклонено пар: {rejected_count}")
    print(f"🔍 Всего проверено: {attempts}")
    print(f"📈 Эффективность: {len(matrix)}/{attempts} = {len(matrix)/attempts*100:.1f}%")
    print(f"🎯 Диапазон a_i: от {start_a} до {current_a - 1}")
    return matrix, a_values, b_values

def print_matrix_system_small(matrix: List[List[int]], factor_base: List[int]):
    """Выводит матрицу и систему уравнений для маленькой базы"""
    print(f"\n--- ШАГ 3: Матрица V размера {len(matrix)}×{len(factor_base)} ---")
    print("Матрица V (показатели по модулю 2):")
    print("   " + " ".join(f" {p} " for p in factor_base))
    for i, row in enumerate(matrix):
        print(f"v{i}: " + " ".join(f" {val} " for val in row))
    
    print(f"\nФакторная база: {factor_base}")
    print(f"Всего факторов: {len(factor_base)}")
    
    print(f"\nВектор булевых переменных c = (c₁, c₂, ..., c_{len(matrix)})")
    print("где c_i ∈ {0, 1}")
    
    print(f"\nСистема уравнений cV = O (по модулю 2):")
    equations_count = 0
    for j in range(len(factor_base)):
        equation = []
        for i in range(len(matrix)):
            if matrix[i][j] == 1:
                equation.append(f"c{i}")
        if equation:
            print(f"  Уравнение {j+1} (фактор {factor_base[j]}): { ' ⊕ '.join(equation) } = 0")
            equations_count += 1
    
    print(f"\n📐 Размерность системы:")
    print(f"  Уравнений: {equations_count}")
    print(f"  Переменных: {len(matrix)}")
    print(f"  Степень свободы: {len(matrix) - equations_count}")
    return equations_count

def solve_mod2_system_small(matrix: List[List[int]]) -> List[List[int]]:
    """
    Решает систему линейных уравнений по модулю 2 для маленькой матрицы
    """
    if not matrix:
        return []
    
    m = len(matrix)  # количество строк
    n = len(matrix[0])  # количество столбцов
    
    print(f"\n--- Решение системы {m}×{n} по модулю 2 ---")
    print(f"Маленькая система: {m} переменных, {n} уравнений")
    
    # Транспонируем матрицу для удобства
    A = [[matrix[j][i] for j in range(m)] for i in range(n)]
    
    print("Матрица системы (транспонированная):")
    for i in range(n):
        print(f"  Ур-ние {i+1} (фактор {[2, 3, 5][i]}): {A[i]}")
    
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
            print(f"  Столбец {col}: нет опорного элемента")
            col += 1
            continue
        
        print(f"  Столбец {col}: опорный элемент в строке {pivot_row}")
        
        # Меняем строки местами
        if pivot_row != row:
            A[row], A[pivot_row] = A[pivot_row], A[row]
            print(f"    Поменяли строки {row} и {pivot_row}")
        
        # Обнуляем элементы ниже
        for r in range(row + 1, n):
            if A[r][col] == 1:
                for j in range(col, m):
                    A[r][j] ^= A[row][j]
                print(f"    Обнулили строку {r} используя строку {row}")
        
        pivot_rows.append(row)
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
    print(f"Свободных переменных: {len(free_vars)}")
    
    # Для каждой свободной переменной
    for free_var in free_vars:
        solution = [0] * m
        solution[free_var] = 1
        
        print(f"\n🔍 Решение для свободной переменной c{free_var} = 1:")
        
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
            print(f"  ✅ Полное решение: {solution}")
            solutions.append(solution)
        else:
            print(f"  ❌ Решение невалидно: {solution}")
    
    print(f"\n🎯 Всего найдено решений: {len(solutions)}")
    return solutions

def dixon_algorithm_strict(n: int) -> int:
    """
    Улучшенный алгоритм Диксона со СТРОГОЙ факторной базой [2, 3, 5]
    """
    print("=== УЛУЧШЕННЫЙ АЛГОРИТМ ДИКСОНА ===")
    print("=== СО СТРОГОЙ ФАКТОРНОЙ БАЗОЙ [2, 3, 5] ===")
    print(f"Факторизация числа: {n}")
    
    # Проверяем что n нечётное и составное
    if n % 2 == 0:
        return 2
    
    # Проверяем что n не является степенью
    for b in range(2, int(math.log2(n)) + 2):
        a = round(n ** (1/b))
        if a ** b == n:
            return a
    
    # Используем маленькую факторную базу
    k = 3  # всего 3 простых числа
    t = k + 1  # нужно 4 пары
    
    while True:
        print(f"\n{'='*60}")
        print(f"=== ПОПЫТКА С ФАКТОРНОЙ БАЗОЙ [2, 3, 5], t={t} ===")
        print(f"{'='*60}")
        
        # 1. Выбираем маленькую факторную базу
        print(f"\n--- ШАГ 1: Выбор факторной базы ---")
        S = build_small_factor_base()
        print(f"Факторная база S: {S}")
        print(f"p_k = {S[-1]}")
        print(f"⌊√{n}⌋ = {math.isqrt(n)}")
        print("СТРОГОЕ условие: b_i должно делиться ТОЛЬКО на 2, 3, 5")
        print("Числа 1 и -1 НЕ подходят!")
        
        # 2. Строим t пар чисел (a_i, b_i)
        V, a_list, b_list = generate_smooth_pairs_dixon_strict(n, S, t)
        
        if len(V) < t:
            print(f"❌ Не удалось найти {t} гладких чисел. Найдено: {len(V)}")
            t += 1
            if t > 8:  # Ограничим максимальное t
                print("Слишком много попыток, увеличиваем t медленнее")
                t = min(t, 10)
            continue
        
        # 3. Составляем и решаем систему уравнений
        equations_count = print_matrix_system_small(V, S)
        
        solutions = solve_mod2_system_small(V)
        
        if not solutions:
            print("❌ Нет нетривиальных решений системы")
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
                print("❌ ∏ b_i не является полным квадратом - пропускаем")
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
                print(f"\n✅ УСПЕХ: x ≠ ±y (mod n)")
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
                    print("❌ Оба НОД тривиальные - продолжаем поиск")
            else:
                print("❌ x ≡ ±y (mod n) - тривиальное решение")
        
        # Если все решения перебрали и не получили результата
        print(f"\n❌ Все {len(solutions)} решений проверены, делитель не найден")
        print(f"Увеличиваем t с {t} до {t + 1}")
        t += 1
        
        if t > 12:  # Ограничим максимальное t
            raise ValueError("Не удалось найти делитель за разумное время со строгой факторной базой")

def main():
    """Основная функция для ввода с клавиатуры"""
    print("=== УЛУЧШЕННЫЙ АЛГОРИТМ ДИКСОНА ===")
    print("=== СО СТРОГОЙ ФАКТОРНОЙ БАЗОЙ [2, 3, 5] ===")
    print("ОСОБЫЕ УСЛОВИЯ:")
    print("- a_i выбираются последовательно начиная с ⌊√n⌋ + 1")
    print("- Факторная база всего из 3 простых чисел: [2, 3, 5]")
    print("- b_i должны делиться ТОЛЬКО на 2, 3, 5")
    print("- Числа 1 и -1 НЕ подходят!")
    print("- Если b_i имеет другие простые множители - пара ОТБРАСЫВАЕТСЯ")
    
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
        
        print(f"\nНачинаем факторизацию числа {n}")
        print("Это может занять некоторое время...")
        
        # Запускаем улучшенный алгоритм Диксона со строгой базой
        divisor = dixon_algorithm_strict(n)
        
        print(f"\n{'='*60}")
        print(f"=== РЕЗУЛЬТАТ ===")
        print(f"{'='*60}")
        print(f"Найден нетривиальный делитель: {divisor}")
        print(f"{n} = {divisor} × {n // divisor}")
        print(f"Алгоритм успешно сработал со СТРОГОЙ факторной базой из 3 простых чисел!")
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