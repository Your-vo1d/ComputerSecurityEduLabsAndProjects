#include "Big_Number.h"
#include <iostream>


int main() {
    // Тестирование метода input_hex
    std::cout << "Enter the first hexadecimal number: "; // Введите первое шестнадцатеричное число
    Big_Number bn1;
    bn1.input_hex();

    std::cout << "Enter the second hexadecimal number: "; // Введите второе шестнадцатеричное число
    Big_Number bn2;
    bn2.input_hex();

    // Тестирование операторов сравнения
    if (bn1 == bn2) {
        std::cout << "The first number is equal to the second number.\n"; // Первое число равно второму числу
    } else {
        std::cout << "The first number is not equal to the second number.\n"; // Первое число не равно второму числу
    }

    if (bn1 != bn2) {
        std::cout << "The first number is not equal to the second number.\n"; // Первое число не равно второму числу
    } else {
        std::cout << "The first number is equal to the second number.\n"; // Первое число равно второму числу
    }

    if (bn1 < bn2) {
        std::cout << "The first number is less than the second number.\n"; // Первое число меньше второго числа
    } else {
        std::cout << "The first number is not less than the second number.\n"; // Первое число не меньше второго числа
    }

    if (bn1 <= bn2) {
        std::cout << "The first number is less than or equal to the second number.\n"; // Первое число меньше или равно второму числу
    } else {
        std::cout << "The first number is greater than the second number.\n"; // Первое число больше второго числа
    }

    if (bn1 > bn2) {
        std::cout << "The first number is greater than the second number.\n"; // Первое число больше второго числа
    } else {
        std::cout << "The first number is not greater than the second number.\n"; // Первое число не больше второго числа
    }

    if (bn1 >= bn2) {
        std::cout << "The first number is greater than or equal to the second number.\n"; // Первое число больше или равно второму числу
    } else {
        std::cout << "The first number is less than the second number.\n"; // Первое число меньше второго числа
    }

    return 0;
}