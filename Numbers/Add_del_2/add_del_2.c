#include <stdio.h>

int main()
{
    int num_1 = 0;
    int num_2 = 0;
    int num_3 = 0;

    // Запрос ввода трех чисел от пользователя
    printf("Enter numbers: ");
    scanf("%d%d%d", &num_1, &num_2, &num_3);

    int *ptrMax = NULL;
    int *ptrMin = NULL;
    
    // Определение начальных значений ptrMax и ptrMin
    if (num_1 > num_2) {
        ptrMax = &num_1;
        ptrMin = &num_2;
    } else {
        ptrMax = &num_2;
        ptrMin = &num_1;
    }

    // Блок логики для сравнения и изменения значений num_1, num_2, num_3
    {
        // Если num_3 меньше текущего минимального значения
        if (*ptrMin > num_3) {
            *ptrMin -= 2 * (*ptrMax == *ptrMin);
            ptrMin = &num_3;
        }
        // Если num_3 равно текущему минимальному значению
        else if (*ptrMin == num_3) {
            if (*ptrMax == num_3) {
                printf("equal");
            } else {
                *ptrMin += 2 * (*ptrMax != *ptrMin);
                printf("%d %d  ", *ptrMin, *ptrMax);
                ptrMin = &num_3;
            }
        }
        // Если num_3 больше или равно текущему максимальному значению
        else if (*ptrMax <= num_3) {
            if (*ptrMax < num_3) {
                *ptrMax += 2 * (*ptrMax == *ptrMin);
            } else {
                *ptrMax -= 2 * (*ptrMax != *ptrMin);
            }
            ptrMax = &num_3;
        }

        // Финальные изменения значений ptrMax и ptrMin
        *ptrMax -= 2;
        *ptrMin += 2;

        // Вывод результатов
        printf("Result: %d %d %d", num_1, num_2, num_3);
        return 0;
    }
}
