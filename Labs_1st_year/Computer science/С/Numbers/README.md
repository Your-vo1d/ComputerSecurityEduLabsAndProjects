# Лабораторная работа: Операции с числами в языке C

## Цель работы
1. Изучить различные операции с числами в языке C
2. Реализовать алгоритмы для:
   - Приближенного вычисления логарифма с помощью ряда Тейлора
   - Реверсирования чисел
   - Удаления нечетных цифр из числа
   - Модификации чисел с использованием указателей

## Описание программ

### 1. Taylor_series_approximations/taylor_series_log.c
**Функциональность**: Приближенное вычисление натурального логарифма с использованием ряда Тейлора

```c
double result = 0;
for (termIndex = 1, term = 2 * x; fabs(term) > epsilon; termIndex++) {
    result += term;
    term *= (2 * termIndex - 1) * ((x * x) / (2 * termIndex + 1));
}
```

### 2. Reverse_number/reverse_number.c
**Функциональность**: Реверсирование цифр в числе с проверкой переполнения

```c
while (number != 0) {
    remainder = number % 10;
    reversedNumber = reversedNumber * 10 + remainder;
    if (reversedNumber < 0) {
        overflowCheck = 1;
        break;
    }
    number = number / 10;
}
```

### 3. Remove_odd_digits/remove_odd_digits.c
**Функциональность**: Удаление нечетных цифр из числа

```c
while (inputNumber > 0) {
    digit = inputNumber % 10;
    if (digit % 2 == 0) {
        result += digit * digitPlace;
        digitPlace *= 10;
    }
    inputNumber /= 10;
}
```

### 4. Add_del_2/add_del_2.c
**Функциональность**: Модификация трех чисел с использованием указателей

```c
if (*ptrMin > num_3) {
    *ptrMin -= 2 * (*ptrMax == *ptrMin);
    ptrMin = &num_3;
}
else if (*ptrMin == num_3) {
    if (*ptrMax == num_3) {
        printf("equal");
    }
    else {
        *ptrMin += 2 * (*ptrMax != *ptrMin);
        ptrMin = &num_3;
    }
}
```

## Инструкция по использованию

### Компиляция
```bash
gcc taylor_series_log.c -o taylor -lm
gcc reverse_number.c -o reverse
gcc remove_odd_digits.c -o remove_odd
gcc add_del_2.c -o modify
```

### Запуск
1. Приближенное вычисление логарифма:
```bash
./taylor
Введите x и epsilon: 0.5 0.0001
```

2. Реверсирование числа:
```bash
./reverse
Введите число: 12345
```

3. Удаление нечетных цифр:
```bash
./remove_odd
Введите число: 123456
```

4. Модификация чисел:
```bash
./modify
Введите три числа: 5 10 15
```

## Примеры работы

### Приближенное вычисление логарифма
```
Ввод: 0.5 0.0001
Вывод:
Result from math.h = 1.098612
Result from Taylor = 1.098041
Accuracy = 0.000571
```

### Реверсирование числа
```
Ввод: 12345
Вывод: Result: 54321
```

### Удаление нечетных цифр
```
Ввод: 123456
Вывод: Result = 246
```

### Модификация чисел
```
Ввод: 5 10 15
Вывод: Result: 3 12 13
```

## Особенности реализации
1. Проверка входных данных на корректность
2. Обработка граничных случаев (переполнение, все нечетные цифры)
3. Использование указателей для модификации чисел
4. Сравнение с математической библиотекой для проверки точности

## Ограничения
1. Ограниченная точность при вычислении ряда Тейлора
2. Максимальное значение для реверсирования - INT_MAX
3. Не поддерживаются отрицательные числа в remove_odd_digits.c