#include <iostream>
#include <limits>

// Класс Маршрут
class route {
private:
    int* citySequence, count; // citySequence – массив, представляющий маршрут; count – количество городов

public:
    route(int num = 0); // создает начальную перестановку по возрастанию
    route(const route&); // конструктор копирования
    route& operator=(const route&); // операция присваивания
    ~route(); // деструктор
    int route_price(int**); // вычисляет стоимость маршрута по матрице стоимости
    bool next_route(); // вычисляет следующий маршрут, используя алгоритм Дейкстры
    friend std::ostream& operator<<(std::ostream&, const route&); // вывод маршрута
};

// создает начальную перестановку по возрастанию
route::route(int num) {
    count = num;
    citySequence = new int[num];
    for (int i = 0; i < count; i++) {
        citySequence[i] = i;
    }
}

// конструктор копирования
route::route(const route& r) {
    count = r.count;
    citySequence = new int[count];
    for (int i = 0; i < count; i++) {
        citySequence[i] = r.citySequence[i];
    }
}

// операция присваивания
route& route::operator=(const route& r) {
    if (this != &r) { // Проверка на самоприсваивание
        delete[] citySequence; // Освобождаем память у текущего объекта
        count = r.count;
        citySequence = new int[count];
        for (int i = 0; i < count; i++) {
            citySequence[i] = r.citySequence[i];
        }
    }
    return *this;
}

// деструктор
route::~route() {
    delete[] citySequence;
}

// вычисляет стоимость маршрута по матрице стоимости
int route::route_price(int** matrix) {
    int price = 0;
    for (int i = 0; i < count - 1; i++) {
        price += matrix[citySequence[i]][citySequence[i + 1]];
    }
    price += matrix[citySequence[count - 1]][citySequence[0]];

    return price;
}

// вычисляет следующий маршрут, используя алгоритм Дейкстры
bool route::next_route() {
    int i = count - 2;
    while (i >= 0 && citySequence[i] > citySequence[i + 1]) {
        i--;
    }
    if (i < 1) {
        return false; // Все маршруты перебраны
    }

    int j = count - 1;
    while (citySequence[i] > citySequence[j]) {
        j--;
    }

    // Меняем местами элементы
    int temp = citySequence[i];
    citySequence[i] = citySequence[j];
    citySequence[j] = temp;

    // Переворачиваем оставшуюся часть
    int left = i + 1;
    int right = count - 1;
    while (left < right) {
        temp = citySequence[left];
        citySequence[left] = citySequence[right];
        citySequence[right] = temp;
        left++;
        right--;
    }

    return true;
}

// вывод маршрута
std::ostream& operator<<(std::ostream& out, const route& r) {
    for (int i = 0; i < r.count; i++) {
        out << r.citySequence[i] << " ";
    }
    return out;
}

int main() {
    int numCities;

    // Запрашиваем у пользователя количество городов и проверяем корректность ввода
    while (true) {
        std::cout << "Введите количество городов: ";
        if (!(std::cin >> numCities) || numCities < 2) {
            std::cout << "Некорректный ввод. Количество городов должно быть целым числом и больше или равно 2.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            break;
        }
    }

    // Создаем матрицу стоимости и запрашиваем у пользователя элементы матрицы
    int** costMatrix = new int*[numCities];
    for (int i = 0; i < numCities; i++) {
        costMatrix[i] = new int[numCities];
        for (int j = 0; j < numCities; j++) {
            while (true) {
                std::cout << "Введите стоимость пути от города " << i << " к городу " << j << ": ";
                if (!(std::cin >> costMatrix[i][j]) || costMatrix[i][j] < 0) {
                    std::cout << "Некорректный ввод. Стоимость должна быть неотрицательным целым числом.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    break;
                }
            }
        }
    }

    route r(numCities); // Создаем объект класса route с начальной перестановкой
    int minCost = r.route_price(costMatrix); // Изначально считаем минимальной стоимостью текущую перестановку
    route minRoute = r; // Изначально считаем минимальной текущую перестановку

    do {
        int currentCost = r.route_price(costMatrix); // Считаем стоимость текущей перестановки
        std::cout << "Путь: " << r << " - Cost: " << currentCost << std::endl;
        if (currentCost < minCost) { // Если она меньше минимальной, обновляем минимальную стоимость и маршрут
            minCost = currentCost;
            minRoute = r;
        }
    } while (r.next_route());

    // Выводим минимальный путь и его стоимость
    std::cout << "Минимальный путь: " << minRoute << " - Cost: " << minCost << std::endl;

    // Освобождаем выделенную память
    for (int i = 0; i < numCities; i++) {
        delete[] costMatrix[i];
    }
    delete[] costMatrix;

    return 0;
}
