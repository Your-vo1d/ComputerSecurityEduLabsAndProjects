#include "Big_Number.h"
#include <vector>

int main() {
    Big_Number number_1; // Инициализация переменной для хранения числа
    number_1.cin_10(); // Ввод числа
    Big_Number result = number_1.p_minus_1_method_polarda_factorize();
    if (result == Big_Number(0ULL)) {
        std::cout << "No Found" << std::endl;
    } 
    else {
        std::cout << "Result:";
        result.cout_10();
    }
    return 0;
}