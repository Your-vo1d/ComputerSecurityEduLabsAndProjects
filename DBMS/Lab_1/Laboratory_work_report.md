<h1 style="text-align: centrt;"> Отчет по лабораторной работе №1 по дисциплине "Система управления базами данных" </h1>
<h1 style="text-align: centrt;"> "Запрос с условием" </h1>

__Задание №1__
Найти все записи, у которых FIRST_NAME имеет в середине букву ‘e’ и заканчивается на "n".
```postgresql
SELECT
    *
From
    employees
WHERE
    first_name LIKE '%e%n'
```
