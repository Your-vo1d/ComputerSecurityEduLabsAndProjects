def fast_square(x, b):
    n = len(x)
    y = [0] * (2 * n)
    
    for i in range(n):
        # Шаг 2.1
        uv = y[2 * i] + x[i] * x[i]
        y[2 * i] = uv % b
        c = uv // b
        
        for j in range(i + 1, n):
            # Шаг 2.2
            cuv = y[i + j] + 2 * x[i] * x[j] + c
            y[i + j] = cuv % b
            c = cuv // b
        
        # Шаг 2.3
        y[i + n] += c
    
    return y


number = input("Введите число: ")
base = int(input("Введите основание системы счисления: "))


x = [int(digit) for digit in number]

result = fast_square(x, base)

print("Результат возведения в квадрат:", result)