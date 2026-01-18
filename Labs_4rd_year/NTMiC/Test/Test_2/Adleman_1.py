import random
import math

# ---------------- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ----------------

def is_prime(n):
    """Проверка простоты наивным способом (для учебных небольших p)."""
    if n < 2:
        return False
    if n % 2 == 0:
        return n == 2
    d = 3
    while d * d <= n:
        if n % d == 0:
            return False
        d += 2
    return True

def generate_primes_up_to(limit):
    """Генерация всех простых до limit включительно."""
    res = []
    for x in range(2, limit+1):
        if is_prime(x):
            res.append(x)
    return res

def first_t_primes(t):
    """Первые t простых чисел."""
    res = []
    x = 2
    while len(res) < t:
        if is_prime(x):
            res.append(x)
        x += 1
    return res

def factor_over_base(n, base):
    """
    Разложение n по модулю обычной арифметики (НЕ по модулю p)
    на простые из факторной базы base.
    Возвращает список показателей e_i, если удалось,
    либо None, если число не является полностью base-разложимым.
    """
    original = n
    exps = [0] * len(base)
    for i, p in enumerate(base):
        while n % p == 0:
            n //= p
            exps[i] += 1
    if n != 1:
        # Остался множитель не из базы
        return None
    return exps

def egcd(a, b):
    """Расширенный алгоритм Евклида."""
    if b == 0:
        return (1, 0, a)
    x1, y1, g = egcd(b, a % b)
    x, y = y1, x1 - (a // b) * y1
    return (x, y, g)

def modinv(a, m):
    """Обратный элемент к a по модулю m, если существует."""
    x, y, g = egcd(a, m)
    if g != 1:
        return None
    return x % m

# Решение системы линейных уравнений A * x ≡ b (mod m) методом Гаусса.
def solve_linear_mod(A, b, m):
    """
    A — матрица (список списков), b — вектор, m — модуль.
    Возвращает одно решение x (если нашлось).
    Для учебных целей: не обрабатывает все тонкие случаи,
    но достаточно для небольших примеров.
    """
    n_rows = len(A)
    n_cols = len(A[0])
    # Расширенная матрица
    M = [row[:] + [b[i]] for i, row in enumerate(A)]

    row = 0
    for col in range(n_cols):
        # Ищем ведущую строку
        pivot = None
        for r in range(row, n_rows):
            if M[r][col] % m != 0:
                pivot = r
                break
        if pivot is None:
            continue
        # Меняем строки местами
        M[row], M[pivot] = M[pivot], M[row]

        # Нормируем ведущий элемент к 1
        inv = modinv(M[row][col] % m, m)
        if inv is None:
            # Делить нельзя, пропустим этот столбец
            continue
        for c in range(col, n_cols+1):
            M[row][c] = (M[row][c] * inv) % m

        # Обнуляем в других строках
        for r in range(n_rows):
            if r != row and M[r][col] % m != 0:
                factor = M[r][col] % m
                for c in range(col, n_cols+1):
                    M[r][c] = (M[r][c] - factor * M[row][c]) % m

        row += 1
        if row == n_rows:
            break

    # Читаем решение: x_j = M[j][n_cols]
    x = [0] * n_cols
    for r in range(min(n_rows, n_cols)):
        # ищем ведущий 1 в строке
        lead_col = None
        for c in range(n_cols):
            if M[r][c] % m == 1:
                lead_col = c
                break
        if lead_col is not None:
            x[lead_col] = M[r][n_cols] % m
    return x

# ---------------- АЛГОРИТМ АДЛЕМАНА ----------------

def adleman_index_calculus(p, g, a, t, max_relations=1000, verbose=True):
    """
    Алгоритм Адлемана (index calculus) в группе (Z/pZ)*.
    p — простой модуль
    g — генератор (или элемент большого порядка)
    a — элемент, для которого ищем x: g^x ≡ a (mod p)
    t — размер факторной базы (кол-во первых простых)
    max_relations — ограничение попыток при поиске соотношений (для безопасности цикла)
    """

    if verbose:
        print("=== Алгоритм Адлемана (index calculus) в группе (Z/{0}Z)* ===".format(p))
        print("Ищем x из уравнения: g^x ≡ a (mod p)")
        print("p = {0}, g = {1}, a = {2}".format(p, g, a))
        print()

    n = p - 1  # порядок группы (Z/pZ)*

    # 1. Выбор факторной базы S = {p1, ..., pt}
    factor_base = first_t_primes(t)
    if verbose:
        print("Шаг 1. Выбираем факторную базу S.")
        print("Первые t = {0} простых чисел: S = {1}".format(t, factor_base))
        print("Факторная база состоит из наименьших простых, по порядку возрастания.")
        print()

    # 2. Сбор соотношений g^k = \prod p_i^{e_i} mod p
    relations = []  # список векторов e_i
    ks = []         # соответствующие k
    attempts = 0
    needed = t      # нужно минимум t независимых соотношений

    if verbose:
        print("Шаг 2. Ищем разложения чисел вида g^k (mod p) в факторной базе.")
        print("Будем выбирать случайные k, вычислять b = g^k (mod p)")
        print("и пытаться представить b как произведение простых из S.")
        print()

    while len(relations) < needed and attempts < max_relations:
        attempts += 1
        k = random.randrange(1, n)  # 0 < k < n
        b = pow(g, k, p)

        if verbose:
            print("  Попытка #{0}".format(attempts))
            print("    Выбираем случайное k в (0, n): k = {0}".format(k))
            print("    Вычисляем b = g^k (mod p) = {0}^{1} (mod {2}) = {3}".format(g, k, p, b))
            print("    Пытаемся разложить b = {0} по факторной базе S = {1}".format(b, factor_base))

        # разложение обычное целое, поэтому сначала "поднимаем" b в [0, p-1]
        # b уже в этом диапазоне, но нам нужно разложить по базовым простым
        exps = factor_over_base(b, factor_base)
        if exps is None:
            if verbose:
                print("    Число b = {0} НЕ разлагается полностью по базе, пропускаем.".format(b))
                print()
            continue

        if verbose:
            print("    Удалось разложить b:")
            # строим строку вида b = p1^e1 * p2^e2 * ...
            parts = []
            for pi, ei in zip(factor_base, exps):
                if ei > 0:
                    parts.append("{0}^{1}".format(pi, ei))
            if parts:
                print("      {0} = {1}".format(b, " * ".join(parts)))
            else:
                print("      {0} = 1 (нет простых из базы с ненулевыми показателями)".format(b))

            print("    Из разложения получаем линейное сравнение по модулю n = p-1:")
            # g^k ≡ \prod p_i^{e_i} (mod p)
            # => k ≡ sum(e_i * log_g(p_i)) (mod n)
            # Коэффициенты при неизвестных log_g(p_i) — это e_i
            # Запишем его в текстовом виде:
            sum_terms = []
            for pi, ei in zip(factor_base, exps):
                if ei != 0:
                    sum_terms.append("{0} * log_g({1})".format(ei, pi))
            if sum_terms:
                print("      k = {0} ≡ {1} (mod {2})".format(k, " + ".join(sum_terms), n))
            else:
                print("      k = {0} ≡ 0 (mod {1})".format(k, n))
            print()

        relations.append(exps)
        ks.append(k)

    if len(relations) < needed:
        print("Не удалось набрать достаточно соотношений (требовалось {0}).".format(needed))
        return None

    if verbose:
        print("Набрано {0} соотношений, переходим к шагу 3.".format(len(relations)))
        print()

    # 3. Решаем систему линейных сравнений для log_g(p_i)
    #    Матрица A: строки = exps, столбцы = i
    #    Вектор b: ks
    A = relations
    b_vec = ks

    if verbose:
        print("Шаг 3. Решаем систему линейных сравнений для логарифмов элементов факторной базы.")
        print("Система имеет вид:")
        for i in range(len(A)):
            exps = A[i]
            k = b_vec[i]
            lhs = " + ".join(
                "{0}*L_{1}".format(exps[j], factor_base[j]) for j in range(len(factor_base))
            )
            print("  {0} ≡ {1} (mod {2})".format(k, lhs, n))
        print()
        print("Решаем систему A * L ≡ k (mod {0}), где L_i = log_g(p_i).".format(n))
        print()

    logs_base = solve_linear_mod(A, b_vec, n)

    if verbose:
        print("Найдены значения логарифмов log_g(p_i) для p_i из факторной базы:")
        for pi, Li in zip(factor_base, logs_base):
            print("  log_g({0}) = {1} (mod {2})".format(pi, Li, n))
        print()

    # 4. Ищем x = log_g(a)
    if verbose:
        print("Шаг 4. Находим x = log_g(a).")
        print("Будем перебирать случайные k, вычислять b = a * g^k (mod p)")
        print("и пытаться разложить b по факторной базе.")
        print("Если удастся, то из равенства:")
        print("  a * g^k ≡ ∏ p_i^{e_i} (mod p)")
        print("получим:")
        print("  log_g(a) + k ≡ Σ e_i * log_g(p_i) (mod n)")
        print("откуда:")
        print("  x = log_g(a) ≡ Σ e_i * log_g(p_i) - k (mod n)")
        print()

    attempts = 0
    while attempts < max_relations:
        attempts += 1
        k = random.randrange(0, n)  # 0 ≤ k < n
        b = (a * pow(g, k, p)) % p

        if verbose:
            print("  Попытка #{0} на шаге 4".format(attempts))
            print("    Выбираем k в [0, n): k = {0}".format(k))
            print("    Вычисляем b = a * g^k (mod p) = {0} * {1}^{2} (mod {3}) = {4}".format(a, g, k, p, b))
            print("    Пытаемся разложить b = {0} по факторной базе S = {1}".format(b, factor_base))

        exps = factor_over_base(b, factor_base)
        if exps is None:
            if verbose:
                print("    Число b = {0} НЕ разлагается полностью по базе, возвращаемся к шагу 4.1.".format(b))
                print()
            continue

        if verbose:
            print("    Удалось разложить b:")
            parts = []
            for pi, ei in zip(factor_base, exps):
                if ei > 0:
                    parts.append("{0}^{1}".format(pi, ei))
            if parts:
                print("      {0} = {1}".format(b, " * ".join(parts)))
            else:
                print("      {0} = 1 (нет простых из базы с ненулевыми показателями)".format(b))

        # Теперь считаем правую часть: sum(e_i * log_g(p_i))
        rhs = 0
        for ei, Li in zip(exps, logs_base):
            rhs = (rhs + ei * Li) % n

        x = (rhs - k) % n

        if verbose:
            print("    Из равенства a*g^k ≡ ∏ p_i^{e_i} (mod p) получаем:")
            sum_terms = []
            for ei, pi in zip(exps, factor_base):
                if ei != 0:
                    sum_terms.append("{0}*log_g({1})".format(ei, pi))
            if sum_terms:
                print("      log_g(a) + k ≡ {0} (mod {1})".format(" + ".join(sum_terms), n))
            else:
                print("      log_g(a) + k ≡ 0 (mod {0})".format(n))
            print("    Подставляем найденные log_g(p_i):")
            print("      Σ e_i * log_g(p_i) (mod n) = {0}".format(rhs))
            print("    Тогда:")
            print("      x = log_g(a) ≡ {0} - {1} ≡ {2} (mod {3})".format(rhs, k, x, n))

        # Проверка решения
        if pow(g, x, p) == a % p:
            if verbose:
                print()
                print("Проверка: g^x (mod p) = {0}^{1} (mod {2}) = {3}".format(g, x, p, pow(g, x, p)))
                print("Это равно a = {0}, значит решение верно.".format(a))
                print()
                print("Ответ алгоритма: x = {0}".format(x))
            return x
        else:
            if verbose:
                print("    Проверка не прошла: g^x (mod p) != a, пробуем другой k.")
                print()

    print("Не удалось найти x за отведённое число попыток на шаге 4.")
    return None

# ---------------- ПРИМЕР ЗАПУСКА ----------------

if __name__ == "__main__":
    # Небольшой учебный пример
    p = 101        # простой модуль
    g = 2          # предполагаемый генератор
    a = 37         # ищем x: 2^x ≡ 37 (mod 101)
    t = 5          # размер факторной базы (первые 5 простых: 2,3,5,7,11)

    x = adleman_index_calculus(p, g, a, t, verbose=True)
    print("Результат: x =", x)
