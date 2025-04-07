import random
import time
from functools import lru_cache


def test_ferma(number: int, parameter: int) -> int:
    """
    Проверяет простоту числа number с помощью теста Ферма.
    """
    for _ in range(parameter):
        a = random.randint(2, number - 2)
        r = pow(a, number - 1, number)
        if r != 1:
            return 1
    return 0

@lru_cache(maxsize=None)  # Неограниченный кэш
def euler_phi(number: int) -> int:
    """
    Вычисляет функцию Эйлера для числа number.
    """
    result = number
    # Обработка делителя 2
    if number % 2 == 0:
        while number % 2 == 0:
            number //= 2
        result //= 2
    # Обработка нечетных делителей
    i = 3
    while i * i <= number:
        if number % i == 0:
            while number % i == 0:
                number //= i
            result //= i
            result *= (i - 1)
        i += 2
    # Если остался простой делитель больше 2
    if number > 1:
        result //= number
        result *= (number - 1)
    return result


def error_probability(number: int, iterations: int) -> None:
    """
    Вычисляет вероятность ошибки теста Ферма для числа number и количества итераций iterations.
    """
    probability = 1 - ((euler_phi(number) / number) ** iterations)
    print(f"Вероятность ошибки = {probability}")


def wrapper_test_ferma(number: int, parameter: int) -> None:
    """
    Обертка для теста Ферма, которая обрабатывает входные данные и выводит результат.
    """
    if number <= 3 and number % 2 == 0:
        print("Число не подходит под условия\n")
    else:
        result = test_ferma(number, parameter)
        if result == 1:
            print("Составное")
        else:
            print("Простое")
            error_probability(number, parameter)


def main() -> None:
    """
    Основная функция для взаимодействия с пользователем.
    """
    while True:
        try:
            number = int(input("Введите число для проверки на простоту: "))
            if number <= 3 or number % 2 == 0:
                print("Ошибка")
            else:
                break
        except ValueError:
            print("Пожалуйста, введите целое число.")

    while True:
        try:
            parameter = int(input("Введите количество итераций для теста Ферма: "))
            if parameter <= 0:
                print("Ошибка")
            else:
                break
        except ValueError:
            print("Пожалуйста, введите целое число.")

    time_start = time.time()
    wrapper_test_ferma(number, parameter)
    time_finish = time.time()
    print(f"Время выполнения теста: {time_finish - time_start}")
if __name__ == "__main__":
    main()