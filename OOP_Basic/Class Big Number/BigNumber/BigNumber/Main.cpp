#include "BigNumber.h"



int main()
{
    // ������������ ������ input_hex
    std::cout << "Enter the first hexadecimal number: "; // ������� ������ ����������������� �����
    Big_Number bn1;
    bn1.input_hex();
    bn1.output_hex();
    std::cout << "Enter the second hexadecimal number: "; // ������� ������ ����������������� �����
    Big_Number bn2;
    bn2.input_hex();
    bn2.output_hex();
    // ������������ ���������� ���������
    if (bn1 == bn2) {
        std::cout << "The first number is equal to the second number.\n"; // ������ ����� ����� ������� �����
    }
    else {
        std::cout << "The first number is not equal to the second number.\n"; // ������ ����� �� ����� ������� �����
    }

    if (bn1 != bn2) {
        std::cout << "The first number is not equal to the second number.\n"; // ������ ����� �� ����� ������� �����
    }
    else {
        std::cout << "The first number is equal to the second number.\n"; // ������ ����� ����� ������� �����
    }

    if (bn1 < bn2) {
        std::cout << "The first number is less than the second number.\n"; // ������ ����� ������ ������� �����
    }
    else {
        std::cout << "The first number is not less than the second number.\n"; // ������ ����� �� ������ ������� �����
    }

    if (bn1 <= bn2) {
        std::cout << "The first number is less than or equal to the second number.\n"; // ������ ����� ������ ��� ����� ������� �����
    }
    else {
        std::cout << "The first number is greater than the second number.\n"; // ������ ����� ������ ������� �����
    }

    if (bn1 > bn2) {
        std::cout << "The first number is greater than the second number.\n"; // ������ ����� ������ ������� �����
    }
    else {
        std::cout << "The first number is not greater than the second number.\n"; // ������ ����� �� ������ ������� �����
    }

    if (bn1 >= bn2) {
        std::cout << "The first number is greater than or equal to the second number.\n"; // ������ ����� ������ ��� ����� ������� �����
    }
    else {
        std::cout << "The first number is less than the second number.\n"; // ������ ����� ������ ������� �����
    }

    Big_Number bn3 = bn1 + bn2;
    std::cout << "bn1 + bn2 = "; // ����� bn1 � bn2
    bn3.output_hex();

    // ������������ ��������� +=
    bn1 += bn2;
    std::cout << "Updated bn1 after adding bn2: "; // ����������� bn1 ����� ���������� bn2
    bn1.output_hex();

    Big_Number bn4 = bn3 - bn2;
    std::cout << "bn_3 - bn_2 = "; // ����� bn1 � bn2
    bn4.output_hex();

    bn1 -= bn2;
    std::cout << "bn_1 -= bn_2 "; // ����� bn1 � bn2
    bn1.output_hex();

    if (bn1 == bn4) {
        std::cout << "The first number is equal to the second number.\n"; // ������ ����� ����� ������� �����
    }
    else {
        std::cout << "The first number is not equal to the second number.\n"; // ������ ����� �� ����� ������� �����
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
