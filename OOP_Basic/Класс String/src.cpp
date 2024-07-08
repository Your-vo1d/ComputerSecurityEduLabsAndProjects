#include <iostream>
#include <cstring>

class String
{
    char *str;
    int len;

public:
    String(int l = 0);          // Конструктор с 1 аргументом - длиной строки
    String(const char *);       // Конструктор с 1 аргументом - указателем на строку
    String(const String &);     // Конструктор копирования
    ~String();                  // Деструктор
    int getLen() const;         // Геттер для получения длины строки
    String &operator=(const String &);            // Перегрузка для операции '=' - присвоение
    char operator[](int) const; // Перегрузка для операции '[]' - элемент по индексу
    bool operator==(const String &) const;       // Перегрузка для операции '==' - сравнение
    bool operator!=(const String &) const;       // Перегруза для операции '!=' неравенство
    String &operator+(const String ) const;      // Перегрузка для операции '+' конкатенация
    String &operator+=(const String &);           // Перегрузка для операции '+=' конкатенация
    friend std::istream &operator>>(std::istream &, String &);  // Потоковый ввод
    friend std::ostream &operator<<(std::ostream &, const String &);  // Потоковый вывод
    int BM_search(const String &) const;
};

// Конструктор с аргументом по умолчанию
String::String(int l) {
    str = new char[l];
    len = 0;
    str[0] = '\0';
}

// Конструктор с аргументом строкой
String::String(const char *s) {
    len = strlen(s);
    str = new char[len + 1];
    strcpy(str, s);
}

// Конструктор копирования
String::String(const String &s) {
    len = s.len;
    str = new char[len + 1];
    strcpy(str, s.str);
}

// Деструктор
String::~String() {
    if (str) {
        delete[] str;
        str = NULL;
    }
}

// Геттер для получения длины строки
int String::getLen() const {
    return len;
}

// Перегрузка для операции '='
String &String::operator=(const String &s) {
    if (this != &s) {
        delete[] str;
        len = s.len;
        str = new char[len + 1];
        strcpy(str, s.str);
    }
    return *this;
}

// Перегрузка для операции '[]'
char String::operator[](int i) const {
    if (i < 0 || i >= len) {
        std::cout << "\n Индекс за пределами строки";
        return str[0];
    }
    return str[i];
}

// Перегрузка для операции '=='
bool String::operator==(const String &s) const {
    return strcmp(str, s.str) == 0;
}

// Перегрузка для операции '!='
bool String::operator!=(const String &s) const {
    return strcmp(str, s.str) != 0;
}

// Перегрузка для операции '+' конкатенация
String &String::operator+(const String s) const {
    String result(len + s.len);
    strcpy(result.str, str);
    strcat(result.str, s.str);
    result.len = strlen(result.str);
    return result;
}

// Перегрузка для операции '+=' конкатенация
String &String::operator+=(const String &s) {
    char *new_str = new char[len + s.len + 1];
    strcpy(new_str, str);
    strcat(new_str, s.str);
    delete[] str;
    str = new_str;
    len = strlen(str);
    return *this;
}

// Потоковый ввод
std::istream &operator>>(std::istream &in, String &s) {
    char buffer[1024];
    in.getline(buffer, sizeof(buffer));
    s = String(buffer);
    return in;
}

// Потоковый вывод
std::ostream &operator<<(std::ostream &out, const String &s) {
    out << s.str;
    return out;
}

// Реализация алгоритма Бойера-Мура
int String::BM_search(const String &s) const {
    int tab[256];
    for (int i = 0; i < 256; i++) {
        tab[i] = s.len;
    }

    for (int i = 0; i < s.len - 1; i++) {
        tab[s[i]] = s.len - 1 - i;
    }

    int indexStr = s.len - 1;
    int indexS = s.len - 1;

    while (indexStr < len) {
        indexS = s.len - 1;
        int k = indexStr;

        while (indexS >= 0 && str[k] == s[indexS]) {
            k--;
            indexS--;
        }

        if (indexS == -1) {
            return k + 1; // Найдено совпадение
        } else {
            indexStr += tab[str[indexStr]]; // Используем tab для сдвига
        }
    }

    return -1; // Не найдено совпадение
}




int main() {
    // Создаем строку s1 и выводим ее длину
    String s1("Hello, World!");
    std::cout << "s1: " << s1 << ", Length: " << s1.getLen() << std::endl;

    // Конструктор копирования
    String s2 = s1;
    std::cout << "s2 (copy of s1): " << s2 << ", Length: " << s2.getLen() << std::endl;

    // Перегрузка оператора присвоения
    String s3;
    s3 = s1;
    std::cout << "s3 (assigned from s1): " << s3 << ", Length: " << s3.getLen() << std::endl;

    // Перегрузка оператора равенства и неравенства
    if (s1 == s2) {
        std::cout << "s1 is equal to s2" << std::endl;
    } else {
        std::cout << "s1 is not equal to s2" << std::endl;
    }

    if (s1 != s3) {
        std::cout << "s1 is not equal to s3" << std::endl;
    } else {
        std::cout << "s1 is equal to s3" << std::endl;
    }

    // Перегрузка оператора конкатенации
    String s4(" from s4");
    String s5 = s1 + s4;
    std::cout << "s5 (s1 + s4): " << s5 << ", Length: " << s5.getLen() << std::endl;

    // Перегрузка оператора конкатенации с присвоением
    s1 += s4;
    std::cout << "s1 after s1 += s4: " << s1 << ", Length: " << s1.getLen() << std::endl;

    // Перегрузка оператора индексации
    std::cout << "s5[7]: " << s5[7] << std::endl;

    // Проверка ввода строки
    String s6;
    std::cout << "Enter a string: ";
    std::cin >> s6;
    std::cout << "You entered: " << s6 << ", Length: " << s6.getLen() << std::endl;

    //Проверка БМ-поиска
    String sBM;
    std::cin >> sBM;
    int index = s6.BM_search(sBM);
    std::cout << index <<std::endl;
    
    return 0;
}
