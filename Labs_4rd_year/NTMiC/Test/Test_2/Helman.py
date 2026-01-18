import math
from typing import List, Tuple, Dict, Optional
import sympy

class PohligHellmanAlgorithm:
    def __init__(self, g: int, a: int, n: int, group_order: int):
        """
        Инициализация алгоритма Полига-Хеллмана
        
        Args:
            g: генератор группы
            a: элемент, для которого ищем логарифм
            n: модуль (простое число)
            group_order: порядок группы
        """
        self.g = g % n
        self.a = a % n
        self.n = n
        self.group_order = group_order
        self.factorization = {}  # факторизация порядка группы
        
    def factorize_order(self) -> Dict[int, int]:
        """
        Факторизация порядка группы на простые множители
        
        Returns:
            Словарь {простое: степень}
        """
        print(f"\nФАКТОРИЗАЦИЯ ПОРЯДКА ГРУППЫ:")
        print(f"n = {self.group_order}")
        
        factors = {}
        temp = self.group_order
        
        # Находим простые делители
        i = 2
        while i * i <= temp:
            if temp % i == 0:
                count = 0
                while temp % i == 0:
                    temp //= i
                    count += 1
                factors[i] = count
                print(f"  Найден множитель: {i}^{count}")
            i += 1 if i == 2 else 2
        
        if temp > 1:
            factors[temp] = 1
            print(f"  Найден множитель: {temp}^1")
        
        self.factorization = factors
        return factors
    
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
    
    def build_tables(self) -> Dict[int, List[Tuple[int, int]]]:
        """
        Построение таблиц для каждого простого множителя
        
        Returns:
            Словарь таблиц: {p_i: [(j, g^(n*j/p_i))]}
        """
        print(f"\nШАГ 1: ПОСТРОЕНИЕ ТАБЛИЦ")
        print("=" * 60)
        
        tables = {}
        
        for p, e in self.factorization.items():
            print(f"\nДля простого множителя p = {p}^(e={e}):")
            
            # Вычисляем g_i = g^(n/p)
            exponent = self.group_order // p
            g_i = self.modular_pow(self.g, exponent, self.n)
            print(f"  1.1 Вычисляем g_i = g^(n/p)")
            print(f"     g_i = {self.g}^({self.group_order}/{p}) mod {self.n}")
            print(f"     g_i = {self.g}^{exponent} mod {self.n}")
            print(f"     g_i = {g_i}")
            
            # Строим таблицу для j = 0..p-1
            table = []
            print(f"  1.2 Строим таблицу для j = 0..{p-1}:")
            print(f"     j | r_ij = g_i^j")
            print(f"     --+----------------")
            
            for j in range(p):
                r_ij = self.modular_pow(g_i, j, self.n)
                table.append((j, r_ij))
                print(f"     {j:2d} | {r_ij:4d}")
            
            tables[p] = table
        
        return tables
    
    def solve_for_prime_power(self, p: int, e: int, tables: Dict[int, List[Tuple[int, int]]]) -> int:
        """
        Решение сравнения для простого множителя p^e
        
        Args:
            p: простое число
            e: степень простого числа
            tables: таблицы, построенные на шаге 1
            
        Returns:
            x_i такое, что x ≡ x_i (mod p^e)
        """
        print(f"\nШАГ 2: РЕШЕНИЕ ДЛЯ p = {p}^(e={e})")
        print("=" * 60)
        
        # 2.1 Вычисляем b_0 = a^(n/p)
        exponent0 = self.group_order // p
        b0 = self.modular_pow(self.a, exponent0, self.n)
        print(f"  2.1 Вычисляем b0 = a^(n/p)")
        print(f"     b0 = {self.a}^({self.group_order}/{p}) mod {self.n}")
        print(f"     b0 = {self.a}^{exponent0} mod {self.n}")
        print(f"     b0 = {b0}")
        
        # Ищем x0 в таблице
        table = tables[p]
        x0 = None
        for j, r_ij in table:
            if r_ij == b0:
                x0 = j
                break
        
        if x0 is None:
            raise ValueError(f"Не найден x0 для p={p}")
        
        print(f"     Находим x0 в таблице: r_ij = {b0} соответствует j = {x0}")
        print(f"     x0 = {x0}")
        
        # Если e = 1, то решение уже найдено
        if e == 1:
            x_i = x0
            print(f"     Так как e=1, решение: x ≡ {x_i} (mod {p})")
            return x_i
        
        # 2.2 Ищем x1, x2, ..., x_{e-1}
        print(f"\n  2.2 Ищем x1, x2, ..., x{e-1}:")
        
        x_values = [x0]
        
        for j in range(1, e):
            print(f"\n    Вычисление x{j}:")
            
            # Вычисляем y_{j-1} = x0 + x1*p + ... + x_{j-1}*p^{j-1}
            y_prev = 0
            print(f"      y{j-1} = ", end="")
            terms = []
            for idx in range(j):
                term = x_values[idx] * (p ** idx)
                y_prev += term
                terms.append(f"{x_values[idx]}·{p}^{idx}")
            
            print(" + ".join(terms) + f" = {y_prev}")
            
            # Вычисляем b_j = (a * g^{-y_{j-1}})^{n/p^{j+1}}
            # Сначала вычисляем g^{-y_{j-1}}
            g_inv = pow(self.g, -y_prev, self.n)  # Обратный элемент
            print(f"      Вычисляем g^(-y{j-1}) = {self.g}^(-{y_prev}) mod {self.n}")
            print(f"      g^(-y{j-1}) = {g_inv}")
            
            # Вычисляем a * g^{-y_{j-1}}
            temp = (self.a * g_inv) % self.n
            print(f"      a * g^(-y{j-1}) = {self.a} * {g_inv} mod {self.n}")
            print(f"      a * g^(-y{j-1}) = {temp}")
            
            # Возводим в степень n/p^{j+1}
            exponent_j = self.group_order // (p ** (j + 1))
            b_j = self.modular_pow(temp, exponent_j, self.n)
            print(f"      b{j} = ({temp})^(n/p^{j+1})")
            print(f"      b{j} = {temp}^({self.group_order}/{p**(j+1)}) mod {self.n}")
            print(f"      b{j} = {temp}^{exponent_j} mod {self.n}")
            print(f"      b{j} = {b_j}")
            
            # Ищем x_j в таблице
            x_j = None
            for idx, r_ij in table:
                if r_ij == b_j:
                    x_j = idx
                    break
            
            if x_j is None:
                raise ValueError(f"Не найден x{j} для p={p}")
            
            print(f"      Находим x{j} в таблице: r_ij = {b_j} соответствует j = {x_j}")
            x_values.append(x_j)
        
        # Вычисляем x_i = x0 + x1*p + x2*p^2 + ... + x_{e-1}*p^{e-1}
        print(f"\n    Вычисляем x_i = x0 + x1·p + x2·p² + ... + x{e-1}·p^{e-1}:")
        
        x_i = 0
        terms_str = []
        for idx in range(e):
            term = x_values[idx] * (p ** idx)
            x_i += term
            terms_str.append(f"{x_values[idx]}·{p}^{idx}")
        
        print(f"      x_i = " + " + ".join(terms_str))
        print(f"      x_i = {x_i}")
        print(f"      x_i mod {p**e} = {x_i % (p**e)}")
        
        return x_i % (p ** e)
    
    def chinese_remainder_theorem(self, remainders: List[int], moduli: List[int]) -> int:
        """
        Китайская теорема об остатках
        
        Args:
            remainders: список остатков
            moduli: список модулей
            
        Returns:
            x такой, что x ≡ remainders[i] (mod moduli[i])
        """
        print(f"\nШАГ 3: КИТАЙСКАЯ ТЕОРЕМА ОБ ОСТАТКАХ")
        print("=" * 60)
        print(f"Решаем систему сравнений:")
        
        for i, (r, m) in enumerate(zip(remainders, moduli)):
            print(f"  x ≡ {r} (mod {m})")
        
        # Реализация CRT
        total_mod = 1
        for m in moduli:
            total_mod *= m
        
        print(f"\n  Общий модуль: M = {' * '.join(map(str, moduli))} = {total_mod}")
        
        result = 0
        for i, (r_i, m_i) in enumerate(zip(remainders, moduli)):
            print(f"\n  Для уравнения x ≡ {r_i} (mod {m_i}):")
            
            # Вычисляем M_i = M / m_i
            M_i = total_mod // m_i
            print(f"    M_{i} = M / m_{i} = {total_mod} / {m_i} = {M_i}")
            
            # Находим обратный к M_i по модулю m_i
            inv_M_i = pow(M_i, -1, m_i)
            print(f"    Находим обратный: {M_i}^(-1) mod {m_i} = {inv_M_i}")
            
            # Добавляем вклад
            term = (r_i * M_i * inv_M_i) % total_mod
            print(f"    Вклад: {r_i} * {M_i} * {inv_M_i} mod {total_mod} = {term}")
            
            result = (result + term) % total_mod
        
        print(f"\n  Итоговый результат: x = {result} mod {total_mod}")
        
        # Проверка
        print(f"\n  Проверка:")
        for i, (r_i, m_i) in enumerate(zip(remainders, moduli)):
            check = result % m_i
            status = "✓" if check == r_i else "✗"
            print(f"    x mod {m_i} = {check} {status} (ожидалось {r_i})")
        
        return result
    
    def run(self) -> Optional[int]:
        """
        Запуск полного алгоритма Полига-Хеллмана
        
        Returns:
            Найденный дискретный логарифм
        """
        print("=" * 80)
        print("АЛГОРИТМ ПОЛИГА-ХЕЛЛМАНА ДЛЯ ДИСКРЕТНОГО ЛОГАРИФМИРОВАНИЯ")
        print("=" * 80)
        print(f"Группа: Z_{self.n}*")
        print(f"Генератор: g = {self.g}")
        print(f"Элемент: a = {self.a}")
        print(f"Порядок группы: n = {self.group_order}")
        print("=" * 80)
        
        # Факторизация порядка группы
        factors = self.factorize_order()
        
        if not factors:
            print("\nПорядок группы - простое число, алгоритм сводится к полному перебору")
            return self.brute_force()
        
        # Шаг 1: Построение таблиц
        tables = self.build_tables()
        
        # Шаг 2: Решение для каждого простого множителя
        print(f"\nШАГ 2: РЕШЕНИЕ ДЛЯ КАЖДОГО ПРОСТОГО МНОЖИТЕЛЯ")
        print("=" * 60)
        
        remainders = []
        moduli = []
        
        for p, e in factors.items():
            print(f"\n--- Обработка множителя {p}^{e} ---")
            x_i = self.solve_for_prime_power(p, e, tables)
            remainders.append(x_i)
            moduli.append(p ** e)
            
            print(f"  Результат: x ≡ {x_i} (mod {p**e})")
        
        # Шаг 3: Китайская теорема об остатках
        if len(remainders) == 1:
            # Если только один множитель
            result = remainders[0]
            print(f"\nТолько один простой множитель, результат: x = {result}")
        else:
            result = self.chinese_remainder_theorem(remainders, moduli)
        
        # Проверка результата
        print(f"\n" + "=" * 80)
        print("ПРОВЕРКА РЕЗУЛЬТАТА")
        print("=" * 80)
        
        check = self.modular_pow(self.g, result, self.n)
        print(f"Вычисляем g^x mod n:")
        print(f"  {self.g}^{result} mod {self.n} = {check}")
        print(f"Ожидалось: a = {self.a}")
        
        if check == self.a % self.n:
            print(f"\n✓ УСПЕХ: log_{self.g}({self.a}) = {result} mod {self.n}")
        else:
            print(f"\n✗ ОШИБКА: Проверка не пройдена")
            # Попробуем найти правильное значение
            correct = self.find_correct_result(result, moduli)
            if correct is not None:
                print(f"  Найдено правильное значение: {correct}")
                result = correct
        
        return result
    
    def brute_force(self) -> int:
        """
        Полный перебор (используется, когда порядок группы простой)
        
        Returns:
            Найденный дискретный логарифм
        """
        print(f"\nПОЛНЫЙ ПЕРЕБОР (n = {self.group_order} - простое):")
        print(f"Ищем x такой, что {self.g}^x ≡ {self.a} (mod {self.n})")
        
        for x in range(self.group_order):
            if self.modular_pow(self.g, x, self.n) == self.a:
                print(f"  Найдено: x = {x}")
                print(f"  Проверка: {self.g}^{x} mod {self.n} = {self.modular_pow(self.g, x, self.n)}")
                return x
        
        raise ValueError(f"Логарифм не найден")
    
    def find_correct_result(self, candidate: int, moduli: List[int]) -> Optional[int]:
        """
        Поиск правильного результата среди кратных общего модуля
        
        Args:
            candidate: кандидат на результат
            moduli: список модулей
            
        Returns:
            Правильное значение или None
        """
        total_mod = 1
        for m in moduli:
            total_mod *= m
        
        print(f"\nПоиск правильного значения среди кандидатов...")
        print(f"Общий модуль: {total_mod}")
        
        for k in range(-5, 6):
            test_val = (candidate + k * total_mod) % self.group_order
            if test_val < 0:
                test_val += self.group_order
            
            check = self.modular_pow(self.g, test_val, self.n)
            if check == self.a:
                print(f"  Найдено: x = {test_val} (candidate + {k}*{total_mod})")
                return test_val
        
        return None


def example_from_book():
    """
    Пример из книги: В группе G = Z_61* = <2> найти x = log_2(7)
    """
    print("\n" + "=" * 80)
    print("ПРИМЕР ИЗ КНИГИ")
    print("=" * 80)
    print("Задача: В группе G = Z_61* = <2> найти x = log_2(7)")
    
    n = 61  # простое число
    g = 2   # генератор
    a = 7   # элемент
    group_order = n - 1  # для Z_p* порядок равен p-1
    
    print(f"\nПараметры:")
    print(f"  Модуль: n = {n} (простое)")
    print(f"  Генератор: g = {g}")
    print(f"  Элемент: a = {a}")
    print(f"  Порядок группы: |G| = {group_order}")
    
    # Факторизация порядка группы: 60 = 2^2 * 3 * 5
    print(f"\nФакторизация порядка группы:")
    print(f"  60 = 2^2 * 3 * 5")
    
    algorithm = PohligHellmanAlgorithm(g, a, n, group_order)
    
    # Устанавливаем факторизацию вручную для примера
    algorithm.factorization = {2: 2, 3: 1, 5: 1}
    
    result = algorithm.run()
    
    if result is not None:
        print(f"\n✓ Результат из книги: log_2(7) = {result} mod 61")
        print(f"  Проверка: 2^{result} mod 61 = {pow(2, result, 61)}")


def example_simple():
    """
    Простой пример для демонстрации
    """
    print("\n" + "=" * 80)
    print("ПРОСТОЙ ПРИМЕР ДЛЯ ДЕМОНСТРАЦИИ")
    print("=" * 80)
    
    # Группа Z_31*, генератор 3
    n = 31  # простое
    g = 3   # генератор Z_31*
    a = 6   # ищем log_3(6)
    group_order = n - 1  # 30
    
    print(f"\nПараметры:")
    print(f"  Модуль: n = {n} (простое)")
    print(f"  Генератор: g = {g}")
    print(f"  Элемент: a = {a}")
    print(f"  Порядок группы: |G| = {group_order}")
    
    algorithm = PohligHellmanAlgorithm(g, a, n, group_order)
    result = algorithm.run()
    
    if result is not None:
        print(f"\n✓ Результат: log_{g}({a}) = {result} mod {n}")
        print(f"  Проверка: {g}^{result} mod {n} = {pow(g, result, n)}")


def main():
    """Основная функция с вводом параметров"""
    print("АЛГОРИТМ ПОЛИГА-ХЕЛЛМАНА ДЛЯ ДИСКРЕТНОГО ЛОГАРИФМИРОВАНИЯ")
    print("=" * 80)
    
    try:
        n = int(input("Введите простое число p (модуль): "))
        if not sympy.isprime(n):
            print(f"Ошибка: {n} не является простым числом!")
            return
        
        g = int(input(f"Введите генератор g группы Z_{n}*: "))
        a = int(input(f"Введите элемент a из Z_{n}*: "))
        
        # Для группы Z_p* порядок равен p-1
        group_order = n - 1
        
        algorithm = PohligHellmanAlgorithm(g, a, n, group_order)
        result = algorithm.run()
        
        if result is not None:
            print(f"\n" + "=" * 80)
            print(f"ИТОГОВЫЙ РЕЗУЛЬТАТ: log_{g}({a}) = {result} mod {n}")
            print(f"Проверка: {g}^{result} mod {n} = {pow(g, result, n)}")
            print(f"Ожидалось: {a}")
        
    except ValueError as e:
        print(f"Ошибка ввода: {e}")
    except Exception as e:
        print(f"Произошла ошибка: {e}")
        import traceback
        traceback.print_exc()


if __name__ == "__main__":
    print("ВЫБЕРИТЕ РЕЖИМ:")
    print("1. Интерактивный ввод")
    print("2. Пример из книги (Z_61*, log_2(7))")
    print("3. Простой пример (Z_31*, log_3(6))")
    
    choice = input("\nВаш выбор (1-3): ").strip()
    
    if choice == "1":
        main()
    elif choice == "2":
        example_from_book()
    elif choice == "3":
        example_simple()
    else:
        print("Неверный выбор, запускаю пример из книги...")
        example_from_book()