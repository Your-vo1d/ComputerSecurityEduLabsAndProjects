#include <stdlib.h>
#include <iomanip>
#include <cstring>
#include <iostream>
#include <random>
typedef unsigned char BASE;

typedef unsigned short int DBASE;

#define BASE_SIZE (sizeof(BASE) * 8)

using namespace std;

class Big_Number
{
	int len;	// Разряд
	int maxlen; // Выделенная память
	BASE *coef; // Массив цифр

public:
	Big_Number(int ml = 1, int pr = 0); // Конструктор по умолчанию/параметрам
	Big_Number(const Big_Number &bn);	// Конструктор копирования
	~Big_Number() // Деструктор
	{
		if (coef) //Проверка указателя
			delete[] coef;
		coef = NULL;
		len = 0;
		maxlen = 0;
	};
	; // Деструктор
	Big_Number operator=( const Big_Number &bn); // Перегрузка оператора присвоения

	void input_hex();  // Ввод большого числа в 16-ом виде
	void output_hex(); // Вывод большого числа в 16-ом виде

	bool operator==(const Big_Number &bn); // Перегрузка оператора ==
	bool operator!=(const Big_Number &bn); // Перегрузка оператора !=
	bool operator<(const Big_Number &bn);  // Перегрузка оператора <
	bool operator<=(const Big_Number &bn); // Перегрузка оператора <=
	bool operator>(const Big_Number &bn);  // Перегрузка оператора >
	bool operator>=(const Big_Number &bn); // Перегрузка оператора >=

	Big_Number operator+(const Big_Number &bn);
	Big_Number& operator+=(const Big_Number &bn);

	Big_Number operator-(const Big_Number &bn);
	Big_Number& operator-=(const Big_Number &bn);

    Big_Number operator*(const Big_Number &bn);
    Big_Number& operator*=(const Big_Number &bn);

    Big_Number operator*(BASE number);
    Big_Number& operator*=(BASE number);

};

// Конструктор по умолчанию/параметрам
Big_Number::Big_Number(int ml, int p)
{
    if (ml < 1)
    {
        exit(1);
    }
    else
    {
        // Максимальное количество байт > 0
        this->maxlen = ml; // Инициализация количества байт для числа
        this->len = ml;
        coef = new BASE[ml]; // Выделение динамической памяти

        if (coef)
        { // Проверка выделения памяти
            if (p == 0)
            { // Если параметр 0, то заполняем все цифры 0
                for (int i = 0; i < ml; i++)
                    coef[i] = 0;
                this->len = 1; // Инициализация количества цифр числа
            }
            else
            { // Иначе заполняем рандомными цифрами
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<BASE> dis(0, std::numeric_limits<BASE>::max());

                for (int i = 0; i < ml; i++)
                    coef[i] = dis(gen); // Генерация случайного числа
				int i = this->len - 1;
				while (i > 0 && coef[i] == 0)
					this->len--;
            }

        }
    }
}
// Конструктор копирования
Big_Number::Big_Number(const Big_Number &bn)
{
	len = bn.len;			 // Копирование значений
	maxlen = bn.maxlen;		 // Копирование значений
	coef = new BASE[maxlen]; // Выделение динамической памяти для массива цифр
	if (coef)
	{ // Проверка выделения динамической памяти
		for (int i = 0; i < bn.maxlen; i++)
		{
			coef[i] = bn.coef[i]; // Копирование цифр
		}
	}
	else
	{
		exit(1);
	}
}

// Перегрузка оператора =
Big_Number Big_Number::operator=(const Big_Number &bn)
{
	if (&bn == this)  // Проверка самоприсвоения
		return *this; // Возвращение самого объекта

	len = bn.len;		// Копирование значений
	maxlen = bn.maxlen; // Копирование значений

	if (coef)				 // Проверка на NULL
		delete coef;		 // Освобождение динамической памяти
	coef = NULL;			 // Присвоение NULL
	coef = new BASE[maxlen]; // Выделение динамической памяти
	if (coef)
	{ // Проверка выделения памяти
		for (int i = 0; i < bn.maxlen; i++)
			coef[i] = bn.coef[i]; // Копирование значений
	}

	return *this; // Возвращение самого объекта
}

// Перегрузка оператора равенства
bool Big_Number::operator==(const Big_Number &bn)
{
	if (this == &bn)
	{ // Проверка на сравнение самим с собой
		return true;
	}

	if (len != bn.len) // Проверка на длину (если количество цифр не одно и
		// то же, то числа разные)
		return false;

	for (int i = len - 1; i >= 0;
		 i--)
	{ // Проход по всем цифрам числа, если хотя бы одно не сходится, то
		// они не равны
		if (this->coef[i] != bn.coef[i]){
			return false;
		}
	}
	return true;
}
// Перегрузка оператора неравенства
bool Big_Number::operator!=(const Big_Number &bn)
{
	if (this == &bn)
	{ // Проверка на сравнение самим с собой
		return false;
	}
	if (len != bn.len) // Проверка на длину (если количество цифр не одно и
		// то же, то числа разные)
		return true;
	for (int i = len - 1; i >= 0;
		 i--)
	{ // Проход по всем цифрам числа, если хотя бы одно не сходится, то
		// они не равны
		if (coef[i] != bn.coef[i])
			return true;
	}
	return false;
}

// Перегрузка оператора больше ИСПРАВИТЬ
bool Big_Number::operator>(const Big_Number &bn)
{
	if (this == &bn || this->len < bn.len) // Если сравнивается объект с самим
		// собой или длины разные, то ложь
		return false;
	if (this->len > bn.len) // Если длина текущего  больше, то истина
		return true;
	for (int i = len - 1; i >= 0; i--)
	{ // Проход по всем цифрам числа, если хотя бы одно меньше, то ложь
		if (coef[i] > bn.coef[i]){
			return true;
		}
		if (coef[i] < bn.coef[i])
			return false;
	}
	return false;
}
// Перегрузка оператора больше или равно
bool Big_Number::operator>=(const Big_Number &bn)
{
    if (this == &bn || this->len > bn.len) // Если сравнивается объект с самим
        // собой или длины разные, то истина
        return true;
    if (this->len < bn.len) // Если длина текущего  больше, то ложь
        return false;
    for (int i = len - 1; i >= 0; i--)
    {
        if (coef[i] < bn.coef[i])
            return false;
        if (coef[i] > bn.coef[i])
		{
            return true;
		}
    }
    return true;
}
// Перегрузка оператора <
bool Big_Number::operator<(const Big_Number &bn)
{
	if (this == &bn || this->len > bn.len) // Если длина текущего больше или сравнивается один и тот же объект
		return false;
	if (this->len < bn.len) // Если длина текущего меньше
		return true;
	for (int i = len - 1; i >= 0; i--) // Проходимся по всем цифрам с начала (в coef записывали наоборот)
	{
			if (coef[i] > bn.coef[i]){
			{
				return false;
			}
			if (coef[i] < bn.coef[i])
			{

			}
				return true;
	}
	}
	return false;
}
// Перегрузка оператора <=
bool Big_Number::operator<=(const Big_Number &bn)
{
	if (this == &bn || this->len < bn.len) // Если сравнивается объект с самим
		// собой или длины разные, то истина
		return true;
	if (this->len > bn.len) // Если длина текущего  больше, то ложь
		return false;
	for (int i = len - 1; i >= 0; i--) // Проходимся по всем цифрам с начала (в coef записывали наоборот)
	{
		if (coef[i] > bn.coef[i]) // Если одна из цифр больше, то ложь
			return false;
		if (coef[i] < bn.coef[i])
			return true;
	}
	return true;
}

// Ввод большого числа в 16-ом виде
// Ввод большого числа в 16-ом виде
void Big_Number::input_hex()
{
	bool flag = false;
	do
	{
		flag = false;
		char buf[128]; // Буфер для строки - числа
		std::cin >> buf; // Ввод строки
		int buf_len = strlen(buf); // Длина введенной строки
		len =(buf_len - 1)/(BASE_SIZE / 4) + 1; // Подсчет длины числа
		delete[] coef; // Очистка памяти
		coef = new BASE[len]; // Выделение памяти под коэффициенты
		// Инициализация коэффициентов нулями
		for (int i = 0; i < len; ++i)
			coef[i] = 0;

		int coef_index = 0; // Индекс текущего коэффициента
		int shift = 0; // Смещение в текущем коэффициенте

		// Обработка символов в обратном порядке
		for (int i = buf_len - 1; i >= 0; --i)
		{
			char ch = buf[i];
			int value = 0;
			if ('0' <= ch && ch <= '9') // Преобразование цифры в число
				value = ch - '0';
			else if ('a' <= ch && ch <= 'f') // Преобразование шестнадцатеричной цифры в число
				value = ch - 'a' + 10;
			else if ('A' <= ch && ch <= 'F') // Преобразование шестнадцатеричной цифры в число
				value = ch - 'A' + 10;
			else // Некорректный символ
			{
				cout <<"Invalid error. Enter a number: ";
				flag = true;
				break;
			}

			// Запись значения в текущий коэффициент
			coef[coef_index] |= value << shift;

			// Переход к следующему коэффициенту или смещение
			shift += 4;
			if (shift >= BASE_SIZE) // Если достигнут конец текущего коэффициента
			{
				shift = 0;
				coef_index++;
			}
		}
	
	}while (flag);
	int i = len - 1;
	while (i > 0 && coef[i] == 0)
		len--;
}


// Вывод в 16-ой системе
void Big_Number::output_hex()
{
    bool non_zero_found = false; // Флаг, указывающий, была ли найдена ненулевая цифра

    int k = BASE_SIZE - 4; // для смещения коэффициента
    int j = len - 1;

    while (j >= 0)
    {
        unsigned char tmp = (coef[j] >> k) & 0xf;

        if (tmp != 0 || non_zero_found) {
            // Если текущая цифра не ноль или уже была найдена ненулевая цифра,
            // то выводим текущую цифру
            non_zero_found = true; // Устанавливаем флаг
            if (0 <= tmp && tmp <= 9)
                std::cout << (char)(tmp + '0');
            else if (10 <= tmp && tmp <= 15)
                std::cout << (char)(tmp - 10 + 'a');
        }

        k -= 4;

        if (k < 0) // Если смещение стало отрицательным, сбросить его и перейти к предыдущему коэффициенту
        {
            k = BASE_SIZE - 4;
            j--;
        }
    }
    if (!non_zero_found) // Если ненулевая цифра не была найдена, значит число ноль, вывести "0"
        std::cout << '0';

    std::cout << std::endl;
}

// Перегрузка оператора сложения +
Big_Number Big_Number::operator+(const Big_Number &bn) {
    // Определение длин чисел
    int n = this->len;
    int m = bn.len;
    int max_len = std::max(n, m) + 1; // учитываем дополнительный разряд для переноса

    // Создание объекта для результата
    Big_Number result(max_len);

    // Инициализация переменной для переноса
    DBASE carry = 0;

    // Сложение разрядов
    for (int j = 0; j < max_len; ++j) {
        // Получение текущих цифр из двух чисел и при необходимости переноса
        DBASE uj = (j < n ? this->coef[j] : 0);
        DBASE vj = (j < m ? bn.coef[j] : 0);

        // Сложение текущих цифр с учетом переноса
        DBASE tmp = uj + vj + carry;

        // Присваивание результату текущего разряда
        result.coef[j] = tmp & std::numeric_limits<BASE>::max();

        // Обновление значения переноса
        carry = tmp >> BASE_SIZE;
    }

    // Устанавливаем длину результата
    result.len = max_len;

    return result;
}

// Перегрузка оператора +=
Big_Number& Big_Number::operator+=(const Big_Number &bn) {
    // Определение длин чисел
    int n = this->len;
    int m = bn.len;
    int max_len = std::max(n, m) + 1; // учитываем дополнительный разряд для переноса

    // Создаем временный массив для хранения результата сложения
    BASE *temp = new BASE[max_len];

    // Инициализация переменной для переноса
    DBASE carry = 0;

    // Сложение разрядов
    for (int j = 0; j < max_len; ++j) {
        // Получение текущих цифр из двух чисел и при необходимости переноса
        DBASE uj = (j < n ? this->coef[j] : 0);
        DBASE vj = (j < m ? bn.coef[j] : 0);

        // Сложение текущих цифр с учетом переноса
        DBASE tmp = uj + vj + carry;

        // Присваивание результату текущего разряда
        temp[j] = tmp & std::numeric_limits<BASE>::max();

        // Обновление значения переноса
        carry = tmp >> BASE_SIZE;
    }

    // Если был перенос за пределы максимальной длины, увеличиваем длину объекта
    if (carry > 0 && this->len < max_len)
        max_len = max_len;

    // Удаляем ведущие нули, если они есть
    while (max_len > 1 && temp[max_len - 1] == 0)
        max_len--;

    // Очищаем старые данные и копируем новый результат
    delete[] this->coef;
    this->coef = new BASE[max_len];
    this->len = max_len;
    std::copy(temp, temp + max_len, this->coef);

    // Освобождаем временный массив
    delete[] temp;

    return *this;
}

// Перегрузка оператора вычитания -
Big_Number Big_Number::operator-(const Big_Number &bn) {
    int max_len = std::max(len, bn.len); // Максимальная длина результата
    Big_Number result(max_len); // Создание объекта для результата

    DBASE borrow = 0; // Инициализация переменной для заема

    for (int j = 0; j < max_len; ++j) {
        DBASE uj = (j < len ? coef[j] : 0); // Получение текущей цифры из первого числа или нуля, если разряд уже закончился
        DBASE vj = (j < bn.len ? bn.coef[j] : 0); // Получение текущей цифры из второго числа или нуля, если разряд уже закончился

        DBASE tmp = uj - vj - borrow; // Вычитание текущих цифр с учетом заема

        if (tmp < 0) {
            tmp += (1 << BASE_SIZE); // Если результат отрицательный, добавляем базу
            borrow = 1; // Устанавливаем заем
        } else {
            borrow = 0; // Сбрасываем заем
        }

        result.coef[j] = BASE(tmp); // Присваиваем результат текущему разряду
    }

    // Удаляем ведущие нули, если они есть
    while (max_len > 1 && result.coef[max_len - 1] == 0)
        max_len--;

    result.len = max_len; // Устанавливаем длину результата

    return result;
}

// Перегрузка оператора -=
Big_Number& Big_Number::operator-=(const Big_Number &bn) {
    int max_len = std::max(len, bn.len); // Максимальная длина результата

    // Создаем временный массив для хранения результата вычитания
    BASE *temp = new BASE[max_len];

    DBASE borrow = 0; // Инициализация переменной для заема

    for (int j = 0; j < max_len; ++j) {
        DBASE uj = (j < len ? coef[j] : 0); // Получение текущей цифры из первого числа или нуля, если разряд уже закончился
        DBASE vj = (j < bn.len ? bn.coef[j] : 0); // Получение текущей цифры из второго числа или нуля, если разряд уже закончился

        DBASE tmp = uj - vj - borrow; // Вычитание текущих цифр с учетом заема

        if (tmp < 0) {
            tmp += (1 << BASE_SIZE); // Если результат отрицательный, добавляем базу
            borrow = 1; // Устанавливаем заем
        } else {
            borrow = 0; // Сбрасываем заем
        }

        temp[j] = BASE(tmp); // Присваиваем результат текущему разряду
    }

    // Удаляем ведущие нули, если они есть
    while (max_len > 1 && temp[max_len - 1] == 0)
        max_len--;

    // Очищаем старые данные и копируем новый результат
    delete[] coef;
    coef = new BASE[max_len];
    len = max_len;
    std::copy(temp, temp + max_len, coef);

    // Освобождаем временный массив
    delete[] temp;

    return *this;
}
Big_Number Big_Number::operator*(BASE number) {
    Big_Number result(len + 1); // Создаем объект для результата умножения

    DBASE tmp; // Переменная для временного хранения промежуточного результата
    int j = 0;
    int k = 0;

    while (j < len) {
        tmp = coef[j] * number + k; // Умножаем текущую цифру на число и прибавляем предыдущий перенос
        result.coef[j] = (BASE)tmp; // Записываем младшие разряды в текущий элемент результата
        k = (BASE)(tmp >> BASE_SIZE); // Вычисляем новый перенос
        j++;
    }

    // Записываем оставшийся перенос в последний разряд результата
    result.coef[j] = k;

    // Обновляем длину результата
    result.len = j + 1;

    return result;
}

Big_Number& Big_Number::operator*=(BASE number) {
    DBASE tmp; // Переменная для временного хранения промежуточного результата
    int j = 0;
    int k = 0;

    while (j < len) {
        tmp = coef[j] * number + k; // Умножаем текущую цифру на число и прибавляем предыдущий перенос
        coef[j] = (BASE)tmp; // Записываем младшие разряды в текущий элемент
        k = (BASE)(tmp >> BASE_SIZE); // Вычисляем новый перенос
        j++;
    }

    // Если остался перенос после цикла, добавляем его как новый разряд
    if (k > 0) {
        // Если текущая длина недостаточна для хранения нового разряда, увеличиваем размер массива
        if (len == maxlen) {
            maxlen++;
            BASE* new_coef = new BASE[maxlen];
            std::copy(coef, coef + len, new_coef);
            delete[] coef;
            coef = new_coef;
        }
        coef[j] = k;
        len++; // Увеличиваем длину числа
    }

    return *this;
}


Big_Number Big_Number::operator*(const Big_Number &bn) {
    int m = len;
    int n = bn.len;
    int max_len = m + n; // Максимальная длина результата

    // Создаем объект для результата умножения
    Big_Number result(max_len);

    // Устанавливаем начальные значения коэффициентов результата в ноль
    for (int i = 0; i < max_len; ++i)
        result.coef[i] = 0;

    // Умножение большого числа на большое число
    for (int j = 0; j < n; ++j) {
        if (bn.coef[j] == 0)
            continue; // Пропускаем умножение на нулевой коэффициент

        BASE k = 0; // Перенос

        for (int i = 0; i < m; ++i) {
            DBASE tmp = result.coef[i + j] + (DBASE)coef[i] * bn.coef[j] + k;
            result.coef[i + j] = (BASE)tmp; // Младшие разряды
            k = (BASE)(tmp >> BASE_SIZE); // Перенос
        }

        // Установка старшего разряда
        result.coef[j + m] = k;
    }

    // Обновление длины результата
    result.len = max_len;

    // Удаление ведущих нулей, если они есть
    while (result.len > 1 && result.coef[result.len - 1] == 0)
        result.len--;

    return result;
}

Big_Number& Big_Number::operator*=(const Big_Number &bn) {
    int m = len;
    int n = bn.len;
    int max_len = m + n; // Максимальная длина результата

    // Создаем временный массив для хранения результата умножения
    BASE *temp = new BASE[max_len];
    std::fill(temp, temp + max_len, 0); // Устанавливаем все элементы в 0

    // Умножение большого числа на большое число
    for (int j = 0; j < n; ++j) {
        if (bn.coef[j] == 0)
            continue; // Пропускаем умножение на нулевой коэффициент

        BASE k = 0; // Перенос

        for (int i = 0; i < m; ++i) {
            DBASE tmp = temp[i + j] + (DBASE)coef[i] * bn.coef[j] + k;
            temp[i + j] = (BASE)tmp; // Младшие разряды
            k = (BASE)(tmp >> BASE_SIZE); // Перенос
        }

        // Установка старшего разряда
        temp[j + m] = k;
    }

    // Очищаем старые данные и копируем новый результат
    delete[] coef;
    coef = new BASE[max_len];
    std::copy(temp, temp + max_len, coef);

    // Обновление длины результата
    len = max_len;

    // Удаление ведущих нулей, если они есть
    while (len > 1 && coef[len - 1] == 0)
        len--;

    // Освобождаем временный массив
    delete[] temp;

    return *this;
}



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
    } else {
        std::cout << "The first number is not equal to the second number.\n"; // Первое число не равно второму числу
    }
    BASE m = 2;
    Big_Number resultMult = bn1 * 3;
    resultMult.output_hex();
    resultMult*= m;
    resultMult.output_hex();

    Big_Number res = bn1 * bn2;
    res.output_hex();
    return 0;
}
