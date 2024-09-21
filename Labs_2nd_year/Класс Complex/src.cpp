#include <math.h>
#include <iostream>
#include <string.h>
#include <limits>

typedef enum {FALSE, TRUE} boolean;

class Complex {
    private: double re, im; //Член - данные
    
    public:
		Complex() : re(0.0), im(0.0) {} // Конструктор по умолчанию

    	Complex(double r) : re(r), im(0.0) {} // Конструктор с 1 аргументом

    	Complex(double r, double i) : re(r), im(i) {} // Конструктор с 2 аргументами

	    boolean operator == (Complex &c) { //Перегрузка '=='
	        if (re == c.re && im == c.im)
	            return TRUE;
	        else
	            return FALSE;
	    } 

		boolean operator != (Complex &c) { //Перегрузка '!='
			if (re != c.re || im != c.im)
				return TRUE;
			else
				return FALSE;
		}

	    Complex operator+(Complex &c) { //Перегрузка '+'
	        Complex temp;
	        temp.re = re + c.re;
	        temp.im = im + c.im;
	        return temp;
	    }

	    Complex & operator+=(Complex &c) { //Перегрузка '+='
	        re += c.re;
	        im += c.im;
	        return *this;
	    }

	    Complex operator- (Complex &c) { //Перегрузка '-'
	        Complex temp;
	        temp.re = re - c.re;
	        temp.im = im - c.im;
	        return temp;
	    }

	    Complex & operator -= (Complex &c) { //Перегрузка '-='
	        re -= c.re;
	        im -= c.im;
	        return *this;
	    }

	    Complex operator * (Complex &c) { //Перегрузка '*'
	        Complex temp;
	        temp.re = re * c.re - im * c.im;
	        temp.im = im * c.re + re * c.im;
	        return temp;
	    }

	    Complex & operator *= (Complex &c) { //Перегрузка '*='
	        Complex temp;
	        temp.re = re * c.re - im * c.im;
	        temp.im = im * c.re + re * c.im;
	        *this = temp;
	        return *this;
	    }
    
	    Complex operator / (Complex &c) { //Перегрузка '/'
	        Complex temp;
	        double z;
	        z = c.re * c.re + c.im * c.im;
	        if (z == 0) {
	            std::cout << "\n Divide by zero!";
	            exit(1);
	        }
	        temp.re = (re * c.re + im * c.im) / z;
	        temp.im = (im * c.re - re * c.im) / z;
	        return temp;
	    }
	
	    Complex & operator /= (Complex &c) { //Перегрузка '/='
	        Complex temp;
	        double z;
	        z = c.re * c.re + c.im * c.im;
	        if (z == 0) {
	            std::cout << "\n Divide by zero!";
	            exit(1);
	        }
	        temp.re = (re * c.re + im * c.im) / z;
	        temp.im = (im * c.re - re * c.im) / z;
	        *this = temp;
	        return *this;
	    }

	    double operator !() { //Модуль
	        return sqrt(re * re + im * im);
	    }

	    // Геттеры для целой и мнимой частей
	    double getReal() const {
	        return re;
	    }
	
	    double getImaginary() const {
	        return im;
	    }
	
	    // Сеттеры для целой и мнимой частей
	    void setReal(double r) {
	        re = r;
	    }
	
	    void setImaginary(double i) {
	        im = i;
	    }

		friend std::istream& operator>>(std::istream& in, Complex& c) {
	        double real, imag;
	        std::cout << "Введите вещественную и мнимую части через пробел: ";
	        
	        while (!(in >> real >> imag)) {
	            std::cout << "Ошибка ввода. Пожалуйста, введите два вещественных числа: ";
	            in.clear(); // Очистить флаги ошибки
	            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Пропустить некорректный ввод
	        }
	        
	        c.setReal(real);
	        c.setImaginary(imag);
	        return in;
    	}

	    friend std::ostream& operator<<(std::ostream& out, const Complex& c) {
	        out << c.getReal() << " + " << c.getImaginary() << "i";
	        return out;
	    }

	    void print() {
	        std::cout << re <<" + " << im << "i\n";
	    }
};


int main() {
    Complex c1, c2, c3;

    std::cout << "Введите первое комплексное число (формат: real imag): " << std::endl;
    std::cin >> c1;

    std::cout << "Введите второе комплексное число (формат: real imag): " << std::endl;
    std::cin >> c2;

    Complex c4(4.0); // Создаем объект с одним аргументом (реальная часть)
    Complex c5(0.0, 2.0); // Создаем объект с двумя аргументами (реальная и мнимая части)

    std::cout << "Третье комплексное число (конструктор с одним аргументом): ";
    c4.print();
    std::cout << std::endl;

    std::cout << "Четвертое комплексное число (конструктор с двумя аргументами): ";
    c5.print();
    std::cout << std::endl;

    c3 = c1 + c2;
    std::cout << "Сумма: ";
    c3.print();
    std::cout << std::endl;

    c3 = c1 - c2;
    std::cout << "Разность: ";
    c3.print();
    std::cout << std::endl;

    c3 = c1 * c2;
    std::cout << "Произведение: ";
    c3.print();
    std::cout << std::endl;

    c3 = c1 / c2;
    std::cout << "Частное: ";
    c3.print();
    std::cout << std::endl;

    c1 += c2;
    std::cout << "После +=: ";
    c1.print();
    std::cout << std::endl;

    c1 -= c2;
    std::cout << "После -=: ";
    c1.print();
    std::cout << std::endl;

    c1 *= c2;
    std::cout << "После *=: ";
    c1.print();
    std::cout << std::endl;

    c1 /= c2;
    std::cout << "После /=: ";
    c1.print();
    std::cout << std::endl;

    std::cout << "Модуль первого комплексного числа: " << !c1 << std::endl;
    std::cout << "Модуль второго комплексного числа: " << !c2 << std::endl;

    if (c1 == c2) {
        std::cout << "Первое и второе комплексные числа равны." << std::endl;
    } else {
        std::cout << "Первое и второе комплексные числа не равны." << std::endl;
    }

	if (c3 != c2) {
		std::cout << "Первое и второе комплексные числа не равны." << std::endl;
	} else {
		std::cout << "Первое и второе комплексные числа равны." << std::endl;
	}
	
    return 0;
}
