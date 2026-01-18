import copy

class Polynomial:
    def __init__(self, coeffs, p):
        self.p = p
        while len(coeffs) > 1 and coeffs[-1] == 0:
            coeffs.pop()
        self.coeffs = [c % p for c in coeffs]

    def degree(self):
        return len(self.coeffs) - 1

    def is_zero(self):
        return self.coeffs == [0]

    def is_one(self):
        return self.coeffs == [1]

    def __repr__(self):
        if self.is_zero(): return "0"
        terms = []
        for i, c in enumerate(self.coeffs):
            if c != 0:
                if i == 0:
                    term = str(c)
                elif i == 1:
                    term = f"{c}x" if c != 1 else "x"
                elif c == 1:
                    term = f"x^{i}"
                else:
                    term = f"{c}x^{i}"
                terms.append(term)
        return " + ".join(reversed(terms))

    def __add__(self, other):
        max_len = max(len(self.coeffs), len(other.coeffs))
        new_coeffs = [0] * max_len
        for i in range(max_len):
            a = self.coeffs[i] if i < len(self.coeffs) else 0
            b = other.coeffs[i] if i < len(other.coeffs) else 0
            new_coeffs[i] = (a + b) % self.p
        return Polynomial(new_coeffs, self.p)

    def __sub__(self, other):
        max_len = max(len(self.coeffs), len(other.coeffs))
        new_coeffs = [0] * max_len
        for i in range(max_len):
            a = self.coeffs[i] if i < len(self.coeffs) else 0
            b = other.coeffs[i] if i < len(other.coeffs) else 0
            new_coeffs[i] = (a - b) % self.p
        return Polynomial(new_coeffs, self.p)

    def __mul__(self, other):
        if isinstance(other, int):
            return Polynomial([(c * other) % self.p for c in self.coeffs], self.p)
        
        deg_new = self.degree() + other.degree()
        new_coeffs = [0] * (deg_new + 1)
        for i in range(len(self.coeffs)):
            for j in range(len(other.coeffs)):
                new_coeffs[i + j] = (new_coeffs[i + j] + self.coeffs[i] * other.coeffs[j]) % self.p
        return Polynomial(new_coeffs, self.p)

    def div_mod(self, other):
        if other.is_zero(): raise ValueError("Деление на ноль")
        
        quotient = [0] * max(1, self.degree() - other.degree() + 1)
        remainder = copy.deepcopy(self)
        
        inv_leading = pow(other.coeffs[-1], -1, self.p)

        while remainder.degree() >= other.degree() and not remainder.is_zero():
            deg_diff = remainder.degree() - other.degree()
            coef_diff = (remainder.coeffs[-1] * inv_leading) % self.p
            
            quotient[deg_diff] = coef_diff
            
            sub_coeffs = [0] * (deg_diff + 1)
            sub_coeffs[-1] = coef_diff
            sub_poly = Polynomial(sub_coeffs, self.p)
            term_to_sub = sub_poly * other
            remainder = remainder - term_to_sub

        return Polynomial(quotient, self.p), remainder

    def __mod__(self, other):
        return self.div_mod(other)[1]
    
    def __truediv__(self, other):
        return self.div_mod(other)[0]

    def derivative(self):
        if self.degree() == 0: return Polynomial([0], self.p)
        new_coeffs = []
        for i in range(1, len(self.coeffs)):
            new_coeffs.append((self.coeffs[i] * i) % self.p)
        return Polynomial(new_coeffs, self.p)

    def pow_mod(self, exp, mod_poly):
        res = Polynomial([1], self.p)
        base = self
        while exp > 0:
            if exp % 2 == 1:
                res = (res * base) % mod_poly
            base = (base * base) % mod_poly
            exp //= 2
        return res

    def make_monic(self):
        if self.is_zero(): return self
        leading = self.coeffs[-1]
        inv = pow(leading, -1, self.p)
        return self * inv

def gcd_poly(a, b, indent=""):
    print(f"{indent}🔍 Вычисляем НОД({a}, {b})")
    step = 0
    while not b.is_zero():
        a, b = b, a % b
        step += 1
        print(f"{indent}  Шаг {step}: НОД = {a}")
    print(f"{indent}✅ НОД найден: {a.make_monic()}")
    return a.make_monic()

# --- 1. Алгоритм освобождения от квадратов ---
def square_free_decomposition(f, step=0):
    indent = "  " * step
    print(f"\n{indent}🎯 Square-Free ШАГ {step}")
    print(f"{indent}📊 Исходный полином: f(x) = {f}")
    print(f"{indent}📏 Степень: {f.degree()}")
    
    f = f.make_monic()
    
    # Шаг 1: Проверка базового случая
    if f.degree() < 1:
        print(f"{indent}✅ БАЗОВЫЙ СЛУЧАЙ: deg(f) < 1")
        print(f"{indent}➡️  Выход: []")
        return []
    
    if f.degree() == 1:
        print(f"{indent}✅ БАЗОВЫЙ СЛУЧАЙ: линейный полином")
        print(f"{indent}➡️  Выход: [{f}]")
        return [f]

    # Шаг 2: Производная
    print(f"{indent}📈 Шаг 2: Вычисляем производную f'(x)")
    g = f.derivative()
    print(f"{indent}     f'(x) = {g}")

    # Шаг 3: Проверка f'(x) = 0
    if g.is_zero():
        print(f"{indent}⚠️  Шаг 3: f'(x) = 0!")
        print(f"{indent}     Полином вида [v(x)]^p")
        print(f"{indent}     Извлекаем p-й корень...")
        
        v_coeffs = []
        for i in range(0, len(f.coeffs), f.p):
            v_coeffs.append(f.coeffs[i])
        v = Polynomial(v_coeffs, f.p)
        print(f"{indent}     v(x) = {v}")
        
        print(f"{indent}     Рекурсивный вызов Square-Free(v(x))")
        factors = square_free_decomposition(v, step+1)
        print(f"{indent}     Получено: {factors}")
        
        result = factors * f.p
        print(f"{indent}✅ Выход: {result}")
        return result

    # Шаг 4: НОД(f, f')
    print(f"{indent}🔗 Шаг 4: Вычисляем d(x) = НОД(f(x), f'(x))")
    d = gcd_poly(f, g, indent + "  ")
    print(f"{indent}     d(x) = {d}")

    # Шаг 5: Проверка d(x) = 1
    if d.degree() == 0:
        print(f"{indent}🎉 Шаг 5: deg(d) = 0!")
        print(f"{indent}     f(x) уже свободен от квадратов!")
        print(f"{indent}➡️  Выход: [{f}]")
        return [f]

    # Шаг 6: Разложение f = d * h
    print(f"{indent}✂️  Шаг 6: Разлагаем f(x) = d(x) * h(x)")
    h = f / d
    print(f"{indent}     h(x) = {h}")
    print(f"{indent}     ✓ Проверка: {d} * {h} = {d * h}")
    
    print(f"{indent}     🔄 Рекурсивно обрабатываем d(x)")
    d_factors = square_free_decomposition(d, step+1)
    print(f"{indent}     Получено из d(x): {d_factors}")
    
    result = d_factors + [h]
    print(f"{indent}🎯 ИТОГ ШАГА {step}: {result}")
    return result

# --- Улучшенный Гаусс ---
def solve_linear_system(matrix, p):
    print(f"\n{'='*50}")
    print(f"🧮 РЕШЕНИЕ СИСТЕМЫ X ⋅ (Q-E) = 0")
    print(f"{'='*50}")
    print(f"📊 Матрица размером {len(matrix)}×{len(matrix[0])}:")
    
    for i, row in enumerate(matrix):
        print(f"  строка {i+1:2d}: {row}")
    
    n, m = len(matrix), len(matrix[0])
    mat = [row[:] for row in matrix]
    pivot_row = 0
    col_to_pivot = {}
    
    print("\n🚀 ПРЯМОЙ ХОД ГАУССА:")
    print("-" * 30)
    
    for col in range(m):
        if pivot_row >= n: break
        
        # Поиск ведущего элемента
        curr = pivot_row
        while curr < n and mat[curr][col] == 0:
            curr += 1
        
        if curr == n: 
            print(f"  столбец {col}: все нули → свободная переменная")
            continue
        
        print(f"  столбец {col}: ведущий элемент в строке {curr}")
        
        # Перестановка строк
        mat[pivot_row], mat[curr] = mat[curr], mat[pivot_row]
        
        # Нормализация
        inv = pow(mat[pivot_row][col], -1, p)
        mat[pivot_row] = [(x * inv) % p for x in mat[pivot_row]]
        print(f"  строка {pivot_row+1} нормализована: {mat[pivot_row]}")
        
        # Зануление столбца
        for i in range(n):
            if i != pivot_row and mat[i][col] != 0:
                factor = mat[i][col]
                mat[i] = [(mat[i][k] - factor * mat[pivot_row][k]) % p for k in range(m)]
                print(f"  строка {i+1} занулена: {mat[i]}")
        
        col_to_pivot[col] = pivot_row
        pivot_row += 1
    
    print("\n🎯 НАЙДЕН БАЗИС НУЛЬ-ПРОСТРАНСТВА:")
    free_vars = [j for j in range(m) if j not in col_to_pivot]
    
    basis = []
    for idx, free_var in enumerate(free_vars):
        res = [0] * m
        res[free_var] = 1
        for pivot_col, row_idx in col_to_pivot.items():
            if free_var < len(mat[row_idx]):
                val = mat[row_idx][free_var]
                res[pivot_col] = (-val) % p
        
        h_poly = Polynomial(res, p)
        basis.append(res)
        print(f"  h_{idx+1}(x) = {h_poly}")
    
    print(f"{'='*50}\n")
    return basis

# --- 2. Алгоритм Берлекэмпа ---
def berlekamp_factorization(f):
    print(f"\n{'='*70}")
    print(f"🔬 АЛГОРИТМ БЕРЛЕКЭМПА")
    print(f"📊 Исходный полином: f(x) = {f}")
    print(f"📏 Степень n = {f.degree()}, поле ℤ_{f.p}")
    print(f"{'='*70}")
    
    f = f.make_monic()
    n = f.degree()
    p = f.p
    
    if n <= 1: 
        print("✅ Базовый случай (deg ≤ 1)")
        return [f] if n == 1 else []

    print("\n1️⃣ ПОСТРОЕНИЕ МАТРИЦЫ Q")
    print("   Строки Q: коэффициенты x^(p⋅i) mod f(x)")
    print("-" * 40)
    
    Q = [[0] * n for _ in range(n)]
    x_poly = Polynomial([0, 1], p)
    
    for i in range(n):
        val = x_poly.pow_mod(p * i, f)
        print(f"   x^{p*i} mod f(x) = {val}")
        
        for k, c in enumerate(val.coeffs):
            if k < n:
                Q[i][k] = c
        
        print(f"   Строка {i+1}: {Q[i]}")

    print("\n2️⃣ Формируем Q - E (единичная матрица)")
    for i in range(n):
        Q[i][i] = (Q[i][i] - 1) % p

    Q_T = [[Q[j][i] for j in range(n)] for i in range(n)]
    
    print("\n3️⃣ Решаем систему (Q-E)⋅X = 0")
    solutions = solve_linear_system(Q_T, p)
    
    print("\n4️⃣ РАСЩЕПЛЕНИЕ НА МНОЖИТЕЛИ")
    print("   Изначально F = {f(x)}")
    factors = [f]
    
    for i, sol_vec in enumerate(solutions):
        if all(x == 0 for x in sol_vec[1:]): 
            print(f"\n   ↩️  Пропускаем тривиальное решение h_{i+1}(x) = 1")
            continue
        
        h = Polynomial(sol_vec, p)
        print(f"\n   🔍 Обрабатываем h_{i+1}(x) = {h}")
        print(f"      Текущее F = {factors}")
        
        new_factors = []
        for u_idx, u in enumerate(factors):
            print(f"      • u_{u_idx+1}(x) = {u}")
            
            if u.degree() <= 1:
                print(f"        Линейный/константа → оставляем")
                new_factors.append(u)
                continue
            
            split_found = False
            for a in range(p):
                term = h - Polynomial([a], p)
                print(f"        a = {a}: gcd({u}, {h} - {a})")
                g = gcd_poly(u, term, "        ")
                
                if 0 < g.degree() < u.degree():
                    quotient = u / g
                    print(f"        🎉 РАЗБИЕНИЕ НАЙДЕНО!")
                    print(f"           {u} = {g} × {quotient}")
                    new_factors.append(g)
                    new_factors.append(quotient)
                    split_found = True
                else:
                    print(f"        Неудача (deg g = {g.degree()})")
            
            if not split_found:
                print(f"        Не удалось расщепить → оставляем {u}")
                new_factors.append(u)
        
        factors = new_factors
        print(f"      ➡️  Новое F = {factors}")
        
        if len(factors) == len(solutions):
            print("      ✅ Все множители найдены!")
            break

    print(f"\n🎉 ИТОГ БЕРЛЕКЭМПА: {factors}")
    print(f"{'='*70}\n")
    return factors

# --- Главная функция ---
def factorize_polynomial(coeffs, p):
    print("🚀" + " ПОЛНАЯ ФАКТОРИЗАЦИЯ ПОЛИНОМА " + "🚀")
    print("=" * 80)
    f_orig = Polynomial(coeffs, p)
    print(f"📋 Исходный полином: f(x) = {f_orig}")
    print(f"🌐 Поле: ℤ_{p}")
    print("=" * 80)

    f = f_orig
    
    print("\n" + "📋 ЭТАП 1: ОСВОБОЖДЕНИЕ ОТ КВАДРАТОВ".center(80, "="))
    sq_free_parts = square_free_decomposition(f, 0)
    print(f"\n✅ Результат: f(x) = {' × '.join(map(str, sq_free_parts))}")
    
    print("\n" + "📋 ЭТАП 2: РАЗЛОЖЕНИЕ НА НЕПРИВОДИМЫЕ".center(80, "="))
    final_factors = []
    
    for i, part in enumerate(sq_free_parts):
        if part.degree() == 0: continue
        print(f"\n🔸 Обрабатываем часть {i+1}: {part}")
        
        if part.degree() == 1:
            print("   ➡️ Линейный полином (неприводимый)")
            final_factors.append(part)
        else:
            print("   ➡️ Применяем алгоритм Берлекэмпа:")
            berl_factors = berlekamp_factorization(part)
            final_factors.extend(berl_factors)
    
    print("\n" + "🏆 КОНЕЧНЫЙ РЕЗУЛЬТАТ".center(80, "="))
    print("f(x) разложен на неприводимые множители:")
    for i, fac in enumerate(final_factors):
        print(f"   f_{i+1}(x) = {fac}")
    
    print("\n✅ ПРОВЕРКА:")
    product = Polynomial([1], p)
    for fac in final_factors:
        product = product * fac
    print(f"   {' × '.join(map(str, final_factors))} = {product}")
    print(f"   ✓ Совпадает с исходным: {f_orig == product}")
    print("=" * 80)
    
    return final_factors

# --- Запуск примеров ---
if __name__ == "__main__":
    print("📚 ПРИМЕРЫ ИЗ ТЕКСТА\n")
    
    print("🎯 ПРИМЕР 1")
    print("x⁷ + x⁶ + x⁵ + x⁴ + x² + 1 ∈ ℤ₂[x]")
    print("Коэффициенты: [1,0,1,0,1,1,1,1]\n")
    res1 = factorize_polynomial([1, 0, 1, 0, 1, 1, 1, 1], 2)
    
    print("\n" + "="*100 + "\n")
    
    print("🎯 ПРИМЕР 2") 
    print("x⁴ + 3x² + 2 ∈ ℤ₅[x]")
    print("Коэффициенты: [2,0,3,0,1]\n")
    res2 = factorize_polynomial([3, 0, 4, 2, 0, 1], 5)
