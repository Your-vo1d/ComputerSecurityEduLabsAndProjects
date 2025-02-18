import time


def to_decimal(number, base):
    decimal_number = 0
    for i, digit in enumerate(number):
        decimal_number += digit * (base ** i)
    return decimal_number

def fast_square(x, b):
    x = x[::-1]
    n = len(x)
    y = [0] * (2 * n + 1)
    
    for i in range(n):
        # Шаг 2.1
        uv = y[2 * i] + pow(x[i], 2)
        y[2 * i] = uv % b
        cu = uv // b
        two_x_i = 2 * x[i]
        # Шаг 2.2
        for j in range(i + 1, n):
            cuv = y[i + j] + two_x_i * x[j] + cu
            y[i + j] = cuv % b
            cu = cuv // b
        y[i + n] += cu % b
        y[i + n + 1] += cu // b
    return y

if __name__ == "__main__":

    # Ввод основания системы счисления числа
    while True:
        try:
            base = int(input("Введите основание системы счисления (не менее 2): "))
            if base < 2:
                print("Ошибка: Основание системы счисления должно быть больше или равно 2.")
            else:
                break
        except ValueError:
            print("Ошибка: Введено нечисловое значение. Пожалуйста, введите целое число.")

    while True:
        try:
            number = int(input("Введите число: "))
            if number < 0:
                print("Ошибка: Основание число должно быть больше или равно 0.")
            else:
                break
        except ValueError:
            print("Ошибка: Введено нечисловое значение. Пожалуйста, введите целое число.")
    number_test = number
    number_list = []
    while number_test > 0:
        d = number_test % base
        number_list.insert(0, d)
        number_test //= base

    # Результат для функции fast_square
    time_fast_square_start = time.time()
    result_fast_square = fast_square(number_list, base)
    time_fast_square_finish = time.time()
    result_time_fast_square = time_fast_square_finish - time_fast_square_start

    result_fast_square = to_decimal(result_fast_square, base)
    # Результат для обычного возведения в квадрат
    time_standart_start = time.time()
    result_standard = number * number
    time_standart_finish = time.time()
    result_starndart = time_standart_finish - time_standart_start
    # Вывод результатов
    print("Результат возведения в квадрат (fast_square):", result_fast_square)
    print(result_time_fast_square)
    print("Результат возведения в квадрат (стандартный):", result_standard)
    print(result_starndart)