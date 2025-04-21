import random
import time
from functools import lru_cache


# =============================================
# ТЕСТ ФЕРМА
# =============================================
def test_ferma(number: int, iterations: int) -> bool:
    """
    Проверяет простоту числа number с помощью теста Ферма.
    Возвращает False, если число составное, и True, если возможно простое.
    """
    if number < 2:
        return False
    if number == 2 or number == 3:
        return True
    if number % 2 == 0:
        return False

    for _ in range(iterations):
        a = random.randint(2, number - 2)
        if pow(a, number - 1, number) != 1:
            return False
    return True


@lru_cache(maxsize=None)
def euler_phi(number: int) -> int:
    """
    Вычисляет функцию Эйлера для числа number (количество чисел, взаимно простых с number).
    """
    if number < 1:
        return 0
    if number == 1:
        return 1
    result = number
    # Обработка делителя 2
    if number % 2 == 0:
        while number % 2 == 0:
            number //= 2
        result -= result // 2
    # Обработка нечётных делителей
    i = 3
    while i * i <= number:
        if number % i == 0:
            while number % i == 0:
                number //= i
            result -= result // i
        i += 2
    if number > 1:
        result -= result // number
    return result


def error_probability_ferma(number: int, iterations: int) -> float:
    """
    Возвращает вероятность ошибки теста Ферма.
    """
    return 1 - ((euler_phi(number) / number) ** iterations)


def wrapper_ferma(number: int, iterations: int) -> None:
    """
    Обёртка для теста Ферма с замером времени и выводом результата.
    """
    start_time = time.time()
    is_prime = test_ferma(number, iterations)
    elapsed_time = time.time() - start_time

    if is_prime:
        print("\nТест Ферма: Возможно простое")
        probability = error_probability_ferma(number, iterations)
        print(f"Вероятность ошибки: {probability:.10f}")
    else:
        print("\nТест Ферма: Составное")

    print(f"Время выполнения: {elapsed_time:.6f} секунд")


# =============================================
# ТЕСТ СОЛОВЕЯ-ШТРАССЕНА
# =============================================
def test_solovay_strassen(number: int, iterations: int) -> bool:
    """
    Проверяет простоту числа number с помощью теста Соловея-Штрассена.
    """
    if number < 2:
        return False
    if number == 2:
        return True
    if number % 2 == 0:
        return False

    for _ in range(iterations):
        a = random.randint(2, number - 2)
        x = pow(a, (number - 1) // 2, number)
        if x != 1 and x != number - 1:
            return False
        jacobi = jacobi_symbol(a, number)
        if x != jacobi % number:
            return False
    return True


def jacobi_symbol(a: int, n: int) -> int:
    """
    Вычисляет символ Якоби (a/n).
    """
    if n <= 0 or n % 2 == 0:
        return 0
    result = 1
    if a < 0:
        a = -a
        if n % 4 == 3:
            result = -result
    while a != 0:
        while a % 2 == 0:
            a //= 2
            if n % 8 == 3 or n % 8 == 5:
                result = -result
        a, n = n, a
        if a % 4 == 3 and n % 4 == 3:
            result = -result
        a %= n
    if n == 1:
        return result
    else:
        return 0


def error_probability_solovay_strassen(number: int, iterations: int) -> float:
    """
    Возвращает вероятность ошибки теста Соловея-Штрассена.
    """
    return (euler_phi(number) / (2 * number)) ** iterations


def wrapper_solovay_strassen(number: int, iterations: int) -> None:
    """
    Обёртка для теста Соловея-Штрассена.
    """
    start_time = time.time()
    is_prime = test_solovay_strassen(number, iterations)
    elapsed_time = time.time() - start_time

    if is_prime:
        print("\nТест Соловея-Штрассена: Возможно простое")
        probability = error_probability_solovay_strassen(number, iterations)
        print(f"Вероятность ошибки: {probability:.10f}")
    else:
        print("\nТест Соловея-Штрассена: Составное")

    print(f"Время выполнения: {elapsed_time:.6f} секунд")


# =============================================
# ТЕСТ МИЛЛЕРА-РАБИНА
# =============================================
def miller_rabin(number: int, iterations: int) -> bool:
    """
    Проверяет простоту числа number с помощью теста Миллера-Рабина.
    """
    if number < 2:
        return False
    if number == 2 or number == 3:
        return True
    if number % 2 == 0:
        return False

    # Представляем (number - 1) в виде (2^s * d)
    d = number - 1
    s = 0
    while d % 2 == 0:
        d //= 2
        s += 1

    for _ in range(iterations):
        a = random.randint(2, number - 2)
        x = pow(a, d, number)
        if x == 1 or x == number - 1:
            continue
        for __ in range(s - 1):
            x = pow(x, 2, number)
            if x == number - 1:
                break
        else:
            return False
    return True


def error_probability_miller_rabin(number: int, iterations: int) -> float:
    """
    Возвращает вероятность ошибки теста Миллера-Рабина.
    """
    return (euler_phi(number) / (4 * number)) ** iterations


def wrapper_miller_rabin(number: int, iterations: int) -> None:
    """
    Обёртка для теста Миллера-Рабина.
    """
    start_time = time.time()
    is_prime = miller_rabin(number, iterations)
    elapsed_time = time.time() - start_time

    if is_prime:
        print("\nТест Миллера-Рабина: Возможно простое")
        probability = error_probability_miller_rabin(number, iterations)
        print(f"Вероятность ошибки: {probability:.10f}")
    else:
        print("\nТест Миллера-Рабина: Составное")

    print(f"Время выполнения: {elapsed_time:.6f} секунд")


# =============================================
# СРАВНЕНИЕ ТЕСТОВ
# =============================================
def compare_tests(number: int, iterations: int) -> None:
    """
    Сравнивает все три теста по времени и результатам.
    """
    print("\n=== Сравнение тестов ===")

    # Тест Ферма
    start = time.time()
    ferma_result = test_ferma(number, iterations)
    ferma_time = time.time() - start

    # Тест Соловея-Штрассена
    start = time.time()
    ss_result = test_solovay_strassen(number, iterations)
    ss_time = time.time() - start

    # Тест Миллера-Рабина
    start = time.time()
    mr_result = miller_rabin(number, iterations)
    mr_time = time.time() - start

    print(f"Результаты:")
    print(f"- Ферма: {'Простое' if ferma_result else 'Составное'}")
    print(f"- Соловей-Штрассен: {'Простое' if ss_result else 'Составное'}")
    print(f"- Миллер-Рабин: {'Простое' if mr_result else 'Составное'}")

    print("\nВремя выполнения:")
    print(f"- Ферма: {ferma_time:.6f} сек")
    print(f"- Соловей-Штрассен: {ss_time:.6f} сек")
    print(f"- Миллер-Рабин: {mr_time:.6f} сек")
    
    print("\nВероятность ошибки (только для 'Простое'):")
    if ferma_result:
        print(f"- Ферма: {error_probability_ferma(number, iterations):.10f}")
    else:
        print("- Ферма: не применимо (составное)")
    
    if ss_result:
        print(f"- Соловей-Штрассен: {error_probability_solovay_strassen(number, iterations):.10f}")
    else:
        print("- Соловей-Штрассен: не применимо (составное)")
    
    if mr_result:
        print(f"- Миллер-Рабин: {error_probability_miller_rabin(number, iterations):.10f}")
    else:
        print("- Миллер-Рабин: не применимо (составное)")


# =============================================
# ОСНОВНАЯ ФУНКЦИЯ
# =============================================
def main() -> None:
    """
    Основная функция для взаимодействия с пользователем.
    """
    print("\n=== ПРОВЕРКА ЧИСЛА НА ПРОСТОТУ ===")

    while True:
        try:
            number = int(input("\nВведите число для проверки: "))
            if number < 2:
                print("Число должно быть ≥ 2.")
                continue
            break
        except ValueError:
            print("Ошибка: введите целое число.")

    while True:
        try:
            iterations = int(input("Введите количество итераций: "))
            if iterations < 1:
                print("Число итераций должно быть ≥ 1.")
                continue
            break
        except ValueError:
            print("Ошибка: введите целое число.")

    print("\n=== РЕЗУЛЬТАТЫ ===")
    wrapper_ferma(number, iterations)
    wrapper_solovay_strassen(number, iterations)
    wrapper_miller_rabin(number, iterations)
    compare_tests(number, iterations)


if __name__ == "__main__":
    main()