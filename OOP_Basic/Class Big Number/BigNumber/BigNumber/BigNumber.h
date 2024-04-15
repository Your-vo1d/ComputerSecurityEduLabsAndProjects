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
	int len;	// ������
	int maxlen; // ���������� ������
	BASE* coef; // ������ ����

public:
	Big_Number(int ml = 1, int pr = 0); // ����������� �� ���������/����������
	Big_Number(const Big_Number& bn);	// ����������� �����������
	~Big_Number() // ����������
	{
		if (coef) //�������� ���������
			delete[] coef;
		coef = NULL;
		len = 0;
		maxlen = 0;
	};
	; // ����������
	Big_Number operator=(const Big_Number& bn); // ���������� ��������� ����������

	void input_hex();  // ���� �������� ����� � 16-�� ����
	void output_hex(); // ����� �������� ����� � 16-�� ����

	bool operator==(const Big_Number& bn); // ���������� ��������� ==
	bool operator!=(const Big_Number& bn); // ���������� ��������� !=
	bool operator<(const Big_Number& bn);  // ���������� ��������� <
	bool operator<=(const Big_Number& bn); // ���������� ��������� <=
	bool operator>(const Big_Number& bn);  // ���������� ��������� >
	bool operator>=(const Big_Number& bn); // ���������� ��������� >=

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
	// ���������� ��������� ������ � �����
	friend ostream& operator << (ostream& r, const Big_Number& bn);

	// ���������� ��������� ����� �� ������
	friend istream& operator >> (istream& r, Big_Number& bn);

};
#endif // BIG_NUMBER_H
