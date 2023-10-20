#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <limits>

class Array
{
    int *ptr, len;

public:
    // Конструктор 1: по параметрам, длине и диапазону значений
    Array(size_t len = 1, int order = 1, int min = 0, int max = 20);

    // конструктор 2: по массиву
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
bool Array::operator==(Array a)
{
    if (len != a.len)
    {
        return false;
    }

    // Проверка с перемещением с конца на найденое место
}

std::istream &operator>>(std::istream &in, Array &a)
{
    size_t len;

    std::cout << "Enter the size of the array: ";
    in >> len;

    while (in.fail() || len <= 0)
    {
        std::cerr << "Invalid input for array size. Please enter a positive integer: ";
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        in >> len;
    }

    a.len = len;

    for (size_t i = 0; i < len; ++i)
    {
        std::cout << "Enter element " << i << ": ";
        in >> a[i];

        while (in.fail())
        {
            std::cerr << "Invalid input for element " << i << ". Please enter a valid value: ";
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            in >> a[i];
        }
    }

    return in;
}

std::ostream &operator<<(std::ostream &out, Array &a)
{
    for (int i = 0; i < a.len; i++)
        std::cout << a.ptr[i];
    std::cout << std::endl;
}

void main()
{
    int len1;
    std::cout << "Enter a size: ";
    std::cin >> len1;
    // Создание массива с параметром - длиной массива
    Array array1[len1];
    std::cout << array1 << std::endl;

    int parametr_t;
    std::cout << "Enter a t parametr: ";
    std::cin >> parametr_t;
    Array array2[len1, 1];
    Array array3[len1 + 4, 2];
    Array array4[len1 + 9, 3];
    Array array5[len1 + 9, 3, 0];
    Array array6[len1 + 9, 3, 0, 100];
    Array array7[len1 + 11, 1, -100, 100];

    Array array9[len1 + 13, 3, 150, 300];
}
