#include "Big_Number.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

Big_Number::Big_Number(unsigned long long num, int alloc_size)
{
    if (num == 0) {
        maxlen = alloc_size;
        length = 1;
        digits = new BASE[maxlen]();
        return;
    }

    int needed = 0;
    unsigned long long temp_num = num;
    do {
        needed++;
        temp_num >>= BASE_SIZE;
    } while (temp_num != 0);

    maxlen = std::max(alloc_size, needed);
    digits = new BASE[maxlen]();
    length = needed;

    unsigned long long mask = (1ULL << BASE_SIZE) - 1;
    int i = 0;
    while (num != 0) {
        digits[i++] = static_cast<BASE>(num & mask);
        num >>= BASE_SIZE;
    }
}

Big_Number::Big_Number(const Big_Number &num)
{
    maxlen = num.maxlen;
    length = num.length;
    digits = new BASE[maxlen]();
    for (int i = 0; i < maxlen; i++)
        digits[i] = num.digits[i];
}

Big_Number::~Big_Number()
{
    if (digits)
        delete[] digits;
    digits = NULL;
}

Big_Number Big_Number::operator=(const Big_Number &num)
{
    if (this != &num)
    {
        delete[] digits;
        maxlen = num.maxlen;
        length = num.length;
        digits = new BASE[maxlen]();
        for (int i = 0; i < maxlen; i++)
            digits[i] = num.digits[i];
    }
    return *this;
}

bool Big_Number::operator==(const Big_Number &num) const
{
    if (length != num.length)
    {
        return false;
    }
    for (int i = 0; i < length; i++)
    {
        if (digits[i] != num.digits[i])
        {
            return false;
        }
    }
    return true;
}

bool Big_Number::operator!=(const Big_Number &d) const
{
    if (length != d.length)
    {
        return true;
    }
    for (int i = 0; i < length; i++)
    {
        if (digits[i] != d.digits[i])
        {
            return true;
        }
    }
    return false;
}

bool Big_Number::operator<(const Big_Number &d) const
{
    if (length < d.length)
    {
        return true;
    }
    else if (length > d.length)
    {
        return false;
    }
    for (int i = length - 1; i > -1; i--)
    {
        if (digits[i] > d.digits[i])
        {
            return false;
        }
        else if (digits[i] < d.digits[i])
        {
            return true;
        }
    }
    return false;
}

bool Big_Number::operator>(const Big_Number &d) const
{
    if (length > d.length)
    {
        return true;
    }
    else if (length < d.length)
    {
        return false;
    }
    for (int i = length - 1; i > -1; i--)
    {
        if (digits[i] < d.digits[i])
        {
            return false;
        }
        else if (digits[i] > d.digits[i])
        {
            return true;
        }
    }
    return false;
}

bool Big_Number::operator>=(const Big_Number &d) const
{
    if (length > d.length)
    {
        return true;
    }
    else if (length < d.length)
    {
        return false;
    }
    for (int i = length - 1; i > -1; i--)
    {
        if (digits[i] < d.digits[i])
        {
            return false;
        }
        else if (digits[i] > d.digits[i])
        {
            return true;
        }
    }
    return true;
}

bool Big_Number::operator<=(const Big_Number &d) const
{
    if (length < d.length)
    {
        return true;
    }
    else if (length > d.length)
    {
        return false;
    }
    for (int i = length - 1; i > -1; i--)
    {
        if (digits[i] > d.digits[i])
        {
            return false;
        }
        else if (digits[i] < d.digits[i])
        {
            return true;
        }
    }
    return true;
}

Big_Number Big_Number::operator+(const Big_Number &bNum) const
{
    int maxLength = std::max(length, bNum.length) + 1;
    int commonLength = std::min(length, bNum.length);

    Big_Number resultNum(0ULL, maxLength);
    DBASE tempSum;

    DBASE carry = 0;
    int index = 0;
    int base = BASE_SIZE;

    while (index < commonLength)
    {
        tempSum = (DBASE)digits[index] + (DBASE)bNum.digits[index] + (DBASE)carry;
        resultNum.digits[index] = (BASE)tempSum;
        carry = (BASE)(tempSum >> base);
        index++;
    }

    while (index < length)
    {
        tempSum = (DBASE)digits[index] + (DBASE)carry;
        resultNum.digits[index] = (BASE)tempSum;
        carry = (BASE)(tempSum >> base);
        index++;
    }

    while (index < bNum.length)
    {
        tempSum = (DBASE)bNum.digits[index] + (DBASE)carry;
        resultNum.digits[index] = (BASE)tempSum;
        carry = (BASE)(tempSum >> base);
        index++;
    }
    resultNum.digits[index] = carry;
    resultNum.length = (carry == 0) ? index : index + 1;

    return resultNum;
}

Big_Number &Big_Number::operator+=(const Big_Number &bNum)
{
    int maxLength = std::max(length, bNum.length) + 1;
    int commonLength = std::min(length, bNum.length);

    BASE *tempDigits = new BASE[maxLength]();

    DBASE carry = 0;
    int index = 0;
    int base = BASE_SIZE;

    while (index < commonLength)
    {
        DBASE tempSum = (DBASE)digits[index] + (DBASE)bNum.digits[index] + (DBASE)carry;
        tempDigits[index] = (BASE)tempSum;
        carry = (BASE)(tempSum >> base);
        index++;
    }

    while (index < length)
    {
        DBASE tempSum = (DBASE)digits[index] + (DBASE)carry;
        tempDigits[index] = (BASE)tempSum;
        carry = (BASE)(tempSum >> base);
        index++;
    }

    while (index < bNum.length)
    {
        DBASE tempSum = (DBASE)bNum.digits[index] + (DBASE)carry;
        tempDigits[index] = (BASE)tempSum;
        carry = (BASE)(tempSum >> base);
        index++;
    }
    tempDigits[index] = carry;

    delete[] digits;
    digits = tempDigits;
    length = (carry == 0) ? index : index + 1;

    return *this;
}

Big_Number Big_Number::operator-(const Big_Number &subtracting) const
{
    if (*this < subtracting) {
        throw invalid_argument("Invalid argument");
    }

    DBASE borrow = 0;
    int base_size = BASE_SIZE;
    DBASE tmp_difference;
    int index = 0;

    Big_Number result(0ULL, length);
    result.length = length;

    while (index < subtracting.length) {
        tmp_difference = ((DBASE)1 << base_size) | (DBASE)digits[index];
        tmp_difference = (DBASE)tmp_difference - (DBASE)subtracting.digits[index] - (DBASE)borrow;

        result.digits[index] = (BASE)tmp_difference;
        borrow = (DBASE)!(tmp_difference >> base_size);

        index++;
    }

    while (index < length) {
        tmp_difference = ((DBASE)1 << base_size) | (DBASE)digits[index];
        tmp_difference -= (DBASE)borrow;

        result.digits[index] = (BASE)tmp_difference;
        borrow = (DBASE)!(tmp_difference >> base_size);

        index++;
    }

    while (result.length > 1 && result.digits[result.length - 1] == 0) {
        result.length--;
    }

    return result;
}

Big_Number& Big_Number::operator-=(const Big_Number &subtracting)
{
    if (*this < subtracting) {
        throw invalid_argument("Invalid argument");
    }

    DBASE borrow = 0;
    int base_size = BASE_SIZE;
    DBASE tmp_difference;
    int index = 0;

    BASE *tempDigits = new BASE[length]();

    while (index < subtracting.length) {
        tmp_difference = ((DBASE)1 << base_size) | (DBASE)digits[index];
        tmp_difference = tmp_difference - subtracting.digits[index] - borrow;

        tempDigits[index] = (BASE)tmp_difference;
        borrow = !(tmp_difference >> base_size);

        index++;
    }

    while (index < length) {
        tmp_difference = ((DBASE)1 << base_size) | (DBASE)digits[index];
        tmp_difference -= borrow;

        tempDigits[index] = (BASE)tmp_difference;
        borrow = !(tmp_difference >> base_size);

        index++;
    }

    delete[] digits;
    digits = tempDigits;
    while (length > 1 && digits[length - 1] == 0) {
        length--;
    }

    return *this;
}

Big_Number Big_Number::operator*(const BASE &multiplier) const {
    int index = 0;
    BASE carry = 0;
    Big_Number result(0ULL, length + 1);
    result.length = length + 1;
    DBASE tmp;
    int base_size = BASE_SIZE;
    
    while (index < length) {
        tmp = (DBASE)digits[index] * (DBASE)multiplier + (DBASE)carry;
        result.digits[index] = (BASE)tmp;
        carry = (BASE)(tmp >> base_size);
        index++;
    }

    result.digits[index] = carry;

    while (result.length > 1 && result.digits[result.length - 1] == 0) {
        result.length--;
    }

    return result;
}

Big_Number& Big_Number::operator*=(const BASE &multiplier) {
    int index = 0;
    BASE carry = 0;
    DBASE tmp;
    int base_size = BASE_SIZE;

    BASE *tempDigits = new BASE[length + 1]();

    while (index < length) {
        tmp = (DBASE)digits[index] * (DBASE)multiplier + (DBASE)carry;
        tempDigits[index] = (BASE)tmp;
        carry = (BASE)(tmp >> base_size);
        index++;
    }

    tempDigits[index] = carry;
    int new_length = length + 1;

    while (new_length > 1 && tempDigits[new_length - 1] == 0) {
        new_length--;
    }

    delete[] digits;
    digits = tempDigits;
    length = new_length;

    return *this;
}

Big_Number Big_Number::operator*(const Big_Number &bNum) const
{
    int j = 0;

    Big_Number newNum(0ULL, length + bNum.length);
    newNum.length = length + bNum.length;
    DBASE tmp;

    int b = BASE_SIZE;

    while (j < bNum.length)
    {
        if (bNum.digits[j] != 0)
        {
            BASE k = 0;
            int i = 0;
            while (i < length)
            {
                tmp = ((DBASE)digits[i] * bNum.digits[j] + (DBASE)newNum.digits[i + j] + (DBASE)k);
                newNum.digits[i + j] = (BASE)tmp;
                k = (BASE)(tmp >> b);
                i++;
            }
            newNum.digits[length + j] = k;
        }
        j++;
    }

    while (newNum.length > 1 && newNum.digits[newNum.length - 1] == 0)
    {
        newNum.length--;
    }

    return newNum;
}

Big_Number& Big_Number::operator*=(const Big_Number &multiplier) {
    BASE *tempDigits = new BASE[length + multiplier.length]();

    DBASE tmp;
    int base_size = BASE_SIZE;

    for (int index_multiplier = 0; index_multiplier < multiplier.length; ++index_multiplier) {
        if (multiplier.digits[index_multiplier] != 0) {
            BASE carry = 0;
            for (int index_current = 0; index_current < length; ++index_current) {
                tmp = ((DBASE)digits[index_current] * multiplier.digits[index_multiplier] + (DBASE)tempDigits[index_current + index_multiplier] + (DBASE)carry);
                tempDigits[index_current + index_multiplier] = (BASE)tmp;
                carry = (BASE)(tmp >> base_size);
            }
            tempDigits[length + index_multiplier] = carry;
        }
    }

    delete[] digits;
    length = length + multiplier.length;
    digits = tempDigits;

    while (length > 1 && digits[length - 1] == 0) {
        length--;
    }

    return *this;
}

Big_Number Big_Number::operator/(const BASE &divisor) const {
    if (divisor == 0)
        throw std::runtime_error("Division by zero");
    int index = 0;
    DBASE remainder = 0;
    Big_Number result(0ULL, length);
    result.length = length;

    int base_size = BASE_SIZE;

    while (index < length) {
        DBASE tmp = (((DBASE)remainder << base_size) + (DBASE)digits[length - 1 - index]);
        result.digits[length - 1 - index] = (BASE)(tmp / (DBASE)divisor);
        remainder = (BASE)(tmp % (DBASE)divisor);
        index++;
    }

    while (result.length > 1 && result.digits[result.length - 1] == 0) {
        result.length--;
    }

    return result;
}

Big_Number Big_Number::operator%(const BASE &divisor) const {
    if (divisor == 0)
        throw std::runtime_error("Division by zero");
    int index = 0;
    DBASE remainder = 0;
    int base_size = BASE_SIZE;

    while (index < length) {
        DBASE tmp = (((DBASE)remainder << base_size) + (DBASE)digits[length - 1 - index]);
        remainder = (BASE)(tmp % (DBASE)divisor);
        index++;
    }

    Big_Number result(0ULL, 1);
    result.length = 1;
    result.digits[0] = static_cast<BASE>(remainder);

    return result;
}

Big_Number& Big_Number::operator/=(const BASE &divisor) {
    if (divisor == 0)
        throw std::runtime_error("Division by zero");
    int index = 0;
    DBASE remainder = 0;
    int base_size = BASE_SIZE;

    BASE *tempDigits = new BASE[length]();

    while (index < length) {
        DBASE tmp = (((DBASE)remainder << base_size) + (DBASE)digits[length - 1 - index]);
        tempDigits[length - 1 - index] = (BASE)(tmp / (DBASE)divisor);
        remainder = (BASE)(tmp % (DBASE)divisor);
        index++;
    }

    delete[] digits;
    digits = tempDigits;

    while (length > 1 && digits[length - 1] == 0) {
        length--;
    }

    return *this;
}

Big_Number& Big_Number::operator%=(const BASE &divisor) {
    if (divisor == 0)
        throw std::runtime_error("Division by zero");
    int index = 0;
    DBASE remainder = 0;
    int base_size = BASE_SIZE;

    while (index < length) {
        DBASE tmp = (((DBASE)remainder << base_size) + (DBASE)digits[length - 1 - index]);
        remainder = (BASE)(tmp % (DBASE)divisor);
        index++;
    }
    delete[] digits;
    digits = new BASE[1]();
    length = 1;
    digits[0] = static_cast<BASE>(remainder);

    return *this;
}

void Big_Number::cout_10()
{
    Big_Number newNum = *this;
    Big_Number zero(0ULL);
    string s;
    while (newNum != zero)
    {
        Big_Number t = newNum % 10;
        s.push_back(t.digits[0] + '0');
        newNum = newNum / 10;
    }
    if (s.empty()) {
        cout << "Base10= 0" << endl;
        return;
    }
    reverse(s.begin(), s.end());
    cout << "Base10= " << s << endl;
}

void Big_Number::cin_10()
{
    string s;
    cout << "Base10: ";
    getline(cin, s);

    size_t start = s.find_first_not_of(" \t");
    if (start == string::npos) {
        *this = Big_Number(0ULL);
        return;
    }
    size_t end = s.find_last_not_of(" \t");
    s = s.substr(start, end - start + 1);

    int k = s.length();
    Big_Number bNum(0ULL);
    for (int j = 0; j < k; ++j)
    {
        if ('0' > s[j] || s[j] > '9')
        {
            throw invalid_argument("Invalid arguments");
        }
        BASE t = s[j] - '0';
        bNum = bNum * (BASE)10 + Big_Number((unsigned long long)t);
    }

    while (bNum.length > 1 && bNum.digits[bNum.length - 1] == 0)
    {
        bNum.length--;
    }
    *this = bNum;
}

Big_Number Big_Number::operator/(const Big_Number &num) const
{
    if (num.length == 1 && num.digits[0] == 0)
    {
        throw invalid_argument("Invalid arguments1.");
    }
    if (*this < num)
    {
        return Big_Number(0ULL);
    }

    if (num.length == 1)
    {
        return *this / num.digits[0];
    }

    int m = length - num.length;
    int base_size = BASE_SIZE;
    DBASE b = ((DBASE)1 << base_size);
    DBASE d_val = b / (DBASE)(num.digits[num.length - 1] + (BASE)1);
    BASE d = static_cast<BASE>(d_val);
    int j = m;

    Big_Number newNum = *this;
    newNum *= d;
    Big_Number delNum = num;
    delNum *= d;

    Big_Number finNum(0ULL, m + 1);
    finNum.length = m + 1;

    if (newNum.length == length)
    {
        newNum.maxlen++;
        newNum.length = newNum.maxlen;
        delete[] newNum.digits;
        newNum.digits = new BASE[newNum.maxlen]();
        for (int i = 0; i < length; i++)
        {
            newNum.digits[i] = digits[i];
        }
        newNum *= d;
        newNum.length++;
        newNum.digits[newNum.length - 1] = 0;
    }

    while (j > -1)
    { 
        DBASE q = (DBASE)((DBASE)((DBASE)((DBASE)(newNum.digits[j + delNum.length]) * (DBASE)(b)) + (DBASE)(newNum.digits[j + delNum.length - 1])) / (DBASE)(delNum.digits[delNum.length - 1]));
        DBASE r = (DBASE)((DBASE)((DBASE)((DBASE)(newNum.digits[j + delNum.length]) * (DBASE)(b)) + (DBASE)(newNum.digits[j + delNum.length - 1])) % (DBASE)(delNum.digits[delNum.length - 1]));

        if (q == b || (DBASE)((DBASE)(q) * (DBASE)(delNum.digits[delNum.length - 2])) > (DBASE)((DBASE)((DBASE)(b) * (DBASE)(r)) + (DBASE)(newNum.digits[j + delNum.length - 2])))
        {
            q--;
            r = (DBASE)(r) + (DBASE)(delNum.digits[delNum.length - 1]);
            if ((DBASE)(r) < b)
            {
                if (q == b || (DBASE)((DBASE)(q) * (DBASE)(delNum.digits[delNum.length - 2])) > (DBASE)((DBASE)((DBASE)(b) * (DBASE)(r)) + (DBASE)(newNum.digits[j + delNum.length - 2])))
                {
                    q--;
                }
            }
        }

        Big_Number u(0ULL, delNum.length + 1);
        u.length = delNum.length + 1;
        for (int i = 0; i < delNum.length + 1; i++)
        {
            u.digits[i] = newNum.digits[j + i]; 
        }

        if (u < delNum * static_cast<BASE>(q))
        { 
            q--;
        }

        u = u - delNum * static_cast<BASE>(q); 
        finNum.digits[j] = static_cast<BASE>(q);

        for (int i = 0; i < delNum.length + 1; i++)
        {
            newNum.digits[j + i] = u.digits[i];
        }

        j--;
    }

    while (finNum.length > 1 && finNum.digits[finNum.length - 1] == 0)
    {
        finNum.length--;
    }

    return finNum;
}

Big_Number Big_Number::operator%(const Big_Number &num) const
{
    if (num.length == 1 && num.digits[0] == 0)
    {
        throw invalid_argument("Invalid arguments.");
    }
    if (*this < num)
    {
        return *this;
    }

    if (num.length == 1)
    {
        return *this % num.digits[0];
    }

    int m = length - num.length;
    int base_size = BASE_SIZE;
    DBASE b = ((DBASE)(1) << (base_size));
    DBASE d_val = ((DBASE)(b) / (DBASE)((num.digits[num.length - 1]) + (BASE)(1)));
    BASE d = static_cast<BASE>(d_val);
    int j = m;

    Big_Number newNum = *this;
    newNum *= d;
    Big_Number delNum = num;
    delNum *= d;
    if (newNum.length == length)
    {
        newNum.maxlen++;
        newNum.length = newNum.maxlen;
        newNum.digits = new BASE[newNum.maxlen]();
        for (int i = 0; i < length; i++)
        {
            newNum.digits[i] = digits[i];
        }
        newNum *= d;
        newNum.length++;
        newNum.digits[newNum.length - 1] = 0;
    }

    while (j > -1)
    {
        DBASE q = (DBASE)((DBASE)((DBASE)((DBASE)(newNum.digits[j + delNum.length]) * (DBASE)(b)) +
                     (DBASE)(newNum.digits[j + delNum.length - 1])) / (DBASE)(delNum.digits[delNum.length - 1]));
        DBASE r = (DBASE)((DBASE)((DBASE)((DBASE)(newNum.digits[j + delNum.length]) * (DBASE)(b)) +
                     (DBASE)(newNum.digits[j + delNum.length - 1])) % (DBASE)(delNum.digits[delNum.length - 1]));
        if (q == b || (DBASE)((DBASE)(q) * (DBASE)(delNum.digits[delNum.length - 2])) >
                          (DBASE)((DBASE)((DBASE)(b) * (DBASE)(r)) + (DBASE)(newNum.digits[j + delNum.length - 2]))) {
            q--;
            r = (DBASE)(r) + (DBASE)(delNum.digits[delNum.length - 1]);
            if ((DBASE)(r) < b) {
                if (q == b || (DBASE)((DBASE)(q) * (DBASE)(delNum.digits[delNum.length - 2])) >
                                  (DBASE)((DBASE)((DBASE)(b) * (DBASE)(r)) + (DBASE)(newNum.digits[j + delNum.length - 2]))) {
                    q--;
                }
            }
        }
        Big_Number u(0ULL, delNum.length + 1);
        u.length = delNum.length + 1;
        for (int i = 0; i < delNum.length + 1; i++)
        {
            u.digits[i] = newNum.digits[j + i];
        }

        if (u < delNum * static_cast<BASE>(q)) {
            q--;
        }

        u = u - (delNum * static_cast<BASE>(q));

        for (int i = 0; i < delNum.length + 1; i++)
        {
            newNum.digits[j + i] = u.digits[i];
        }

        j--;
    }
    
    while (newNum.length > 1 && newNum.digits[newNum.length - 1] == 0)
    {
        newNum.length--;
    }

    return newNum / d;
}

Big_Number& Big_Number::operator/=(const Big_Number &divisor) {
    if (divisor.length == 1 && divisor.digits[0] == 0) {
        throw invalid_argument("Invalid divisor.");
    }
    if (*this < divisor) {
        *this = Big_Number(0ULL);
        return *this;
    }

    if (divisor.length == 1) {
        *this /= divisor.digits[0];
        return *this;
    }

    int result_length = length - divisor.length;
    int base_size = BASE_SIZE;
    DBASE base_val = ((DBASE)1 << base_size);
    DBASE norm_factor_val = base_val / (DBASE)(divisor.digits[divisor.length - 1] + (BASE)1);
    BASE norm_factor = static_cast<BASE>(norm_factor_val);
    int j = result_length;

    Big_Number dividend = *this;
    dividend *= norm_factor;
    Big_Number divisor_normalized = divisor;
    divisor_normalized *= norm_factor;

    Big_Number quotient(0ULL, result_length + 1);
    quotient.length = result_length + 1;

    if (dividend.length == length) {
        dividend.maxlen++;
        dividend.length = dividend.maxlen;
        delete[] dividend.digits;
        dividend.digits = new BASE[dividend.maxlen]();
        for (int i = 0; i < length; i++) {
            dividend.digits[i] = digits[i];
        }
        dividend *= norm_factor;
        dividend.length++;
        dividend.digits[dividend.length - 1] = 0;
    }

    while (j > -1) {
        DBASE q = (DBASE)((DBASE)((DBASE)((DBASE)(dividend.digits[j + divisor_normalized.length]) * (DBASE)(base_val)) +
                     (DBASE)(dividend.digits[j + divisor_normalized.length - 1])) / (DBASE)(divisor_normalized.digits[divisor_normalized.length - 1]));
        DBASE r = (DBASE)((DBASE)((DBASE)((DBASE)(dividend.digits[j + divisor_normalized.length]) * (DBASE)(base_val)) +
                     (DBASE)(dividend.digits[j + divisor_normalized.length - 1])) % (DBASE)(divisor_normalized.digits[divisor_normalized.length - 1]));

        if (q == base_val || (DBASE)((DBASE)(q) * (DBASE)(divisor_normalized.digits[divisor_normalized.length - 2])) >
                          (DBASE)((DBASE)((DBASE)(base_val) * (DBASE)(r)) + (DBASE)(dividend.digits[j + divisor_normalized.length - 2]))) {
            q--;
            r = (DBASE)(r) + (DBASE)(divisor_normalized.digits[divisor_normalized.length - 1]);
            if ((DBASE)(r) < base_val) {
                if (q == base_val || (DBASE)((DBASE)(q) * (DBASE)(divisor_normalized.digits[divisor_normalized.length - 2])) >
                                  (DBASE)((DBASE)((DBASE)(base_val) * (DBASE)(r)) + (DBASE)(dividend.digits[j + divisor_normalized.length - 2]))) {
                    q--;
                }
            }
        }

        Big_Number u(0ULL, divisor_normalized.length + 1);
        u.length = divisor_normalized.length + 1;
        for (int i = 0; i < divisor_normalized.length + 1; i++) {
            u.digits[i] = dividend.digits[j + i];
        }

        if (u < divisor_normalized * static_cast<BASE>(q)) {
            q--;
        }

        u = u - divisor_normalized * static_cast<BASE>(q);
        quotient.digits[j] = static_cast<BASE>(q);

        for (int i = 0; i < divisor_normalized.length + 1; i++) {
            dividend.digits[j + i] = u.digits[i];
        }

        j--;
    }

    while (quotient.length > 1 && quotient.digits[quotient.length - 1] == 0) {
        quotient.length--;
    }

    *this = quotient;
    return *this;
}

Big_Number& Big_Number::operator%=(const Big_Number &divisor) {
    if (divisor.length == 1 && divisor.digits[0] == 0) {
        throw invalid_argument("Invalid divisor.");
    }
    if (*this < divisor) {
        return *this;
    }

    if (divisor.length == 1) {
        *this %= divisor.digits[0];
        return *this;
    }

    int result_length = length - divisor.length;
    int base_size = BASE_SIZE;
    DBASE base_val = ((DBASE)(1) << (base_size));
    DBASE norm_factor_val = ((DBASE)(base_val) / (DBASE)((divisor.digits[divisor.length - 1]) + (BASE)(1)));
    BASE norm_factor = static_cast<BASE>(norm_factor_val);
    int j = result_length;

    Big_Number dividend = *this;
    dividend *= norm_factor;
    Big_Number divisor_normalized = divisor;
    divisor_normalized *= norm_factor;
    if (dividend.length == length) {
        dividend.maxlen++;
        dividend.length = dividend.maxlen;
        dividend.digits = new BASE[dividend.maxlen]();
        for (int i = 0; i < length; i++) {
            dividend.digits[i] = digits[i];
        }
        dividend *= norm_factor;
        dividend.length++;
        dividend.digits[dividend.length - 1] = 0;
    }

    while (j > -1) {
        DBASE q = (DBASE)((DBASE)((DBASE)((DBASE)(dividend.digits[j + divisor_normalized.length]) * (DBASE)(base_val)) +
                     (DBASE)(dividend.digits[j + divisor_normalized.length - 1])) / (DBASE)(divisor_normalized.digits[divisor_normalized.length - 1]));
        DBASE r = (DBASE)((DBASE)((DBASE)((DBASE)(dividend.digits[j + divisor_normalized.length]) * (DBASE)(base_val)) +
                     (DBASE)(dividend.digits[j + divisor_normalized.length - 1])) % (DBASE)(divisor_normalized.digits[divisor_normalized.length - 1]));
        if (q == base_val || (DBASE)((DBASE)(q) * (DBASE)(divisor_normalized.digits[divisor_normalized.length - 2])) >
                          (DBASE)((DBASE)((DBASE)(base_val) * (DBASE)(r)) + (DBASE)(dividend.digits[j + divisor_normalized.length - 2]))) {
            q--;
            r = (DBASE)(r) + (DBASE)(divisor_normalized.digits[divisor_normalized.length - 1]);
            if ((DBASE)(r) < base_val) {
                if (q == base_val || (DBASE)((DBASE)(q) * (DBASE)(divisor_normalized.digits[divisor_normalized.length - 2])) >
                                  (DBASE)((DBASE)((DBASE)(base_val) * (DBASE)(r)) + (DBASE)(dividend.digits[j + divisor_normalized.length - 2]))) {
                    q--;
                }
            }
        }
        Big_Number u(0ULL, divisor_normalized.length + 1);
        u.length = divisor_normalized.length + 1;
        for (int i = 0; i < divisor_normalized.length + 1; i++) {
            u.digits[i] = dividend.digits[j + i];
        }

        if (u < divisor_normalized * static_cast<BASE>(q)) {
            q--;
        }

        u = u - (divisor_normalized * static_cast<BASE>(q));

        for (int i = 0; i < divisor_normalized.length + 1; i++) {
            dividend.digits[j + i] = u.digits[i];
        }

        j--;
    }
     
    while (dividend.length > 1 && dividend.digits[dividend.length - 1] == 0) {
        dividend.length--;
    }

    *this = dividend / norm_factor;
    return *this;
}

istream &operator>>(istream &in, Big_Number &bNum)
{
    char *s = new char[1000];
    int b = BASE_SIZE;
    int j = 0;
    int k = 0;
    unsigned int tmp = 0;

    in.getline(s, 1000);
    bNum.length = (strlen(s) - 1) / (b / 4) + 1;
    bNum.maxlen = bNum.length;
    delete[] bNum.digits;
    bNum.digits = new BASE[bNum.maxlen]();
    for (int i = 0; i < bNum.length; i++)
    {
        bNum.digits[i] = 0;
    }
    for (int i = strlen(s) - 1; i > -1; i--)
    {
        if ('0' <= s[i] && s[i] <= '9')
        {
            tmp = s[i] - '0';
        }
        else if ('a' <= s[i] && s[i] <= 'f')
        {
            tmp = s[i] - 'a' + 10;
        }
        else if ('A' <= s[i] && s[i] <= 'F')
        {
            tmp = s[i] - 'A' + 10;
        }
        else
        {
            throw invalid_argument("Invalid arguments.");
        }

        bNum.digits[j] |= tmp << (k * 4);
        k++;
        if (k >= b / 4)
        {
            k = 0;
            j++;
        }
    }
    while (bNum.length > 1 && bNum.digits[bNum.length - 1] == 0) {
        bNum.length--;
    }
    delete[] s;
    return in;
}

ostream &operator<<(ostream &st, const Big_Number &num)
{
    if (num.length == 0) {
        st << '0' << endl;
        return st;
    }
    int size = BASE_SIZE;
    for (int j = num.length - 1; j >= 0; j--)
    {
        st.width(size / 4);
        st.fill('0');
        st << hex << (int)num.digits[j];
    }
    st << dec;
    return st;
}

Big_Number Big_Number::pow(const Big_Number& exponent) const {
    Big_Number zero(0ULL);
    Big_Number one(1ULL);
    if (exponent < zero) {
        throw invalid_argument("Negative exponent not supported");
    }
    if (*this == zero) {
        return (exponent == zero) ? one : zero;
    }
    if (exponent == zero) {
        return one;
    }
    
    Big_Number result(1ULL);
    Big_Number base = *this;
    Big_Number exp = exponent;
    Big_Number two(2ULL);
    
    while (exp > zero) {
        if ((exp.digits[0] & 1) == 1) {  
            result = result * base;
        }
        base = base * base;
        exp = exp / two;
    }
    
    return result;
}

Big_Number Big_Number::sqrt() const {
    Big_Number zero(0ULL);
    Big_Number one(1ULL);
    if (*this < zero) {
        throw invalid_argument("Square root of negative not supported");
    }
    if (*this == zero || *this == one) {
        return *this;
    }
    
    Big_Number low(1ULL);
    Big_Number high = (*this / Big_Number(2ULL)) + one;
    Big_Number two(2ULL);
    
    while (low <= high) {
        Big_Number mid = (low + high) / two;
        Big_Number sq = mid * mid;
        if (sq == *this) {
            return mid;
        } else if (sq < *this) {
            low = mid + one;
        } else {
            high = mid - one;
        }
    }
    return high;
}


bool Big_Number::isPrime(int iterations) const {

    Big_Number zero(0ULL);
    Big_Number one(1ULL);
    Big_Number two(2ULL);
    Big_Number three(3ULL);
    Big_Number four(4ULL);

    if (*this <= one) return false; 
    if (*this == two || *this == three) return true; 
    if (*this % two == zero) return false; 

   
    Big_Number d = *this - one;
    Big_Number r(0ULL);
    while ((d % two) == zero) {
        d /= two;
        r += one;
    }

    for (int i = 0; i < iterations; ++i) {
        Big_Number a = random_range(two, *this - two);
        
        Big_Number x = a.pow_mod(d, *this);
        if (x == one || x == *this - one) continue; 

        for (Big_Number j(0ULL); j < r - one; j += one) {
            x = (x * x) % *this;
            if (x == *this - one) break;
            if (x == one) return false;
        }
        if (x != *this - one) return false;
    }

    return true; 
}

Big_Number Big_Number::pow_mod(const Big_Number& exponent, const Big_Number& modulus) const {

    Big_Number zero(0ULL);
    Big_Number one(1ULL);
    if (modulus == zero) {
        throw invalid_argument("Modulus cannot be zero");
    }
    if (exponent < zero) {
        throw invalid_argument("Negative exponent not supported");
    }
    
    Big_Number result(1ULL);
    Big_Number base = *this % modulus;
    Big_Number exp = exponent;
    Big_Number two(2ULL);
    
    while (exp > zero) {
        if ((exp.digits[0] & 1) == 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exp = exp / two;
    }
    
    return result;
}

Big_Number Big_Number::random(int bits) {
    if (bits <= 0) return Big_Number(0ULL);
    int num_limbs = (bits + BASE_SIZE - 1) / BASE_SIZE;
    Big_Number rand_num(0ULL, num_limbs);
    rand_num.length = num_limbs;
    for (int i = 0; i < num_limbs; ++i) {
        rand_num.digits[i] = rand();
    }

    rand_num.digits[num_limbs - 1] |= (1ULL << ((bits - 1) % BASE_SIZE));
    return rand_num;
}

Big_Number Big_Number::random_range(const Big_Number& min, const Big_Number& max) {
    if (min > max) {
        throw invalid_argument("Invalid range");
    }
    Big_Number range = max - min;
    Big_Number rand_val = random((max.length * BASE_SIZE));
    rand_val = rand_val % (range + Big_Number(1ULL));
    return min + rand_val;
}

std::vector<Big_Number> Big_Number::factorize() const {
    std::vector<Big_Number> factors;
    Big_Number n = *this;
    Big_Number one(1ULL);
    if (n <= one) return factors;
    Big_Number zero(0ULL);
    Big_Number two(2ULL);

    while (n % two == zero) {
        factors.push_back(two);
        n /= two;
    }
    bool isPrime = n.isPrime();
    if (isPrime) {
        factors.push_back(n);
        return factors;
    }

    Big_Number three(3ULL);
    while (n % three == zero) {
        factors.push_back(three);
        n /= three;
    }

    Big_Number five(5ULL);
    Big_Number d = five;
    int inc = 2;
    while (d * d <= n) {
        while (n % d == zero) {
            factors.push_back(d);
            n /= d;
        }
        d += Big_Number(inc);
        inc = (inc == 2) ? 4 : 2;
    }

    if (n > one) {
        factors.push_back(n);
    }

    return factors;
}