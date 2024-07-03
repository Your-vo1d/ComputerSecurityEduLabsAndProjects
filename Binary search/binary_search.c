#include <stdio.h>

int main() {
    int len, flag = 1, i, temp, count = 0, desired_element, result = -1;

    printf("Enter the number of digits in the array: "); // Ввод количества элементов массива
    if (scanf("%d", &len) != 1 || len <= 0) { // Проверка ввода
        printf("Error when entering the number of array elements\n");
        return 1;
    }

    int array[len]; // Объявляем массив
    int position = len - 1; // Позиция до которой делать перебор

    printf("Enter elements of array: "); // Ввод элементов массива
    for (i = 0; i < len; i++) {
        if (scanf("%d", &array[i]) != 1) { // Проверка ввода элементов массива
            printf("Element input error\n");
            return 1;
        }
    }

    // Сортировка пузырьком
    while (flag && len > 0) {
        flag = 0;
        for (i = 0; i < position; i++) {
            if (array[i] > array[i + 1]) {
                temp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = temp;
                flag = 1;
            }
        }
        count++;
        position = i;
    }

    // Вывод отсортированного массива
    for (i = 0; i < len; i++) {
        printf("Arr[%d] = %d\n", i, array[i]);
    }

    // Ввод искомого элемента
    printf("Enter the desired item: ");
    if (scanf("%d", &desired_element) != 1) {
        printf("Error entering the desired element\n");
        return 1;
    }

    // Бинарный поиск
    int left = 0;
    int right = len - 1;
    while (right >= left && result < 0) {
        int middle = (left + right) / 2;
        if (desired_element == array[middle]) {
            result = middle;
        } else if (array[middle] < desired_element) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }

    // Вывод результата поиска
    if (result >= 0) {
        printf("Result: %d\n", result);
    } else {
        printf("Element not found\n");
    }

    return 0;
}
