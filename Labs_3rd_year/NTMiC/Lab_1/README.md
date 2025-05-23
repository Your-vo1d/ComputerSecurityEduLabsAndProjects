# Алгоритм быстрого возведения в квадрат

Этот проект реализует алгоритм быстрого возведения чисел в квадрат в произвольных системах счисления и сравнивает его производительность со стандартным методом умножения.

## Описание

Код предоставляет две основные функции:
1. `fast_square(x, b)` - собственный алгоритм для возведения в квадрат числа, представленного в виде списка цифр в системе счисления с основанием `b`
2. `to_decimal(number, base)` - вспомогательная функция для преобразования числа из произвольной системы счисления обратно в десятичную

## Идея алгоритма

Алгоритм `fast_square` реализует оптимизированную версию возведения в квадрат, основанную на следующих ключевых идеях:

1. **Разложение квадрата**:
   - Используется формула (a+b)² = a² + 2ab + b²
   - Алгоритм эффективно вычисляет все частичные произведения

2. **Поразрядная обработка**:
   - Число обрабатывается по цифрам, начиная с младших разрядов
   - Для каждой цифры вычисляется ее вклад в конечный результат

3. **Учет переносов**:
   - Корректно обрабатываются переносы между разрядами
   - Все вычисления выполняются в исходной системе счисления

## Оценка сложности

Вычислительная сложность алгоритма:
- **Временная сложность**: O(n²), где n - количество цифр в числе
  - Лучше чем стандартное умножение O(n²) за счет оптимизаций
  - Для каждой из n цифр выполняется O(n) операций

- **Пространственная сложность**: O(n)
  - Требуется дополнительный массив длиной 2n+1 для хранения промежуточных результатов

## Как использовать

1. Запустите программу: `python fast_square.py`
2. Введите основание системы счисления (целое число ≥ 2)
3. Введите число для возведения в квадрат (целое число ≥ 0)
4. Программа выведет:
   - Результат возведения в квадрат с помощью `fast_square`
   - Время выполнения `fast_square`
   - Результат стандартного умножения
   - Время выполнения стандартного умножения

## Пример работы

```
Введите основание системы счисления (не менее 2): 10
Введите число: 123
Результат возведения в квадрат (fast_square): 15129
0.000123 секунд
Результат возведения в квадрат (стандартный): 15129
0.000045 секунд
```

## Особенности реализации

- Алгоритм `fast_square` работает непосредственно с цифрами числа в заданной системе счисления
- Реализована проверка корректности ввода (основание ≥ 2, число ≥ 0)
- Выполняется сравнение времени работы обоих методов
- Поддерживаются произвольные системы счисления (от основания 2 и выше)

## Требования

- Python 3.x