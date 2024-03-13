#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <cstring>
#include <iostream>

typedef unsigned char BASE;

#define BASE_SIZE (sizeof(BASE) * 8)

using namespace std;

class Big_Number
{
	int len;	// Разряд
	int maxlen; // Выделенная память
	BASE *coef;

public:
	Big_Number(int ml = 1, int pr = 0); // Конструктор по умолчанию/параметрам
	Big_Number(const Big_Number &bn);	// Конструктор копирования
	~Big_Number()
	{
		if (coef)
			delete[] coef;
		coef = NULL;
		len = 0;
		maxlen = 0;
	};
	; // Деструктор
	Big_Number operator=(
		const Big_Number &bn); // Перегрузка оператора присвоения

	bool input_hex();  // Ввод большого числа в 16-ом виде
	void output_hex(); // Вывод большого числа в 16-ом виде

	bool operator==(const Big_Number &bn); // Перегрузка оператора ==
	bool operator!=(const Big_Number &bn); // Перегрузка оператора !=
	bool operator<(const Big_Number &bn);  // Перегрузка оператора <
	bool operator<=(const Big_Number &bn); // Перегрузка оператора <=
	bool operator>(const Big_Number &bn);  // Перегрузка оператора >
	bool operator>=(const Big_Number &bn); // Перегрузка оператора >=
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
			{	// Иначе заполняем рандомными цифрами
				for (int i = 0; i < ml; i++)
					coef[i] = rand() % 256; //ИСПРАВИТ
				int i = len - 1;
				while (i > 0 && coef[i] == 0)
					len--;
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

	if (this->len != bn.len) // Проверка на длину (если количество цифр не одно и
		// то же, то числа разные)
		return false;

	for (size_t i = len - 1; i >= 0;
		 i--)
	{ // Проход по всем цифрам числа, если хотя бы одно не сходится, то
		// они не равны
		if (coef[i] != bn.coef[i])
			return false;
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
	if (this->len != bn.len) // Проверка на длину (если количество цифр не одно и
		// то же, то числа разные)
		return true;
	for (size_t i = len - 1; i >= 0;
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
	for (size_t i = len - 1; i >= 0;
		 i--)
	{ // Проход по всем цифрам числа, если хотя бы одно меньше, то ложь
		if (coef[i] < bn.coef[i] || (i == 0 && coef[i] == bn.coef[i]))
			return false;
	}
	return true;
}
// Перегрузка оператора больше или равно
bool Big_Number::operator>=(const Big_Number &bn)
{
	if (this == &bn || this->len > bn.len) // Если сравнивается объект с самим
		// собой или длины разные, то истина
		return true;
	if (this->len < bn.len) // Если длина текущего  больше, то ложь
		return false;
	for (size_t i = len - 1; i >= 0; i--)
	{
		if (coef[i] < bn.coef[i])
			return false;
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
	for (size_t i = len - 1; i >= 0; i--) // Проходимся по всем цифрам с начала (в coef записывали наоборот)
	{
		if (coef[i] > bn.coef[i] || (i == 0 && coef[i] == bn.coef[i])) // Если одна из цифр больше или конечные цифры равны, то ложь
			return false;
	}
	return true;
}
// Перегрузка оператора <=
bool Big_Number::operator<=(const Big_Number &bn)
{
	if (this == &bn || this->len < bn.len) // Если сравнивается объект с самим
		// собой или длины разные, то истина
		return true;
	if (this->len > bn.len) // Если длина текущего  больше, то ложь
		return false;
	for (size_t i = len - 1; i >= 0; i--) // Проходимся по всем цифрам с начала (в coef записывали наоборот)
	{
		if (coef[i] > bn.coef[i]) // Если одна из цифр больше, то ложь
			return false;
	}
	return true;
}

// Ввод большого числа в 16-ом виде
bool Big_Number::input_hex()
{
    char buf[128]; // Буфер для строки - числа

    std::cin >> buf;                                       // Ввод строки
    len = (strlen(buf) - 1) / (BASE_SIZE / 4) + 1;         // Подсчет длины
    size_t index_Big_Number = 0;
	delete coef;
	coef = NULL;                         // Индекс по цифрам большого числа
	coef = new BASE[len]; 
	for (int i = 0; i < maxlen; i++)
	coef[i] = 0;                           // Динамический массив для коэффициентов
    int offset = 0;
    int tmp = 0;

    for (int index_str = strlen(buf) - 1; index_str >= 0; index_str--)
    {
        if ('0' <= buf[index_str] && buf[index_str] <= '9') // Если символ - цифра, то преобразуем его в число
        {
            tmp = buf[index_str] - '0';
        }
        else if ('a' <= buf[index_str] && buf[index_str] <= 'f') // Если символ - цифра 16-ой системы, то преобразуем его в число
        {
            tmp = buf[index_str] - 'a' + 10;
        }
        else if ('A' <= buf[index_str] && buf[index_str] <= 'F') // Если символ - цифра 16-ой системы, то преобразуем его в число
        {
            tmp = buf[index_str] - 'A' + 10;
        }
        else // Иначе возвращаем false
        {
            delete[] coef;
            return false;
        }
		cout <<"TMP - " <<tmp <<endl;			
		cout<<"dfsdsa"<<endl;
			for (int i = len - 1; i >=0; i--)
		cout<<coef[i];			
		cout<<"qwer"<<endl;
        coef[index_Big_Number] |= tmp << (offset * 4); // Записываем число, смещая его на коэф. * 4 (преобразование в 10 число)
        offset++;
        if (offset >= BASE_SIZE / 4) // Конец текущей цифры
        {
            offset = 0;
            index_Big_Number++;
        }
    }
    return true;
}
// Вывод в 16-ой системе
// Вывод в 16-ой системе
void Big_Number::output_hex()
{
	char *result = new char[len * 2];
	int k = BASE_SIZE - 4;
	int i = 0;
	int j = len - 1;
	while (j >= 0) {
		char tmp = ((coef[j] >>k) & (0xf));
		if (tmp < 10 && tmp >= 0)
		{
			result[i] = (char)(tmp + '0');
		}
		else if (tmp < 16 && tmp >= 10)
		{
			result[i]= (char)(tmp + 'a' - 10);
		}
		i++;
		k -=4;
		if (k < 0) {
			k = BASE_SIZE - 4;
			j--;
		}
	}
	for (int i = 0; i < len; i++) {
		cout <<result[i];
	}
	cout <<endl;
}

int main()
{
	Big_Number bn_1(1);
	cout << "Big number created by the constructor with parameter ml = 1" << endl;
	bn_1.output_hex();

	Big_Number bn_2(4, 1);
	cout << "Big number created by the constructor with parameters ml = 4, p = 1 (max length 4, random digits)" << endl;
	bn_2.output_hex();

	Big_Number bn_3(4);
	cout << "Big number created by the constructor with parameter ml = 4 (max length 4)" << endl;
	bn_3.output_hex();

	Big_Number bn_4(4);
	cout << "Big number created by the default constructor (without parameters)" << endl;
	bn_4.output_hex();

	Big_Number bn_5(2);
	if (bn_5.input_hex()) {
		cout<<"HEX";
		bn_5.output_hex();
	}

	if (bn_1 == bn_3) {
		cout<<"YES";
	}
	else
		cout<<"NO";
	
	if (bn_1 > bn_2) {
		cout<<"YES";
	}
	else
		cout<<"NO";

	if (bn_1 >= bn_2) {
		cout<<"YES";
	}
	else
		cout<<"NO";

	
	if (bn_1 < bn_2) {
		cout<<"YES";
	}
	else
		cout<<"NO";


}