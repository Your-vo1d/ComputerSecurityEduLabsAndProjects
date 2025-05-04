# Лабораторная работа: Операции со строками в языке C

## Цель работы
1. Изучить основные операции со строками в языке C
2. Реализовать функции для работы с подстроками:
   - Поиск подстроки
   - Замена текста
   - Удаление символов

## Описание программ

### 1. copying_substring.c
**Функциональность**: Замена текста на подстроку

```c
bool ChangeText(char *someText, char *someStr) {
    // Заменяет содержимое someText на someStr
    // Возвращает true при успешном выполнении
}
```

### 2. remove_characters.c
**Функциональность**: Удаление указанных символов из строки

```c
int DeleteSymbols(char *text, char *symbolsToRemove) {
    // Удаляет из text все символы, присутствующие в symbolsToRemove
    // Возвращает количество удаленных символов
}
```

### 3. search_change_remove_from_string.c
**Функциональность**: Комплексная работа со строками

```c
// Поиск подстроки
int FindSubString(char *someText, char *someStr) {
    // Возвращает позицию первого вхождения подстроки или -1 если не найдено
}

// Замена текста
void ChangeText(char *someText, char *someStr) {
    // Заменяет someText на someStr
}

// Удаление символов
void DeleteStr(char *someText, char *someStr) {
    // Удаляет из someText все символы из someStr
}
```

## Инструкция по использованию

### Компиляция
```bash
gcc copying_substring.c -o copy
gcc remove_characters.c -o remove
gcc search_change_remove_from_string.c -o string_ops
```

### Запуск
1. Для замены текста:
```bash
./copy
Введите текст: Hello World
Введите подстроку: Goodbye
```

2. Для удаления символов:
```bash
./remove
Введите текст: abcdefg
Введите символы для удаления: bdf
```

3. Для комплексных операций:
```bash
./string_ops
Введите текст: Programming is fun
Введите подстроку: fun
```

## Примеры работы

### copying_substring.c
```
Ввод:
Текст: Hello
Подстрока: World

Вывод:
Copying was successful. Result: World
```

### remove_characters.c
```
Ввод:
Текст: abcdefghijk
Символы для удаления: aeiou

Вывод:
bcdfghjk
```

### search_change_remove_from_string.c
```
Ввод:
Текст: The quick brown fox
Подстрока: quick

Вывод:
Fragments position is = 4
```

## Особенности реализации
1. Использование указателей для работы со строками
2. Эффективные алгоритмы поиска и замены
3. Проверка входных данных
4. Обработка граничных случаев

## Ограничения
1. Максимальная длина строки - 100 символов
2. Нет поддержки Unicode
3. Ограниченная обработка ошибок ввода
