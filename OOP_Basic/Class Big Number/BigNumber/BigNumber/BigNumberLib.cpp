#include "BigNumber.h"


// ����������� �� ���������/����������
#include <random>

Big_Number::Big_Number(int ml, int p)
{
    if (ml < 1)
    {
        exit(1);
    }
    else
    {
        // ������������ ���������� ���� > 0
        this->maxlen = ml; // ������������� ���������� ���� ��� �����
        this->len = ml;
        coef = new BASE[ml]; // ��������� ������������ ������

        if (coef)
        { // �������� ��������� ������
            if (p == 0)
            { // ���� �������� 0, �� ��������� ��� ����� 0
                for (int i = 0; i < ml; i++)
                    coef[i] = 0;
                this->len = 1; // ������������� ���������� ���� �����
            }
            else
            { // ����� ��������� ���������� �������
                std::random_device rd;
                std::mt19937 gen(rd());

                if constexpr (std::is_signed<BASE>::value) {
                    std::uniform_int_distribution<int> dis(std::numeric_limits<BASE>::min(), std::numeric_limits<BASE>::max());
                    for (int i = 0; i < ml; i++) {
                        coef[i] = static_cast<BASE>(dis(gen)); // ��������� ���������� �����
                    }

                }
                else {
                    std::uniform_int_distribution<int> dis(0, std::numeric_limits<BASE>::max());
                    for (int i = 0; i < ml; i++) {
                        coef[i] = static_cast<BASE>(dis(gen)); // ��������� ���������� �����
                    }

                }

                int i = this->len - 1;
                while (i > 0 && coef[i] == 0)
                    this->len--;
            }
        }
    }
}




// ����������� �����������
Big_Number::Big_Number(const Big_Number& bn)
{
    len = bn.len;			 // ����������� ��������
    maxlen = bn.maxlen;		 // ����������� ��������
    coef = new BASE[maxlen]; // ��������� ������������ ������ ��� ������� ����
    if (coef)
    { // �������� ��������� ������������ ������
        for (int i = 0; i < bn.maxlen; i++)
        {
            coef[i] = bn.coef[i]; // ����������� ����
        }
    }
    else
    {
        exit(1);
    }
}

// ���������� ��������� =
Big_Number Big_Number::operator=(const Big_Number& bn)
{
    if (&bn == this)  // �������� ��������������
        return *this; // ����������� ������ �������

    len = bn.len;		// ����������� ��������
    maxlen = bn.maxlen; // ����������� ��������

    if (coef)				 // �������� �� NULL
        delete coef;		 // ������������ ������������ ������
    coef = NULL;			 // ���������� NULL
    coef = new BASE[maxlen]; // ��������� ������������ ������
    if (coef)
    { // �������� ��������� ������
        for (int i = 0; i < bn.maxlen; i++)
            coef[i] = bn.coef[i]; // ����������� ��������
    }

    return *this; // ����������� ������ �������
}

// ���������� ��������� ���������
bool Big_Number::operator==(const Big_Number& bn)
{
    if (this == &bn)
    { // �������� �� ��������� ����� � �����
        return true;
    }

    if (len != bn.len) // �������� �� ����� (���� ���������� ���� �� ���� �
        // �� ��, �� ����� ������)
        return false;

    for (int i = len - 1; i >= 0;
        i--)
    { // ������ �� ���� ������ �����, ���� ���� �� ���� �� ��������, ��
        // ��� �� �����
        if (this->coef[i] != bn.coef[i]) {
            return false;
        }
    }
    return true;
}
// ���������� ��������� �����������
bool Big_Number::operator!=(const Big_Number& bn)
{
    if (this == &bn)
    { // �������� �� ��������� ����� � �����
        return false;
    }
    if (len != bn.len) // �������� �� ����� (���� ���������� ���� �� ���� �
        // �� ��, �� ����� ������)
        return true;
    for (int i = len - 1; i >= 0;
        i--)
    { // ������ �� ���� ������ �����, ���� ���� �� ���� �� ��������, ��
        // ��� �� �����
        if (coef[i] != bn.coef[i])
            return true;
    }
    return false;
}

// ���������� ��������� ������ ���������
bool Big_Number::operator>(const Big_Number& bn)
{
    if (this == &bn || this->len < bn.len) // ���� ������������ ������ � �����
        // ����� ��� ����� ������, �� ����
        return false;
    if (this->len > bn.len) // ���� ����� ��������  ������, �� ������
        return true;
    for (int i = len - 1; i >= 0; i--)
    { // ������ �� ���� ������ �����, ���� ���� �� ���� ������, �� ����
        if (coef[i] > bn.coef[i]) {
            return true;
        }
        if (coef[i] < bn.coef[i])
            return false;
    }
    return false;
}
// ���������� ��������� ������ ��� �����
bool Big_Number::operator>=(const Big_Number& bn)
{
    if (this == &bn || this->len > bn.len) // ���� ������������ ������ � �����
        // ����� ��� ����� ������, �� ������
        return true;
    if (this->len < bn.len) // ���� ����� ��������  ������, �� ����
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
// ���������� ��������� <
bool Big_Number::operator<(const Big_Number& bn)
{
    if (this == &bn || this->len > bn.len) // ���� ����� �������� ������ ��� ������������ ���� � ��� �� ������
        return false;
    if (this->len < bn.len) // ���� ����� �������� ������
        return true;
    for (int i = len - 1; i >= 0; i--) // ���������� �� ���� ������ � ������ (� coef ���������� ��������)
    {
        if (coef[i] > bn.coef[i]) {
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
// ���������� ��������� <=
bool Big_Number::operator<=(const Big_Number& bn)
{
    if (this == &bn || this->len < bn.len) // ���� ������������ ������ � �����
        // ����� ��� ����� ������, �� ������
        return true;
    if (this->len > bn.len) // ���� ����� ��������  ������, �� ����
        return false;
    for (int i = len - 1; i >= 0; i--) // ���������� �� ���� ������ � ������ (� coef ���������� ��������)
    {
        if (coef[i] > bn.coef[i]) // ���� ���� �� ���� ������, �� ����
            return false;
        if (coef[i] < bn.coef[i])
            return true;
    }
    return true;
}

// ���� �������� ����� � 16-�� ����
// ���� �������� ����� � 16-�� ����
void Big_Number::input_hex()
{
    bool flag = false;
    do
    {
        flag = false;
        char buf[128]; // ����� ��� ������ - �����
        std::cin >> buf; // ���� ������
        int buf_len = strlen(buf); // ����� ��������� ������
        len = (buf_len - 1) / (BASE_SIZE / 4) + 1; // ������� ����� �����
        delete[] coef; // ������� ������
        coef = new BASE[len]; // ��������� ������ ��� ������������
        // ������������� ������������� ������
        for (int i = 0; i < len; ++i)
            coef[i] = 0;

        int coef_index = 0; // ������ �������� ������������
        int shift = 0; // �������� � ������� ������������

        // ��������� �������� � �������� �������
        for (int i = buf_len - 1; i >= 0; --i)
        {
            char ch = buf[i];
            int value = 0;
            if ('0' <= ch && ch <= '9') // �������������� ����� � �����
                value = ch - '0';
            else if ('a' <= ch && ch <= 'f') // �������������� ����������������� ����� � �����
                value = ch - 'a' + 10;
            else if ('A' <= ch && ch <= 'F') // �������������� ����������������� ����� � �����
                value = ch - 'A' + 10;
            else // ������������ ������
            {
                cout << "Invalid error. Enter a number: ";
                flag = true;
                break;
            }

            // ������ �������� � ������� �����������
            coef[coef_index] |= value << shift;

            // ������� � ���������� ������������ ��� ��������
            shift += 4;
            if (shift >= BASE_SIZE) // ���� ��������� ����� �������� ������������
            {
                shift = 0;
                coef_index++;
            }
        }

    } while (flag);
    int i = len - 1;
    while (i > 0 && coef[i] == 0)
        len--;
}


// ����� � 16-�� �������
void Big_Number::output_hex()
{
    bool non_zero_found = false; // ����, �����������, ���� �� ������� ��������� �����

    int k = BASE_SIZE - 4; // ��� �������� ������������
    int j = len - 1;

    while (j >= 0)
    {
        unsigned char tmp = (coef[j] >> k) & 0xf;

        if (tmp != 0 || non_zero_found) {
            // ���� ������� ����� �� ���� ��� ��� ���� ������� ��������� �����,
            // �� ������� ������� �����
            non_zero_found = true; // ������������� ����
            if (0 <= tmp && tmp <= 9)
                std::cout << (char)(tmp + '0');
            else if (10 <= tmp && tmp <= 15)
                std::cout << (char)(tmp - 10 + 'a');
        }

        k -= 4;

        if (k < 0) // ���� �������� ����� �������������, �������� ��� � ������� � ����������� ������������
        {
            k = BASE_SIZE - 4;
            j--;
        }
    }
    if (!non_zero_found) // ���� ��������� ����� �� ���� �������, ������ ����� ����, ������� "0"
        std::cout << '0';

    std::cout << std::endl;
}

// ���������� ��������� �������� +
Big_Number Big_Number::operator+(const Big_Number& bn) {
    // ����������� ���� �����
    int n = this->len;
    int m = bn.len;
    int max_len = std::max(n, m) + 1; // ��������� �������������� ������ ��� ��������

    // �������� ������� ��� ����������
    Big_Number result(max_len);

    // ������������� ���������� ��� ��������
    DBASE carry = 0;

    // �������� ��������
    for (int j = 0; j < max_len; ++j) {
        // ��������� ������� ���� �� ���� ����� � ��� ������������� ��������
        DBASE uj = (j < n ? this->coef[j] : 0);
        DBASE vj = (j < m ? bn.coef[j] : 0);

        // �������� ������� ���� � ������ ��������
        DBASE tmp = uj + vj + carry;

        // ������������ ���������� �������� �������
        result.coef[j] = tmp & std::numeric_limits<BASE>::max();

        // ���������� �������� ��������
        carry = tmp >> BASE_SIZE;
    }

    // ������������� ����� ����������
    result.len = max_len;

    return result;
}

// ���������� ��������� +=
Big_Number& Big_Number::operator+=(const Big_Number& bn) {
    // ����������� ���� �����
    int n = this->len;
    int m = bn.len;
    int max_len = std::max(n, m) + 1; // ��������� �������������� ������ ��� ��������

    // ������� ��������� ������ ��� �������� ���������� ��������
    BASE* temp = new BASE[max_len];

    // ������������� ���������� ��� ��������
    DBASE carry = 0;

    // �������� ��������
    for (int j = 0; j < max_len; ++j) {
        // ��������� ������� ���� �� ���� ����� � ��� ������������� ��������
        DBASE uj = (j < n ? this->coef[j] : 0);
        DBASE vj = (j < m ? bn.coef[j] : 0);

        // �������� ������� ���� � ������ ��������
        DBASE tmp = uj + vj + carry;

        // ������������ ���������� �������� �������
        temp[j] = tmp & std::numeric_limits<BASE>::max();

        // ���������� �������� ��������
        carry = tmp >> BASE_SIZE;
    }

    // ���� ��� ������� �� ������� ������������ �����, ����������� ����� �������
    if (carry > 0 && this->len < max_len)
        max_len = max_len;

    // ������� ������� ����, ���� ��� ����
    while (max_len > 1 && temp[max_len - 1] == 0)
        max_len--;

    // ������� ������ ������ � �������� ����� ���������
    delete[] this->coef;
    this->coef = new BASE[max_len];
    this->len = max_len;
    std::copy(temp, temp + max_len, this->coef);

    // ����������� ��������� ������
    delete[] temp;

    return *this;
}

// ���������� ��������� ��������� -
Big_Number Big_Number::operator-(const Big_Number& bn) {
    int max_len = std::max(len, bn.len); // ������������ ����� ����������
    Big_Number result(max_len); // �������� ������� ��� ����������

    DBASE borrow = 0; // ������������� ���������� ��� �����

    for (int j = 0; j < max_len; ++j) {
        DBASE uj = (j < len ? coef[j] : 0); // ��������� ������� ����� �� ������� ����� ��� ����, ���� ������ ��� ����������
        DBASE vj = (j < bn.len ? bn.coef[j] : 0); // ��������� ������� ����� �� ������� ����� ��� ����, ���� ������ ��� ����������

        DBASE tmp = uj - vj - borrow; // ��������� ������� ���� � ������ �����

        if (tmp < 0) {
            tmp += (1 << BASE_SIZE); // ���� ��������� �������������, ��������� ����
            borrow = 1; // ������������� ����
        }
        else {
            borrow = 0; // ���������� ����
        }

        result.coef[j] = BASE(tmp); // ����������� ��������� �������� �������
    }

    // ������� ������� ����, ���� ��� ����
    while (max_len > 1 && result.coef[max_len - 1] == 0)
        max_len--;

    result.len = max_len; // ������������� ����� ����������

    return result;
}

// ���������� ��������� -=
Big_Number& Big_Number::operator-=(const Big_Number& bn) {
    int max_len = std::max(len, bn.len); // ������������ ����� ����������

    // ������� ��������� ������ ��� �������� ���������� ���������
    BASE* temp = new BASE[max_len];

    DBASE borrow = 0; // ������������� ���������� ��� �����

    for (int j = 0; j < max_len; ++j) {
        DBASE uj = (j < len ? coef[j] : 0); // ��������� ������� ����� �� ������� ����� ��� ����, ���� ������ ��� ����������
        DBASE vj = (j < bn.len ? bn.coef[j] : 0); // ��������� ������� ����� �� ������� ����� ��� ����, ���� ������ ��� ����������

        DBASE tmp = uj - vj - borrow; // ��������� ������� ���� � ������ �����

        if (tmp < 0) {
            tmp += (1 << BASE_SIZE); // ���� ��������� �������������, ��������� ����
            borrow = 1; // ������������� ����
        }
        else {
            borrow = 0; // ���������� ����
        }

        temp[j] = BASE(tmp); // ����������� ��������� �������� �������
    }

    // ������� ������� ����, ���� ��� ����
    while (max_len > 1 && temp[max_len - 1] == 0)
        max_len--;

    // ������� ������ ������ � �������� ����� ���������
    delete[] coef;
    coef = new BASE[max_len];
    len = max_len;
    std::copy(temp, temp + max_len, coef);

    // ����������� ��������� ������
    delete[] temp;

    return *this;
}
Big_Number Big_Number::operator*(BASE number) {
    Big_Number result(len + 1); // ������� ������ ��� ���������� ���������

    DBASE tmp; // ���������� ��� ���������� �������� �������������� ����������
    int j = 0;
    int k = 0;

    while (j < len) {
        tmp = coef[j] * number + k; // �������� ������� ����� �� ����� � ���������� ���������� �������
        result.coef[j] = (BASE)tmp; // ���������� ������� ������� � ������� ������� ����������
        k = (BASE)(tmp >> BASE_SIZE); // ��������� ����� �������
        j++;
    }

    // ���������� ���������� ������� � ��������� ������ ����������
    result.coef[j] = k;

    // ��������� ����� ����������
    result.len = j + 1;

    return result;
}

Big_Number& Big_Number::operator*=(BASE number) {
    DBASE tmp; // ���������� ��� ���������� �������� �������������� ����������
    int j = 0;
    int k = 0;

    while (j < len) {
        tmp = coef[j] * number + k; // �������� ������� ����� �� ����� � ���������� ���������� �������
        coef[j] = (BASE)tmp; // ���������� ������� ������� � ������� �������
        k = (BASE)(tmp >> BASE_SIZE); // ��������� ����� �������
        j++;
    }

    // ���� ������� ������� ����� �����, ��������� ��� ��� ����� ������
    if (k > 0) {
        // ���� ������� ����� ������������ ��� �������� ������ �������, ����������� ������ �������
        if (len == maxlen) {
            maxlen++;
            BASE* new_coef = new BASE[maxlen];
            std::copy(coef, coef + len, new_coef);
            delete[] coef;
            coef = new_coef;
        }
        coef[j] = k;
        len++; // ����������� ����� �����
    }

    return *this;
}


Big_Number Big_Number::operator*(const Big_Number& bn) {
    int m = len;
    int n = bn.len;
    int max_len = m + n; // ������������ ����� ����������

    // ������� ������ ��� ���������� ���������
    Big_Number result(max_len);

    // ������������� ��������� �������� ������������� ���������� � ����
    for (int i = 0; i < max_len; ++i)
        result.coef[i] = 0;

    // ��������� �������� ����� �� ������� �����
    for (int j = 0; j < n; ++j) {
        if (bn.coef[j] == 0)
            continue; // ���������� ��������� �� ������� �����������

        BASE k = 0; // �������

        for (int i = 0; i < m; ++i) {
            DBASE tmp = result.coef[i + j] + (DBASE)coef[i] * bn.coef[j] + k;
            result.coef[i + j] = (BASE)tmp; // ������� �������
            k = (BASE)(tmp >> BASE_SIZE); // �������
        }

        // ��������� �������� �������
        result.coef[j + m] = k;
    }

    // ���������� ����� ����������
    result.len = max_len;

    // �������� ������� �����, ���� ��� ����
    while (result.len > 1 && result.coef[result.len - 1] == 0)
        result.len--;

    return result;
}

Big_Number& Big_Number::operator*=(const Big_Number& bn) {
    int m = len;
    int n = bn.len;
    int max_len = m + n; // ������������ ����� ����������

    // ������� ��������� ������ ��� �������� ���������� ���������
    BASE* temp = new BASE[max_len];
    std::fill(temp, temp + max_len, 0); // ������������� ��� �������� � 0

    // ��������� �������� ����� �� ������� �����
    for (int j = 0; j < n; ++j) {
        if (bn.coef[j] == 0)
            continue; // ���������� ��������� �� ������� �����������

        BASE k = 0; // �������

        for (int i = 0; i < m; ++i) {
            DBASE tmp = temp[i + j] + (DBASE)coef[i] * bn.coef[j] + k;
            temp[i + j] = (BASE)tmp; // ������� �������
            k = (BASE)(tmp >> BASE_SIZE); // �������
        }

        // ��������� �������� �������
        temp[j + m] = k;
    }

    // ������� ������ ������ � �������� ����� ���������
    delete[] coef;
    coef = new BASE[max_len];
    std::copy(temp, temp + max_len, coef);

    // ���������� ����� ����������
    len = max_len;

    // �������� ������� �����, ���� ��� ����
    while (len > 1 && coef[len - 1] == 0)
        len--;

    // ����������� ��������� ������
    delete[] temp;

    return *this;
}

Big_Number Big_Number::operator/(const BASE v) {
    if (v == 0) {
        // � ������ ������� �� ����, ������� ������� ������� � �������
        return Big_Number(1); // ���������� ����
    }

    int n = len;
    Big_Number q(n); // ������� ������� ����� n

    DBASE r = 0; // �������������� ������� �����

    for (int j = n - 1; j >= 0; j--) {
        // ��������� �������, ������� ��� �� ������ ���� � �������� ��������� �����
        DBASE tmp = (r << BASE_SIZE) + coef[j];

        // ��������� ������� � ������� �� ������� tmp �� v
        q.coef[j] = BASE(tmp / v);
        r = tmp % v;
    }

    // ������� ������� ���� � �������, ���� ��� ����
    while (q.len > 1 && q.coef[q.len - 1] == 0)
        q.len--;

    return q;
}

Big_Number Big_Number::operator%(const BASE v) {
    if (v == 0) {
        // � ������ ������� �� ����, ������� ������� �������
        return Big_Number(1); // ���������� ����
    }

    DBASE r = 0; // �������������� ������� �����

    for (int j = len - 1; j >= 0; j--) {
        // ��������� �������, ������� ��� �� ������ ���� � �������� ��������� �����
        DBASE tmp = (r << BASE_SIZE) + coef[j];

        // ��������� ������� �� ������� tmp �� v
        r = tmp % v;
    }

    // ������� ����� ������ Big_Number ��� ������������� �������
    Big_Number remainder(1);
    remainder.coef[0] = BASE(r);
    remainder.len = 1;

    return remainder;
}

Big_Number& Big_Number::operator/=(const BASE v) {
    if (v == 0) {
        // � ������ ������� �� ����, ��������� ������ ��� ���������
        return *this;
    }

    DBASE r = 0; // �������������� ������� �����

    for (int j = len - 1; j >= 0; j--) {
        // ��������� �������, ������� ��� �� ������ ���� � �������� ��������� �����
        DBASE tmp = (r << BASE_SIZE) + coef[j];

        // ��������� ������� � ������� �� ������� tmp �� v
        coef[j] = BASE(tmp / v);
        r = tmp % v;
    }

    // ������� ������� ����, ���� ��� ����
    while (len > 1 && coef[len - 1] == 0)
        len--;

    return *this;
}

Big_Number& Big_Number::operator%=(const BASE v) {
    if (v == 0) {
        // � ������ ������� �� ����, ��������� ������ ��� ���������
        return *this;
    }

    DBASE r = 0; // �������������� ������� �����

    for (int j = len - 1; j >= 0; j--) {
        // ��������� �������, ������� ��� �� ������ ���� � �������� ��������� �����
        DBASE tmp = (r << BASE_SIZE) + coef[j];

        // ��������� ������� �� ������� tmp �� v
        r = tmp % v;
    }

    // ��������� ������, ����� �� �������� ������ �������
    len = 1;
    coef[0] = BASE(r);

    return *this;
}

std::istream& operator>>(std::istream& input, Big_Number& bn) {
    std::string S;
    input >> S; // ��������� ������ S

    int c;
    input >> c; // ��������� ��������� ������� ���������

    bn = Big_Number(); // �������� ������� �����
    Big_Number temp;
    for (char ch : S) {
        BASE t;
        if ('0' <= ch && ch <= '9') // �������������� ����� � �����
            t = ch - '0';
        cout << t << endl;
        if (t >= c) {
            // � ������, ���� ����� ������ ��������� ������� ���������, ������� ���������� ��� ������������ ������
            break;
        }
        temp.coef[0] = t;
        bn *= BASE(c); // �������� ������� ����� �� ��������� ������� ���������
        bn += temp; // ��������� ����� ����� � �������� �����

    }

    return input;
}

std::ostream& operator<<(std::ostream& output, const Big_Number& bn) {
    // �������� �� ����
    if (bn.len == 1 && bn.coef[0] == 0) {
        output << "0";
        return output;
    }

    // ������� ��������� ������� ����� ��� �������� �������
    Big_Number temp(bn);
    std::string result;

    // ���� ������� ����� �� ����� ����
    while (!(temp.len == 1 && temp.coef[0] == 0)) {
        // ��������� ������� �� ������� �� ��������� ������� ���������
        BASE remainder = 0;
        for (int i = temp.len - 1; i >= 0; --i) {
            DBASE current = DBASE(remainder) * BASE(256) + temp.coef[i];
            temp.coef[i] = BASE(current / BASE(10));
            remainder = BASE(current % BASE(10));
        }

        // ����������� ������� � ������ � ��������� ��� � ������ ������ ����������
        result.insert(result.begin(), char(remainder + '0'));
    }

    // ������� ������ ����������
    output << result;

    return output;
}



void Big_Number::trim() {
    while (len > 1 && coef[len - 1] == 0) {
        len--;
    }
}

