import time

def barret_algorithm(dividend: int, diviser: int, pre_calculation: int, base: int, number_diviser: int) -> int:
    if base <= 3 or diviser <= 0 or len(str(dividend)) > 2 * len(str(diviser)):
        print("Ошибка")
        return -1  # Возвращаем -1 в случае некорректных входных данных

    # Вычисляем приближенное частное
    approximate_quotient = ((dividend // base ** (number_diviser - 1)) * pre_calculation) // base ** (number_diviser + 1)

    # Вычисляем остатки
    r_1 = dividend % base ** (number_diviser + 1)
    r_2 = (approximate_quotient * diviser) % base ** (number_diviser + 1)

    # Корректируем остаток
    if r_1 >= r_2:
        r_sub = r_1 - r_2
    else:
        r_sub = base ** (number_diviser + 1) + r_1 - r_2

    # Корректируем r_sub, если он больше diviser
    while r_sub >= diviser:
        r_sub -= diviser

    return r_sub

def get_positive_integer_input(prompt: str) -> int:
    while True:
        try:
            value = int(input(prompt))
            if value < 0:
                print("Пожалуйста, введите положительное число.")
            else:
                return value
        except ValueError:
            print("Пожалуйста, введите целое число.")

def main():
    dividend = get_positive_integer_input("Введите число: ")
    diviser = 0
    while diviser == 0:
        diviser = get_positive_integer_input("Введите делитель: ")

    number_diviser = len(str(diviser))
    base = 10
    pre_calculation = pow(base, 2 * number_diviser) // diviser

    # Измерение времени для встроенной операции %
    time_builtin_start = time.time()
    builtin_mod_result = dividend % diviser
    time_builtin_finish = time.time()
    time_builtin_result = time_builtin_finish - time_builtin_start

    # Измерение времени для алгоритма Баррета
    time_barret_start = time.time()
    barret_algorithm_mod_result = barret_algorithm(dividend, diviser, pre_calculation, base, number_diviser)
    if barret_algorithm_mod_result == -1:
        return 0
    time_barret_finish = time.time()
    time_barret_result = time_barret_finish - time_barret_start

    print(f"\nРезультаты вычислений:")
    print(f"{dividend} mod {diviser} = {builtin_mod_result} (Встроенная функция '%'). Время: {time_builtin_result:.6f} секунд")
    print(f"{dividend} mod {diviser} = {barret_algorithm_mod_result} (Алгоритм Баррета). Время: {time_barret_result:.6f} секунд")

if __name__ == "__main__":
    main()