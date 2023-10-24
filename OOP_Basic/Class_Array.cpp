#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <limits>

class Array
{
    int *ptr, len;

public:
    // Конструктор 1: по параметру порядка, длине и диапазону значений
    Array(size_t len = 1, int order = 1, int min = 0, int max = 20);

    // конструктор 2: по массиву и длине массива
    Array(int *, size_t);

    // Конструктор копирования
    Array(Array &);

    // Деструктор
    ~Array();

    // Перегрузка оператора "=" (присвоение)
    Array &operator=(Array &);

    // Перегрузка оператор [] (работа с элементами массива по индексу)
    int &operator[](int);

    // Проверка на упорядоченность по неубыванию
    bool Test();

    // равенство элементов массивов (но не порядка)
    bool operator==(Array);

    void Shell_sort(); // Сортировка Шелла
    void Heapsort();   // Пирамидальная сортировка
    void Hoar_sort();  // Быстрая сортировка
    void Bit_sort();   // Битовая сортировка

    void Sifting(); // Вспомогательная фукция "просеивание"

    // Перегрузка операции потокового ввода
    friend std::istream &operator>>(std::istream &, Array &);
    // Перегрузка операции потокового вывода
    friend std::ostream &operator<<(std::ostream &, Array &);
};

// Конструктор 1 по параметрам, длине и диапазону значений
Array::Array(size_t len, int order, int min, int max)
{
    if (len > 0) // Проверка корректности ввода длины массива
    {
        if (min > max) // Проверка на корректность ввода границ диапазона чисел
        {
            min = min ^ max;
            max = min ^ max;
            min = min ^ max;
        }
        // Выделение памяти под массив
        ptr = new int[len];
        if (ptr != nullptr) // Проверка выделения памяти
        {
            this->len = len;   // Инициализация длины массива
            srand(time(NULL)); // Инициализация генератора псевдослучайных чисел
            if (order == 1)    // Если параметр = 1, то будет псевдослучайная последовательность
            {
                for (int i = 0; i < len; i++)
                {
                    ptr[i] = min + rand() % (max - min + 1); // Генерация случайного числа в диапазоне
                }
            }
            else if (order == 2) // Если параметр = 2, то будет последовательность формируется по неубыванию
            {
                for (int i = 0; i < len; i++)
                {
                    if (i == 0)
                        ptr[i] = min + rand() % max; // Генерация 1-ого числа, которое >= минимального
                    else
                    {
                        ptr[i] = ptr[i - 1] + rand() % max; // Генерация случайного числа, которое >= предыдущего
                    }
                }
            }
            else if (order == 3) // Если параметр = 3, то будет последовательность формируется по невозрастанию
            {
                for (int i = 0; i < len; i++)
                {
                    if (i == 0)
                        ptr[i] = max - rand() % min; // Генерация 1-ого числа, которое <= максимального
                    else
                    {
                        ptr[i] = ptr[i - 1] - rand() % min; // Генерация случайного числа, которое <= предыдущего
                    }
                }
            }
            else // Если параметр другой, то последовательность состоит из 1 элемента (самого параметра)
            {
                std::cout << "Input error.\n"; // Сообщение об ошибке
                for (int i = 0; i < len; i++)
                    ptr[i] = order; // Заполнение массива 1 числом
            }
        }
    }
}

// Конструтор 2 (по указателю и длине массива)
Array::Array(int *array, size_t len)
{
    if (array != nullptr)
    {
        ptr = new int[len]; // Выделение памяти для массива
        if (ptr != nullptr) // Проверка выделение памяти
        {
            this->len = len; // Инициализация длины массива
            for (int i = 0; i < len; i++)
                ptr[i] = array[i]; // Копирование значений массива
        }
    }
}

// Конструктор копирования
Array::Array(Array &a)
{
    len = a.len;        // Инициализация длины массива
    ptr = new int[len]; // Выделение памяти для массива
    if (ptr != nullptr) // Проверка выделение памяти
    {
        for (int i = 0; i < len; i++)
            ptr[i] = a[i]; // Копирование значений массива
    }
}

// Деструктор
Array::~Array()
{
    if (ptr) // Проверка выделение памяти
    {
        delete[] ptr; // Освобождение памяти для массива
        ptr = NULL;   // Обнуляем указатель
    }
}

// Перегрузка операции присвоение
Array &Array::operator=(Array &a)
{
    if (this != &a) // Проверка самоприсвоения
    {
        delete[] ptr;       // Освобождение памяти для массива
        len = a.len;        // Изменение длины массива
        ptr = new int[len]; // Выделение памяти для массива
        if (ptr)
        {
            for (int i = 0; i < len; i++)
                ptr[i] = a[i]; // Копирование значений массива
        }
    }
    return *this; // Возвращение текущего объекта
}

// Перегрузка операции взятие элемента по индексе
int &Array::operator[](int i)
{
    if (i < 0 || i >= len) // Проверка корректности ввода индекса
    {
        std::cout << "\n Индекс за пределами массива" << std::endl; // Вывод сообщения об ошибке
        return ptr[0];                                              // Возвращение 1 элемента
    }
    return ptr[i]; // Возвращение i-ого элемента
}

// Проверка на упорядоченность по неубыванию
bool Array::Test()
{
    for (int i = 0; i < len - 1; i++) // Проход по всем элементам
    {
        if (ptr[i] > ptr[i + 1]) // Если текущий элемент меньше следуюшего, то массив неупорядочен
            return false;        // Массив неупорядочен
    }
    return true; // Массив упорядочен
}

// Перегрузка для операции '=='
bool Array::operator==(Array a) {
    if (len != a.len) {
        return false;
    }

    // Создаем копии обоих массивов, чтобы не менять оригинальные данные
    Array arr1(*this);
    Array arr2(a);

    // Сортируем оба массива с использованием std::sort
    std::sort(arr1.ptr, arr1.ptr + len);
    std::sort(arr2.ptr, arr2.ptr + len);

    for (int i = 0; i < len; i++) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }
    return true;
}

// Перегрузка операции потокового ввода
std::istream &operator>>(std::istream &in, Array &a)
{
    size_t len; // Переменная для хранения длины массива

    std::cout << "Enter the size of the array: "; // Сообщение-приглашение для ввода длины массива
    in >> len;                                    // Считывание длины массива из входного потока

    while (in.fail()) // Цикл проверки корректности ввода длины массива
    {
        std::cerr << "Invalid input for array size. Please enter a positive integer: "; // Вывод сообщения об ошибке в случае некорректного ввода
        in.clear();                                                                     // Сброс флагов ошибок во входном потоке
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');                   // Игнорирование оставшейся части строки до символа новой строки
        in >> len;                                                                      // Повторная попытка считывания длины массива
    }

    a.len = len; // Присвоение длины массива объекту класса Array

    for (size_t i = 0; i < len; ++i) // Цикл для ввода элементов массива
    {
        std::cout << "Enter element " << i << ": "; // Сообщение-приглашение для ввода элемента
        in >> a[i];                                 // Считывание значения элемента из входного потока

        while (in.fail()) // Цикл проверки корректности ввода элемента
        {
            std::cerr << "Invalid input for element " << i << ". Please enter a valid value: "; // Вывод сообщения об ошибке в случае некорректного ввода элемента
            in.clear();                                                                         // Сброс флагов ошибок во входном потоке
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');                       // Игнорирование оставшейся части строки до символа новой строки
            in >> a[i];                                                                         // Повторная попытка считывания элемента
        }
    }

    return in; // Возврат входного потока для поддержки цепочки операций ввода
}

// Перегрузка операции потокового выводаы
std::ostream &operator<<(std::ostream &out, Array &a)
{
    for (int i = 0; i < a.len; i++)
        out << a.ptr[i] << " "; // Вывод элементов массива
    out << std::endl;    // Перевод на новую строку
    return out;
}

//Сортировка Шелла
void Array::Shell_sort()
{
    for (int gap = len / 2; gap > 0; gap /= 2) // Начальный проход по массиву с уменьшением gap
    {
        for (int i = gap; i < len; i++) // Перебор элементов, начиная с gap
        {
            int temp = ptr[i]; // Сохранение текущего элемента во временной переменной temp
            int j; // Индекс для перебора элементов внутри подмассива
            for (j = i; j >= gap && ptr[j - gap] > temp; j -= gap) // Пока не найдено правильное место для temp
            {
                ptr[j] = ptr[j - gap]; // Перемещение элементов на gap позиций вправо
            }
            ptr[j] = temp; // Вставка temp на правильное место
        }
    }
}


int main() {
    // Тест конструктора 1 (по параметрам, длине и диапазону значений)
    Array array1(10, 1, 0, 100);  // Создаем массив длиной 10, случайного порядка, числа в диапазоне [0, 100]
    std::cout << "Array1: " << array1 << std::endl;

    // Тест конструктора 2 (по массиву и длине массива)
    int initialArray[5] = {5, 4, 3, 2, 1};
    Array array2(initialArray, 5);  // Создаем массив на основе массива initialArray
    std::cout << "Array2: " << array2 << std::endl;

    // Тест конструктора копирования
    Array array3(array2);  // Создаем копию массива array2
    std::cout << "Array3 (copy of Array2): " << array3 << std::endl;

    // Тест оператора присвоения
    Array array4 = array1;  // Присваиваем массив array1 массиву array4
    std::cout << "Array4 (assigned from Array1): " << array4 << std::endl;

    // Тест оператора []
    std::cout << "Element at index 2 of Array2: " << array2[2] << std::endl;

    // Тест метода Test (проверка на упорядоченность)
    if (array2.Test()) {
        std::cout << "Array2 is ordered in non-decreasing order." << std::endl;
    } else {
        std::cout << "Array2 is not ordered in non-decreasing order." << std::endl;
    }

    // Тест оператора ==
    if (array1 == array4) {
        std::cout << "Array1 is equal to Array4." << std::endl;
    } else {
        std::cout << "Array1 is not equal to Array4." << std::endl;
    }

    // Тест сортировки Шелла
    array2.Shell_sort();
    std::cout << "Array2 after Shell sort: " << array2 << std::endl;

    return 0;
}