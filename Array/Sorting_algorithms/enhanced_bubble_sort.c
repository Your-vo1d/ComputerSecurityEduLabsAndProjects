#include <stdio.h>

/* Задача: сортировка улучшенным пузырьком (с сохранением позиции последнего изменения) */
int main() {
    int arrayLength, flag = 1, i, temp, iterationCount = 0; // arrayLength - длина массива, flag - флаг сортировки, temp - временная переменная, iterationCount - количество итераций

    printf("Enter the number of elements in the array: ");
    if (scanf("%d", &arrayLength) != 1 || arrayLength <= 0) {
        printf("Error when entering the number of array elements\n");
    } else {
        int array[arrayLength]; // Объявление массива
        int lastSwapPosition = arrayLength - 1; // Позиция последнего обмена

        printf("Enter elements of array: ");
        // Ввод элементов массива
        for (int i = 0; i < arrayLength; i++) {
            if (scanf("%d", &array[i]) != 1) {
                arrayLength = 0; // Если ввод некорректен, обнуляем длину массива
            }
        }

        // Если ввод элементов прошел успешно
        if (arrayLength != 0) {
            // Основной цикл сортировки пузырьком
            while (flag && arrayLength > 0) {
                flag = 0;
                for (i = 0; i < lastSwapPosition; i++) {
                    if (array[i] > array[i + 1]) {
                        temp = array[i];
                        array[i] = array[i + 1];
                        array[i + 1] = temp;
                        flag = 1; // Устанавливаем флаг, что произошел обмен
                    }
                }
                iterationCount += 1; // Увеличиваем счетчик итераций
                lastSwapPosition = i; // Обновляем позицию последнего обмена
            }

            printf("Number of iterations: %d\n", iterationCount);
            printf("Sorted array: ");
            for (int i = 0; i < arrayLength; i++) {
                printf("%d ", array[i]);
            }
            printf("\n");
        } else {
            printf("Element input error\n");
        }
    }
    return 0;
}
