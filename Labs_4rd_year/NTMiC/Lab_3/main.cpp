#include "Big_Number.h"
#include <vector>

int main() {
    Big_Number number_1; // Инициализация переменной для хранения числа
    number_1.cin_10(); // Ввод числа
    std::vector<Big_Number> result = number_1.method_Ferma();
    if (result.empty()){
        std::cout << "Prime" << std::endl;
    }
    else {
        std::cout << "A: ";
        result[0].cout_10();
        std::cout << "B: ";
        result[1].cout_10();
    }
    return 0;
}