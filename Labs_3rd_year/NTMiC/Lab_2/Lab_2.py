import time
import sys

# Увеличиваем лимит на количество цифр
sys.set_int_max_str_digits(100000)  

def fast_power(base: int, exponent: int) -> int:
    if base == 0 and exponent == 0:
        return -1
    if exponent == 0:
        return 1
    result = base
    for i in range(exponent.bit_length() - 2, -1, -1):
        result = result ** 2
        if (exponent >> i) & 1:
            result *= base 
    return result

def standard_power(base: int, exponent: int) -> int:
    if base == 0 and exponent == 0:
        return -1
    result = 1
    for _ in range(exponent):
        result *= base
    return result

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
    base = get_positive_integer_input("Введите число: ")
    exponent = get_positive_integer_input("Введите степень: ")
    
    # Замер времени для встроенной функции pow
    time_builtin_start = time.time()
    builtin_pow_result = pow(base, exponent)
    time_builtin_finish = time.time()
    time_builtin_result = time_builtin_finish - time_builtin_start
    
    # Замер времени для fast_power
    time_fast_start = time.time()
    fast_power_result = fast_power(base, exponent)
    time_fast_finish = time.time()
    time_fast_result = time_fast_finish - time_fast_start
    
    # Замер времени для standard_power
    time_standard_start = time.time()
    standard_power_result = standard_power(base, exponent)
    time_standard_finish = time.time()
    time_standard_result = time_standard_finish - time_standard_start
    if base == 0 and exponent == 0:
        print("\n Неопрделеннность")
    else:
    # Вывод результатов
        print(f"\nРезультаты вычислений:")
        print(f"{base} ^ {exponent} = {builtin_pow_result} (Встроенная функция pow)")
        print(f"{base} ^ {exponent} = {fast_power_result} (fast_power)")
        print(f"{base} ^ {exponent} = {standard_power_result} (standard_power)")
    
    # Вывод времени выполнения
    print("\nВремя выполнения:")
    print(f"Встроенная функция pow: {time_builtin_result:.6f} секунд")
    print(f"fast_power: {time_fast_result:.6f} секунд")
    print(f"standard_power: {time_standard_result:.6f} секунд")
    
    if base == 0 == exponent:
        print("\nРезультаты не определены для данного выражения.")
    # Сравнение результатов
    if fast_power_result == standard_power_result == builtin_pow_result:
        print("\nРезультаты всех методов совпадают.")
    else:
        print("\nРезультаты не совпадают.")

if __name__ == "__main__":
    main()