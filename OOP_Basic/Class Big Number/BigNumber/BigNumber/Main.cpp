#include "BigNumber.h"



int main()
{
    // Тестирование метода input_hex
    std::cout << "Enter the first hexadecimal number: "; // Введите первое шестнадцатеричное число
    Big_Number bn1;
    bn1.input_hex();
    bn1.output_hex();
    std::cout << "Enter the second hexadecimal number: "; // Введите второе шестнадцатеричное число
    Big_Number bn2;
    bn2.input_hex();
    bn2.output_hex();
    // Тестирование операторов сравнения
    if (bn1 == bn2) {
        std::cout << "The first number is equal to the second number.\n"; // Первое число равно второму числу
    }
    else {
        std::cout << "The first number is not equal to the second number.\n"; // Первое число не равно второму числу
    }

    if (bn1 != bn2) {
        std::cout << "The first number is not equal to the second number.\n"; // Первое число не равно второму числу
    }
    else {
        std::cout << "The first number is equal to the second number.\n"; // Первое число равно второму числу
    }

    if (bn1 < bn2) {
        std::cout << "The first number is less than the second number.\n"; // Первое число меньше второго числа
    }
    else {
        std::cout << "The first number is not less than the second number.\n"; // Первое число не меньше второго числа
    }

    if (bn1 <= bn2) {
        std::cout << "The first number is less than or equal to the second number.\n"; // Первое число меньше или равно второму числу
    }
    else {
        std::cout << "The first number is greater than the second number.\n"; // Первое число больше второго числа
    }

    if (bn1 > bn2) {
        std::cout << "The first number is greater than the second number.\n"; // Первое число больше второго числа
    }
    else {
        std::cout << "The first number is not greater than the second number.\n"; // Первое число не больше второго числа
    }

    if (bn1 >= bn2) {
        std::cout << "The first number is greater than or equal to the second number.\n"; // Первое число больше или равно второму числу
    }
    else {
        std::cout << "The first number is less than the second number.\n"; // Первое число меньше второго числа
    }

    Big_Number bn3 = bn1 + bn2;
    std::cout << "bn1 + bn2 = "; // Сумма bn1 и bn2
    bn3.output_hex();

    // Тестирование оператора +=
    bn1 += bn2;
    std::cout << "Updated bn1 after adding bn2: "; // Обновленный bn1 после добавления bn2
    bn1.output_hex();

    Big_Number bn4 = bn3 - bn2;
    std::cout << "bn_3 - bn_2 = "; // Сумма bn1 и bn2
    bn4.output_hex();

    bn1 -= bn2;
    std::cout << "bn_1 -= bn_2 "; // Сумма bn1 и bn2
    bn1.output_hex();

    if (bn1 == bn4) {
        std::cout << "The first number is equal to the second number.\n"; // Первое число равно второму числу
    }
    else {
        std::cout << "The first number is not equal to the second number.\n"; // Первое число не равно второму числу
    }
    BASE m = 2;
    Big_Number resultMult = bn1 * 3;
    resultMult.output_hex();
    resultMult *= m;
    resultMult.output_hex();

    Big_Number res = bn1 * bn2;
    res.output_hex();

    return 0;

}
