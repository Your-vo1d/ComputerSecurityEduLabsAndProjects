def compute_m_prime(m, b):
    """
    Вычисляет m' = -m^{-1} mod b.
    
    :param m: Модуль.
    :param b: Основание системы счисления.
    :return: m' = -m^{-1} mod b.
    """
    def extended_gcd(a, b):
        """
        Расширенный алгоритм Евклида для нахождения НОД и коэффициентов Безу.
        """
        if a == 0:
            return b, 0, 1
        else:
            g, x, y = extended_gcd(b % a, a)
            return g, y - (b // a) * x, x

    # Находим НОД и коэффициенты Безу
    g, x, _ = extended_gcd(m, b)
    
    # Если НОД(m, b) != 1, обратное не существует
    if g != 1:
        raise ValueError("Обратное не существует, так как m и b не взаимно просты.")
    
    # Модулярное обратное m^{-1} mod b
    m_inv = x % b
    
    # Вычисляем m' = -m_inv mod b
    m_prime = (-m_inv) % b
    return m_prime


def montgomery_reduction(x, m, b):
    """
    Выполняет алгоритм преобразования Монтгомери с обратной индексацией.
    
    :param x: Число, которое нужно преобразовать.
    :param m: Модуль.
    :param b: Основание системы счисления.
    :return: Результат преобразования Монтгомери.
    """
    # Вычисляем m'
    m_prime = compute_m_prime(m, b)
    print(f"Вычислено m' = -m^(-1) mod b = {m_prime}")

    # Шаг 1: Положить y = x
    y = x
    print(f"MR. Шаг 1: Положить y = x. y = {y}")

    # Шаг 2: Цикл по i от 0 до k-1
    k = len(str(m))  # Количество цифр в m (для наглядности используем длину строки)
    print(f"MR. Шаг 2: Начинаем цикл по i от 0 до k-1, где k = {k} (количество цифр в m).")

    for i in range(k):
        print(f"\nMR. Шаг 2. Итерация i = {i}:")

        # Шаг 2.1: Вычислить u = (y_i * m_prime) mod b
        y_i = (y // (b ** i)) % b  # i-й разряд справа (младший разряд)
        print(f"MR. Шаг 2.1: Вычисляем y_i = (y // (b^{i})) % b = {y_i}")
        
        u = (y_i * m_prime) % b
        print(f"MR. Шаг 2.1: Вычисляем u = (y_i * m_prime) % b = {u}")

        # Шаг 2.2: Положить y = y + u * m * (b ** i)
        y_old = y  # Сохраняем старое значение y для вывода
        y = y + u * m * (b ** i)
        print(f"MR. Шаг 2.2: Положить y = y + u * m * (b^{i}) = {y_old} + {u} * {m} * {b ** i} = {y}")

    # Шаг 3: Положить y = y // (b ** k)
    y_old = y  # Сохраняем старое значение y для вывода
    y = y // (b ** k)
    print(f"\nMR. Шаг 3: Положить y = y // (b^{k}) = {y_old} // {b ** k} = {y}")

    # Шаг 4: Если y >= m, уменьшить y на m
    if y >= m:
        y_old = y  # Сохраняем старое значение y для вывода
        y -= m
        print(f"MR. Шаг 4: y >= m, поэтому уменьшаем y на m: y = {y_old} - {m} = {y}")
    else:
        print(f"MR. Шаг 4: y < m, поэтому y не изменяется: y = {y}")

    # Шаг 5: Вернуть результат
    print(f"\nMR. Шаг 5: Результат преобразования Монтгомери: {y}")
    return y


def montgomery_multiplication(x, y, m, b):
    """
    Выполняет алгоритм произведения Монтгомери.
    
    :param x: Первое число.
    :param y: Второе число.
    :param m: Модуль.
    :param b: Основание системы счисления.
    :return: Результат произведения Монтгомери.
    """
    # Вычисляем m'
    m_prime = compute_m_prime(m, b)
    print(f"Вычислено m' = -m^(-1) mod b = {m_prime}")

    # Шаг 1: Положить z = 0
    z = 0
    print(f"MP. Шаг 1: Положить z = 0. z = {z}")

    # Шаг 2: Цикл по i от 0 до k-1
    k = len(str(m))  # Количество цифр в m (для наглядности используем длину строки)
    print(f"MP. Шаг 2: Начинаем цикл по i от 0 до k-1, где k = {k} (количество цифр в m).")

    for i in range(k):
        print(f"\nMP. Шаг 2. Итерация i = {i}:")

        # Шаг 2.1: Вычислить u = (z_0 + x_i * y_0) * m' mod b
        z_0 = z % b  # Младший разряд z
        x_i = (x // (b ** i)) % b  # i-й разряд x справа
        y_0 = y % b  # Младший разряд y
        u = (z_0 + x_i * y_0) * m_prime % b
        print(f"MP. Шаг 2.1: Вычисляем u = (z_0 + x_i * y_0) * m' mod b = ({z_0} + {x_i} * {y_0}) * {m_prime} mod {b} = {u}")

        # Шаг 2.2: Положить z = (z + x_i * y + u * m) // b
        z_old = z  # Сохраняем старое значение z для вывода
        z = (z + x_i * y + u * m) // b
        print(f"MP. Шаг 2.2: Положить z = (z + x_i * y + u * m) // b = ({z_old} + {x_i} * {y} + {u} * {m}) // {b} = {z}")

    # Шаг 3: Если z >= m, уменьшить z на m
    if z >= m:
        z_old = z  # Сохраняем старое значение z для вывода
        z -= m
        print(f"MP. Шаг 3: z >= m, поэтому уменьшаем z на m: z = {z_old} - {m} = {z}")
    else:
        print(f"MP. Шаг 3: z < m, поэтому z не изменяется: z = {z}")

    # Шаг 4: Вернуть результат
    print(f"\nMP. Шаг 4: Результат произведения Монтгомери: {z}")
    return z

def compute_R2(m, b):
    """
    Вычисляет R2 = b^{2k} mod m, где k — количество цифр в m по основанию b.
    
    :param m: Модуль.
    :param b: Основание системы счисления.
    :return: R2 = b^{2k} mod m.
    """
    # Вычисляем k как количество цифр в m по основанию b
    k = 0
    temp = m
    while temp > 0:
        temp = temp // b
        k += 1

    # Вычисляем R2 = b^{2k} mod m
    R2 = pow(b, 2 * k, m)
    print(f"Вычислено R2 = b^{2*k} mod m = {R2}")
    return R2

def montgomery_exponentiation(x, y, m, b):
    """
    Выполняет алгоритм возведения в степень по модулю методом Монтгомери.
    
    :param x: Основание степени.
    :param y: Показатель степени в двоичном виде (начиная со старшего бита).
    :param m: Модуль.
    :param b: Основание системы счисления.
    :return: Результат возведения в степень по модулю.
    """
    # Вычисляем R2
    R2 = compute_R2(m, b)

    # Шаг 1: Вычислить x' = MP(x, R2, b, m) и положить z = x'
    x_prime = montgomery_multiplication(x, R2, m, b)
    z = x_prime
    print(f"ME. Шаг 1: Вычислено x' = MP(x, R2, b, m) = {x_prime}. z = {z}")

    # Шаг 2: Цикл по i от n-2 до 0
    n = y.bit_length()  # Количество бит в y
    print(f"ME. Шаг 2: Начинаем цикл по i от {n-2} до 0.")

    for i in range(n - 2, -1, -1):
        print(f"\nME. Шаг 2. Итерация i = {i}:")

        # Шаг 2.1: Вычислить q = z, затем z = MR(q, b, m)
        q = z**2
        z = montgomery_reduction(q, m, b)
        print(f"ME. Шаг 2.1: Вычислено q = z = {q}. Затем z = MR(q, b, m) = {z}")

        # Шаг 2.2: Если y_i = 1, положить z = MP(z, x', b, m)
        y_i = (y >> i) & 1  # i-й бит y
        print(f"ME. Шаг 2.2: Проверяем y_i (бит {i} числа y): y_i = {y_i}")

        if y_i == 1:
            z_old = z  # Сохраняем старое значение z для вывода
            z = montgomery_multiplication(z, x_prime, m, b)
            print(f"ME. Шаг 2.2: y_i = 1, поэтому z = MP(z, x', b, m) = {z_old} * {x_prime} mod m = {z}")
        else:
            print(f"ME. Шаг 2.2: y_i = 0, поэтому z не изменяется: z = {z}")

    # Шаг 3: Положить z = MR(z, b, m)
    z_old = z  # Сохраняем старое значение z для вывода
    z = montgomery_reduction(z, m, b)
    print(f"\nME. Шаг 3: Положить z = MR(z, b, m) = {z_old} mod m = {z}")

    # Шаг 4: Ответ: z
    print(f"\nME. Шаг 4: Результат возведения в степень по модулю: {z}")
    return z


def montgomery_exponentiation_file(input_file):
    """
    Читает входные данные из файла, выполняет алгоритм возведения в степень по модулю методом Монтгомери
    и записывает результаты каждого теста в отдельный файл.
    
    :param input_file: Имя файла с входными данными.
    """

    # Открываем файл для чтения
    with open(input_file, 'r', encoding="UTF-8") as f:
        lines = f.readlines()

    # Обрабатываем каждую строку
    for line in lines:
        # Убираем лишние символы и разбиваем строку на части
        line = line.strip().replace(';', '')  # Убираем точку с запятой
        parts = line.split(', ')  # Разбиваем по запятой и пробелу

        # Извлекаем значения x, y, m
        x = int(parts[0].split(' = ')[1])
        y = int(parts[1].split(' = ')[1])
        m = int(parts[2].split(' = ')[1])

        # Извлекаем номер теста
        test_number = line.split('.')[0]

        # Создаем имя файла для текущего теста
        output_file = f"output_{test_number}.txt"

        # Открываем файл для записи
        with open(output_file, 'w', encoding="UTF-8") as f:
            # Перенаправляем вывод в файл
            import sys
            sys.stdout = f

            # Выводим номер теста
            print(f"Тест {test_number}: x = {x}, y = {y}, m = {m}")

            # Выполняем алгоритм
            result = montgomery_exponentiation(x, y, m, b=10)  # Основание b = 16

            # Возвращаем вывод в консоль
            sys.stdout = sys.__stdout__

        print(f"Результаты теста {test_number} записаны в файл {output_file}")

# Пример использования
montgomery_exponentiation_file('input.txt')

m = 227
b = 10
x = 201
y = 8

result = montgomery_exponentiation(x, y, m, b)
print("Результат преобразования Монтгомери:", result)