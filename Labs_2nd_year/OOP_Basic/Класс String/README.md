# Лабораторная работа: Реализация класса String с алгоритмом поиска Бойера-Мура

## Цель работы
1. Реализовать класс String для работы со строками
2. Реализовать алгоритм поиска подстроки (Бойера-Мура)
3. Изучить основные принципы ООП: инкапсуляцию, перегрузку операторов, конструкторы/деструкторы

## Описание класса String

### Основные поля класса
```cpp
char *str;  // Указатель на динамический массив символов
int len;    // Длина строки
```

### Конструкторы
| Конструктор | Описание |
|------------|----------|
| `String(int l = 0)` | Создает пустую строку заданной длины |
| `String(const char *)` | Создает строку из C-строки |
| `String(const String &)` | Конструктор копирования |

### Основные методы
| Метод | Описание |
|-------|----------|
| `getLen()` | Возвращает длину строки |
| `BM_search()` | Реализует алгоритм поиска Бойера-Мура |

### Перегруженные операторы
| Оператор | Описание |
|----------|----------|
| `=` | Присваивание строк |
| `[]` | Доступ к символам по индексу |
| `==` и `!=` | Сравнение строк |
| `+` и `+=` | Конкатенация строк |
| `<<` и `>>` | Ввод/вывод в поток |

## Алгоритм поиска Бойера-Мура

```cpp
int String::BM_search(const String &s) const {
    int tab[256];
    for (int i = 0; i < 256; i++) {
        tab[i] = s.len;
    }

    for (int i = 0; i < s.len - 1; i++) {
        tab[s[i]] = s.len - 1 - i;
    }

    int indexStr = s.len - 1;
    int indexS = s.len - 1;

    while (indexStr < len) {
        indexS = s.len - 1;
        int k = indexStr;

        while (indexS >= 0 && str[k] == s[indexS]) {
            k--;
            indexS--;
        }

        if (indexS == -1) {
            return k + 1;
        } else {
            indexStr += tab[str[indexStr]];
        }
    }

    return -1;
}
```

## Примеры использования

### Создание строк
```cpp
String s1("Hello, World!");  // Создание из C-строки
String s2 = s1;              // Копирование
String s3;                   // Пустая строка
```

### Операции со строками
```cpp
s3 = s1 + " from C++";       // Конкатенация
s1 += "!!!";                 // Добавление в конец
char c = s1[0];              // Доступ по индексу
```

### Поиск подстроки
```cpp
int pos = s1.BM_search("World");
if (pos != -1) {
    cout << "Подстрока найдена на позиции: " << pos;
}
```

### Ввод/вывод
```cpp
String input;
cin >> input;                // Ввод строки
cout << input;               // Вывод строки
```

## Особенности реализации
1. Динамическое выделение памяти для строк
2. Проверка границ при доступе к символам
3. Поддержка операций конкатенации и сравнения
4. Реализация эффективного алгоритма поиска подстроки

## Ограничения
1. Работает только с ASCII-символами
2. Максимальная длина строки ограничена доступной памятью
3. Нет поддержки Unicode

## Инструкция по сборке
```bash
g++ string_bm.cpp -o string_bm
./string_bm
```