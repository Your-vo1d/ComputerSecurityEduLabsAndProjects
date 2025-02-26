#include <stdio.h>
#include <math.h>

int main() {
    double epsilon; // Переменная для указания необходимой точности
    double x; 
    double result = 0; // Результат вычисления ряда Тейлора
    int termIndex;
    double term; // Текущий элемент ряда

    printf("Enter x and epsilon: ");

    // Проверка корректности ввода
    if ((scanf("%lf%lf", &x, &epsilon) != 2) || epsilon >= 1 || epsilon <= -1 || x >= 1 || x <= -1) {
        printf("Input error\n");
    } else {
        // Инициализация первого элемента ряда
        for (termIndex = 1, term = 2 * x; fabs(term) > epsilon; termIndex++) {
            result += term; // Добавление текущего элемента к результату
            // Вычисление следующего элемента ряда
            term *= (2 * termIndex - 1) * ((x * x) / (2 * termIndex + 1));
        }

        // Вывод результатов
        printf("Result from math.h = %lf\n", log((1 + x) / (1 - x)));
        printf("Result from Taylor = %lf\n", result);
        printf("Accuracy = %lf\n", log((1 + x) / (1 - x)) - result);
    }
    
    return 0;
}
