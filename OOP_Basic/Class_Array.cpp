#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <limits>

class Array { 
    int *ptr, len;

    public:
        
        //Конструктор 1: по параметрам, длине и диапазону значений
        Array(int len = 1, int t = 1, int min = 0, int max = 20); 
        
        // конструктор 2: по массиву
        Array(int *, int); 
        
        //Конструктор копирования
        Array(Array &);
        
        //Деструктор
        ~Array();
        
        //Перегрузка оператора "=" (присвоение)
        Array& operator = (Array &);

        //Перегрузка оператор [] (работа с элементами массива по индексу)
        int &operator [](int);

        //Проверка на упорядоченность по неубыванию
        bool Test();

        // равенство элементов массивов (но не порядка)
        bool operator == (Array); 
        
        //Сортировка Шелла
        void Shell_sort();
        void Heapsort();
        void Hoar_sort();
        void Bit_sort();

        //Перегрузка операции потокового ввода
        friend std::istream & operator >> (std::istream &, Array &);
        //Перегрузка операции потокового вывода
        friend std::ostream & operator << (std::ostream &, Array &);
};

//Конструктор 1 по параметрам, длине и диапазону значений
Array::Array(int len, int t, int min, int max) {
    if (len > 0) {
        if (min > max) {
            min = min ^ max;
            max = min ^ max;
            min = min ^ max; 
        }
        t %= 4;
        ptr = new int[len];
        this->len = len;
        srand(time(NULL));
        //Если параметр = 1, то будет псевдослучайная последовательность
        if (t == 1) {
            for (int i = 0; i < len; i++) {
                ptr[i] =  min + rand() % (max - min + 1);
            }
        }
        //Если параметр = 2, то будет последовательность формируется по неубыванию
        else if (t == 2) {
            for (int i = 0; i < len; i++) {
                if (i == 0)
                    ptr[i] = min + rand() % max;
                else {
                    ptr[i] = ptr[i - 1] + rand() % max;
                }
            }
        }
        //Если параметр = 3, то будет последовательность формируется по невозрастанию
        else if (t == 3) {
            for (int i = 0; i < len; i++) {
                if (i == 0)
                    ptr[i] = max - rand() % min;
                else {
                    ptr[i] = ptr[i - 1] - rand() % min;
                }
            }
        }
        //Если параметр другой, то последовательность состоит из 1 элемента (минимального элемента)
        else {
            std::cout << "Input error.\n";
            for (int i = 0; i < len; i++)
                ptr[i] = min;
        }
    }
}

//Конструтор 2 (по указателю и длине массива)
Array::Array(int *array, int len) {
    ptr = new int[len];
    this->len = len;
    for (int i = 0; i < len; i++)
        ptr[i] = array[i];
}

//Конструктор копирования 
Array::Array(Array &a) {
    len = a.len;
    ptr = new int[len];
    for (int i = 0; i < len; i++)
        ptr[i] = a[i];
}

//Деструктор
Array::~Array() {
    if (ptr) {
        delete [] ptr;
        ptr = NULL;
    }
}

//Перегрузка операции присвоение
Array &Array::operator=(Array &a) {
    if (this != &a) {
        delete [] ptr;
        len = a.len;
        ptr = new int[len];
        for(int i = 0; i < len; i++)
            ptr[i] = a[i];
    }
    return *this;
}

//Перегрузка операции взятие элемента по индексе
int &Array::operator[](int i) {
    if (i < 0 || i >= len) {
        std::cout << "\n Индекс за пределами массива";
        return ptr[0];
    }
    return ptr[i];
}

//Проверка на упорядоченность по неубыванию
bool Array:: Test() {
    for (int i = 0; i < len - 1; i++) {
        if (ptr[i] <= ptr[i + 1])
            return false;
    }
    return true;
}

// Перегрузка для операции '=='
bool Array::operator==(Array a) {
    if (len != a.len) {
        return false;
    }

    //Проверка с перемещением с конца на найденое место
    bool flag = false;

    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            if (a[i] == this->ptr[j]) { 
                a[i] = a[len - 1];
                a.len --;
            }
        }
    }

    if (a.len == 1) {
        return true;
    }
    return false;

    // Создаем копии обоих массивов, чтобы не менять оригинальные данные
    Array arr1(*this);
    Array arr2(a);

    // Сортируем оба массива с использованием std::sort
    std::sort(arr1.ptr, arr1.ptr + len);
    std::sort(arr2.ptr, arr2.ptr + len);

    bool found = false;

    for (int i = 0; i < len; i++) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }
}

std::istream& operator>>(std::istream& in, Array& a) {
    size_t len;

    std::cout << "Enter the size of the array: ";
    in >> len;

    while (in.fail() || len <= 0) {
        std::cerr << "Invalid input for array size. Please enter a positive integer: ";
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        in >> len;
    }

    a.len = len;

    for (size_t i = 0; i < len; ++i) {
        std::cout << "Enter element " << i << ": ";
        in >> a[i];

        while (in.fail()) {
            std::cerr << "Invalid input for element " << i << ". Please enter a valid value: ";
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            in >> a[i];
        }
    }

    return in;
}

std::ostream & operator << (std::ostream &out, Array &a) {
    for (int i = 0; i < a.len; i++)
        std::cout << a.ptr[i];
    std::cout << std::endl;
}


void main() {
    int len1;
    std::cout << "Enter a size: ";
    std::cin >> len1;
    //Создание массива с параметром - длиной массива
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
