#pragma once
#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <stdlib.h>
#include <iomanip>
#include <type_traits>
#include <cstring>
#include <iostream>
#include <random>
typedef short int BASE;

typedef long int DBASE;

#define BASE_SIZE (sizeof(BASE) * 8)

using namespace std;

class Big_Number
{
	int len;	// Разряд
	int maxlen; // Выделенная память
	BASE* coef; // Массив цифр

public:
	Big_Number(int ml = 1, int pr = 0); // Конструктор по умолчанию/параметрам
	Big_Number(const Big_Number& bn);	// Конструктор копирования
	~Big_Number() // Деструктор
	{
		if (coef) //Проверка указателя
			delete[] coef;
		coef = NULL;
		len = 0;
		maxlen = 0;
	};
	; // Деструктор
	Big_Number operator=(const Big_Number& bn); // Перегрузка оператора присвоения

	void input_hex();  // Ввод большого числа в 16-ом виде
	void output_hex(); // Вывод большого числа в 16-ом виде

	bool operator==(const Big_Number& bn); // Перегрузка оператора ==
	bool operator!=(const Big_Number& bn); // Перегрузка оператора !=
	bool operator<(const Big_Number& bn);  // Перегрузка оператора <
	bool operator<=(const Big_Number& bn); // Перегрузка оператора <=
	bool operator>(const Big_Number& bn);  // Перегрузка оператора >
	bool operator>=(const Big_Number& bn); // Перегрузка оператора >=

	Big_Number operator+(const Big_Number& bn);
	Big_Number& operator+=(const Big_Number& bn);

	Big_Number operator-(const Big_Number& bn);
	Big_Number& operator-=(const Big_Number& bn);

	Big_Number operator*(const Big_Number& bn);
	Big_Number& operator*=(const Big_Number& bn);

	Big_Number operator*(const BASE number);
	Big_Number& operator*=(const BASE number);

	Big_Number operator/(const BASE number);
	Big_Number operator %(const BASE number);

	Big_Number& operator/=(const BASE number);
	Big_Number& operator %=(const BASE number);

	Big_Number operator/(const Big_Number& bn);
	
	void trim();
	bool isZero();
	// Перегрузка оператора вывода в поток
	friend ostream& operator << (ostream& r, const Big_Number& bn);

	// Перегрузка оператора ввода из потока
	friend istream& operator >> (istream& r, Big_Number& bn);

};
#endif // BIG_NUMBER_H
