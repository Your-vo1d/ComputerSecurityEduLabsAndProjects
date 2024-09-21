#include <stdio.h>

// Задача: удалить из числа все нечетные цифры
int main() {
    int inputNumber, result = 0, digitPlace = 1, digit; // inputNumber - введенное число, result - результат, digitPlace - множитель для разряда, digit - текущая цифра
    int allDigitsOdd = 1; // Флаг - все цифры нечетные

    printf("Enter number: ");
    // Проверка корректности ввода
    if (scanf("%d", &inputNumber) != 1) {
        printf("Input error \n");
    } else {
        // Обработка числа, пока оно не станет нулем
        while (inputNumber > 0) {
            digit = inputNumber % 10; // Получаем последнюю цифру
            if (digit % 2 == 0) { // Если цифра четная
                allDigitsOdd = 0; // Устанавливаем флаг, что не все цифры нечетные
                result += digit * digitPlace; // Добавляем цифру к результату
                digitPlace *= 10; // Переходим к следующему разряду
            }
            inputNumber /= 10; // Удаляем последнюю цифру из числа
        }
        // Если все цифры были нечетными и число не равно нулю
        if (allDigitsOdd) {
            printf("All digits are odd \n");
        } else {
            printf("Result = %d \n", result);
        }
    }
    return 0;
}
