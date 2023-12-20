#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <fstream>
#include <cstring>

using namespace std;

class BoolV
{
    unsigned int* v; 
    int count_bit, count_elemet; // count_bit – количество бит вектора, count_elemet – количество элементов массива v
public:
    // Конструктор: формирует булев вектор из count_bit бит; все биты со значениями 0
    BoolV(int count_bit_n = 1);

    // Конструктор: формирует булев вектор по строке из нулей и единиц
    BoolV(const char* strV);

    // Конструктор копирования
    BoolV(const BoolV& Vec);

    // Деструктор: освобождает память, занятую вектором
    ~BoolV() { if (v) delete[] v; v = NULL; };

    // Устанавливает указанный бит в 1
    void Set1(int bit);

    // Устанавливает указанный бит в 0
    void Set0(int bit);

    // Перегрузка оператора [] для доступа к битам вектора
    int operator[](int bit);

    // Перегрузка оператора присваивания
    BoolV operator = (const BoolV& Vec);

    // Перегрузка оператора сравнения на равенство
    bool operator == (const BoolV& Vec);

    // Перегрузка оператора "или" для булевого вектора
    BoolV operator | (const BoolV& Vec);

    // Перегрузка оператора "и" для булевого вектора
    BoolV operator & (const BoolV& Vec);

    // Перегрузка оператора "не" для булевого вектора
    BoolV operator ~ ();

    // Перегрузка оператора вывода в поток
    friend ostream& operator << (ostream& r, const BoolV& Vec);

    // Перегрузка оператора ввода из потока
    friend istream& operator >> (istream& r, BoolV& Vec);
};


BoolV::BoolV(int count_bit_n)
{
    // Проверка на минимальное количество бит, устанавливаем 1, если меньше
    if (count_bit_n < 1) 
        count_bit_n = 1;
    else 
        count_bit = count_bit_n;

    // Вычисление количества элементов для хранения битов в блоках по 32 бита
    count_elemet = (count_bit_n - 1) / 32 + 1;

    // Выделение памяти под массив беззнаковых целых чисел
    v = new unsigned int[count_elemet];

    // Инициализация каждого элемента массива значением 0
    for (int i = 0; i < count_elemet; i++) 
        v[i] = 0;
}


BoolV::BoolV(const char* strV)
{
    if (strV)
    {
        int i = 0, count = 0, j = 0, k = 0, sizeSTR = strlen(strV);
        unsigned int mask = 1;
        char* strVec = new char[sizeSTR + 1];

        // Фильтрация символов '0' и '1' из строки
        while (strV[i] != '\0')
        {
            if (strV[i] == '1' || strV[i] == '0')
            {
                strVec[count] = strV[i];
                count++;
            }
            i++;
        }
        strVec[count] = '\0';

        // Если строка пуста, устанавливается минимальное количество бит в 1
        if (count == 0) count_bit = 1;
        else count_bit = count;

        // Вычисление количества элементов для хранения битов в блоках по 32 бита
        count_elemet = (count_bit - 1) / 32 + 1;
        v = new unsigned int[count_elemet];

        k = count - 1;
        // Инициализация булевого вектора значениями из строки
        for (i = 0; i < count_elemet; i++)
        {
            v[i] = 0;
            j = 0;
            mask = 1 << 31;
            while (j < 32 && k >= 0)
            {
                if (strVec[k] == '1')
                {
                    v[i] = v[i] | mask;
                }
                mask = mask >> 1;
                k--; j++;
            }
        }

        // Освобождение памяти, выделенной для временного массива
        delete[] strVec;
    }
}


BoolV::BoolV(const BoolV& Vec)
{
    count_bit = Vec.count_bit;
    count_elemet = Vec.count_elemet;
    
    // Выделение памяти под новый массив
    v = new unsigned int[count_elemet];

    // Копирование данных из существующего объекта
    for (int i = 0; i < count_elemet; i++)
    {
        v[i] = Vec.v[i];
    }
}

void BoolV::Set1(int bit)
{
    // Проверка корректности индекса бита
    if (bit <= count_bit && bit >= 0)
    {
        // Вычисление индекса блока (элемента массива v)
        size_t i = bit / 32;
       
        // Вычисление маски для установки бита в 1
        unsigned int mask = 1 << (31 - bit % 32);

        // Установка бита в 1
        v[i] = v[i] | mask;
    }
}

void BoolV::Set0(int bit)
{
    // Проверка корректности индекса бита
    if (bit <= count_bit && bit >= 0)
    {
        // Вычисление индекса элемента массива v
        int i = bit / 32;

        // Вычисление смещения бита внутри элемента массива
        int setbit = bit % 32;

        // Вычисление маски для установки бита в 0
        unsigned int mask = 1 << (31 - setbit);
        mask = ~mask;

        // Установка бита в 0 путем выполнения операции "и" с инвертированной маской
        v[i] = v[i] & mask;
    }
}

int BoolV::operator[](int bit)
{
    // Проверка корректности индекса бита
    if (bit > count_bit || bit < 0) 
        return -1;

    // Вычисление индекса элемента массива v, представляющего блок бита
    size_t mbit = bit / 32;

    // Вычисление относительного индекса бита внутри элемента массива
    size_t ncount_bit = bit % 32;

    // Вычисление маски для проверки значения бита
    unsigned int mask = 1 << (31 - ncount_bit);

    // Возвращение значения бита: 1, если бит установлен в 1, иначе 0
    return ((v[mbit] & mask) != 0) ? 1 : 0;
}

BoolV BoolV::operator = (const BoolV& Vec)
{
    // Проверка на самоприсваивание
    if (&Vec == this) 
        return *this;

    // Копирование данных из объекта Vec
    count_bit = Vec.count_bit;
    count_elemet = Vec.count_elemet;

    // Освобождение памяти, занятой текущим массивом v
    if (v) 
        delete[] v;
    v = NULL;

    // Выделение новой памяти и копирование данных
    v = new unsigned int[count_elemet];
    for (size_t i = 0; i < count_elemet; i++)
    {
        v[i] = Vec.v[i];
    }

    // Возвращение объекта для поддержки цепочечных операций присваивания
    return *this;
}

bool BoolV::operator == (const BoolV& Vec)
{
    // Проверка на равенство количества бит
    if (count_bit != Vec.count_bit) 
        return false;

    // Сравнение элементов массива v
    size_t i = 0;
    while (i < count_elemet - 1)
    {
        if (Vec.v[i] != v[i]) 
            return false;
        i++;
    }

    // Возвращение true, если все биты совпадают
    return true;
}


BoolV BoolV::operator | (const BoolV& Vec)
{
    // Проверка, что текущий объект содержит больше элементов
    if (count_elemet > Vec.count_elemet)
    {
        // Создание нового объекта-результата, инициализированного текущим объектом
        BoolV result(*this);

        // Выполнение побитового "или" для каждого элемента массива v
        int i = 0;
        while (i < Vec.count_elemet)
        {
            result.v[i] = result.v[i] | Vec.v[i];
            i++;
        }

        // Возвращение результата
        return result;
    }
    else if (count_elemet > 0)
    {
        // Создание нового объекта-результата, инициализированного объектом Vec
        BoolV result(Vec);

        // Выполнение побитового "или" для каждого элемента массива v
        int i = 0;
        while (i < count_elemet)
        {
            result.v[i] = result.v[i] | v[i];
            i++;
        }

        // Возвращение результата
        return result;
    }
    
    // Возвращение пустого объекта BoolV в случае, если оба объекта пусты
    return BoolV();
}


BoolV BoolV::operator & (const BoolV& Vec)
{
    // Проверка, что текущий объект содержит больше элементов
    if (count_elemet > Vec.count_elemet)
    {
        // Создание нового объекта-результата, инициализированного текущим объектом
        BoolV result(*this);

        // Выполнение побитового "и" для каждого элемента массива v
        int i = 0;
        while (i < Vec.count_elemet)
        {
            result.v[i] = result.v[i] & Vec.v[i];
            i++;
        }

        // Возвращение результата
        return result;
    }
    else if (count_elemet > 0)
    {
        // Создание нового объекта-результата, инициализированного объектом Vec
        BoolV result(Vec);

        // Выполнение побитового "и" для каждого элемента массива v
        size_t i = 0;
        while (i < count_elemet)
        {
            result.v[i] = result.v[i] & v[i];
            i++;
        }

        // Возвращение результата
        return result;
    }

    // Возвращение пустого объекта BoolV в случае, если оба объекта пусты
    return BoolV();
}

BoolV BoolV::operator ~ ()
{
    // Создание нового объекта-результата, инициализированного текущим объектом
    BoolV result(*this);

    // Выполнение побитового отрицания для каждого элемента массива v
    int i = 0;
    while (i < count_elemet)
    {
        result.v[i] = ~v[i];
        i++;
    }

    // Вычисление остаточных битов (tail), которые не являются частью полного блока по 32 бита
    int tail = count_elemet * 32 - count_bit;
    unsigned int mask;

    // Если есть остаточные биты, применяется маска для их обнуления
    if (tail)
    {
        mask = 1;
        for (i = 0; i < tail - 1; i++)
        {
            mask = (mask << 1) | 1;
        }

        mask = ~mask;
        result.v[count_elemet - 1] = result.v[count_elemet - 1] & mask;
    }

    // Возвращение результата
    return result;
}

ostream& operator << (ostream& r, const BoolV& Vec)
{
    size_t j = 0, cart = 0;
    unsigned int mask = 1;

    // Цикл по элементам массива v в обратном порядке
    for (int i = Vec.count_elemet - 1; i >= 0; i--)
    {
        j = 0;

        // Цикл по битам в элементе массива v
        while (j < 32)
        {
            mask = 1 << j;

            // Проверка значения бита и вывод "1" или "0" в поток
            if ((Vec.v[i] & mask) != 0) 
                r << "1";
            else 
                r << "0";

            j++;

            // Вывод пробела после каждых 8 бит (1 байта)
            cart++;
            if (cart == 8)
            {
                r << " ";
                cart = 0;
            }
        }
    }

    return r;
}


istream& operator >> (istream& r, BoolV& Vec)
{
    // Буфер для считывания строки из входного потока
    char buf[128];

    // Считывание строки из входного потока
    cin >> buf;

    // Создание временного объекта BoolV из считанной строки
    BoolV BUF(buf);

    // Присвоение значений временного объекта Vec
    Vec = BUF;

    // Возвращение потока ввода
    return r;
}


class BoolM
{
    BoolV* bm;  // Указатель на массив объектов класса BoolV
    int m, n;   // m – количество строк, n – количество столбцов
public:
    BoolM(int k = 1, int l = 1);  // Конструктор для формирования нулевой булевой матрицы
    BoolM(const BoolM& M);       // Конструктор копирования
    BoolV& operator[] (int l);    // Перегрузка оператора индексации
    BoolM operator = (const BoolM& M);  // Перегрузка оператора присваивания
    friend ostream& operator << (ostream& r, BoolM& M);  // Перегрузка оператора вывода в поток
};

BoolM::BoolM(int k, int l)
{
    if (k < 1) k = 1;
    if (l < 1) l = 1;

    m = l;
    n = k;
    bm = new BoolV[l];  // Выделение памяти под массив объектов BoolV

    // Инициализация каждого элемента массива bm объектом BoolV длины k
    BoolV vec(k);
    for (int i = 0; i < l; i++)
    {
        bm[i] = vec;
    }
}

BoolM::BoolM(const BoolM& M)
{
    m = M.m;
    n = M.n;
    bm = new BoolV[m];  // Выделение памяти под массив объектов BoolV

    // Копирование каждого элемента массива M.bm в массив bm
    for (int i = 0; i < m; i++)
    {
        bm[i] = M.bm[i];
    }
}

BoolV& BoolM::operator[] (int l)
{
    if (l < 0 || l >= m)
    {
        cout << "Index not in a row\n";
        exit(0);
    }

    return bm[l];  // Возврат ссылки на объект BoolV внутри матрицы
}

BoolM BoolM::operator = (const BoolM& M)
{
    if (&M == this) return *this;  // Проверка на самоприсваивание

    m = M.m;
    n = M.n;

    if (bm) delete[] bm;  // Освобождение памяти, если она уже была выделена
    bm = NULL;
    bm = new BoolV[m];  // Выделение памяти под массив объектов BoolV

    // Копирование каждого элемента массива M.bm в массив bm
    for (int i = 0; i < m; i++)
    {
        bm[i] = M.bm[i];
    }

    return *this;
}

ostream& operator << (ostream& r, BoolM& M)
{
    // Вывод матрицы на экран
    for (int i = 0; i < M.m; i++)
    {
        r << "( " << M.bm[i] << " )\n";
    }
    r << "\n";

    return r;
}

void topsort(BoolM& M, int m, int* a)
{
    if (m <= 0 && !a) exit(0);  // Проверка на корректность входных данных

    BoolV v0(m), v1(m), v2(m), null(m);

    int cart = 0;
    BoolM tmp(M);
    bool flag = true;
    while (cart != m && flag)
    {
        // Вычисление v1 - логическое ИЛИ всех строк матрицы tmp
        v1 = null;
        for (int i = 0; i < m; i++)
        {
            v1 = v1 | tmp[i];
        }

        // Вычисление v2 - логическое И от отрицания v1 и v0
        v2 = (~v1) & (~v0);
        cout << v1 <<endl;
        cout<<"dsd"<<endl;
        cout << v2 <<endl;
        flag = false;
        for (int i = 0; i < m; i++)
        {
            // Поиск первой единицы в v2
            if (v2[i] != 0)
            {
                flag = true;
                a[cart] = i + 1;  // Запись индекса вершины в массив a
                cart++;
                tmp[i] = null;  // Обнуление строки i в матрице tmp
            }
        }
        v0 = v0 | v2;  // Обновление v0
        if (!flag) puts("Matrix have cycle");
    }
}


int main()
{
	char sample[38] = "1 01101111 11100000 00000000 00000000";
	char sample2[18] = "00000000 01000000";
    
	BoolV a(33), b(sample), c(sample2);
	cout <<"Bool vector a: " << a << "\nBool vector b: " << b << "\nBool vector c: " << c << "\n";

	b.Set1(3);
	b.Set0(32);
	cout <<"The result of setting the bit to 3 and 32,  position: "<<endl << b << endl;

	a = b;
	cout << a << endl;

	if (a == b) cout << "Equal\n";
    	else cout << "Not equal\n";

	int bit = 30;
	cout << "The digit of bit = " << bit << " is equal " << b[bit] << "\n";

    cout << "Vector a:" << a << endl;
    cout << "Vector c:" << c << endl;

    BoolV cCopy(c);
	a = ~a;
	cout << a << "\n";

	c = cCopy | a;
	cout << c << "\n";

	c = cCopy & a;
	cout << c << "\n";

	cin >> a;
	cout << a << "\n";
    
    std::ifstream graphInFile("C:\\Users\\Void\\Documents\\Lab_2nd_years\\Lab_2nd_years\\OOP_Basic\\graph.txt");
    int vertexA = 0, vertexB = 0;
	int k = 0, size;
	if (graphInFile)
	{
    	graphInFile >> size;
    	BoolM Mx(size, size);
    	int* a = new int[size];

        while (graphInFile >> vertexA >> vertexB) {
            std::cout << "(" << vertexA << "," << vertexB << ")\n";
            Mx[vertexA - 1].Set1(vertexB - 1);
        }

    	graphInFile.close();

        cout << Mx[1] << endl;
    	cout << Mx;

    	topsort(Mx, size, a);

    	for (int i = 0; i < size; i++)
    	{
        	cout << a[i] << ", ";
    	}
	}
	else
    	puts("File is not open");

	return 0;
}

