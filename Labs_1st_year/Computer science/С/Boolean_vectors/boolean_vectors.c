#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Функция вывода булевого вектора
void PrintBoolVectorToConsole(unsigned char *vector, size_t bits)
{
    if (vector && bits > 0)
    {
        size_t bytes = ((bits - 1) / 8) + 1; // Количество байт
        for (size_t i = 0; i < bytes; i++)
        {
            unsigned char mask = 1 << 7; // Создаем маску, начиная с самого левого бита
            // Цикл для обработки каждого бита в байте
            for (int j = 0; j < 8; j++)
            {
                if ((vector[i] & mask) != 0)
                    printf("1"); // Выводим "1", если бит установлен
                else
                    printf("0");  // Выводим "0", если бит не установлен
                mask = mask >> 1; // Сдвигаем маску вправо для проверки следующего бита
            }
            printf(" ");
        }
    }
    else
        printf("Can't print vector."); // Если указатель на вектор пуст, выводим сообщение об ошибке
    printf("\n");                      // Переводим строку после вывода битового вектора
}

// Конвертация строки в булев вектор
unsigned char *ConvertStrToVector(char *str)
{
    if (str)
    {
        size_t len = strlen(str); // длина строки
        if (len > 0)
        {
            size_t bytes = ((len - 1) / 8) + 1;                                            // вычисление количества ячеек булева вектора
            unsigned char *vec = (unsigned char *)malloc(sizeof(unsigned char) * (bytes)); // выделение памяти под вектор
            if (vec)
            {
                for (int i = 0; i < bytes; i++)
                    vec[i] = 0;

                int ix = 0;
                unsigned char mask = 1 << 7; // Создаем маску, начиная с самого левого бита

                for (int i = 0; i < bytes; i++)
                {
                    for (int j = 0; j < 8 && (ix < len); j++)
                    {
                        if (str[ix] != '0')
                            vec[i] = vec[i] | mask;

                        mask = mask >> 1; // Смещаем маску
                        ix++;
                    }
                    mask = 1 << 7; // Создаем маску, начиная с самого левого бита; // сбрасываем маску перед переходом к следующему байту
                }
                return vec;
            }
        }
        else
            return NULL;
    }
    return NULL;
}

// Увеличение длины булевого вектора
unsigned char *IncreaseLengthVector(unsigned char *vector, size_t oldBytes, size_t newBytes)
{
    if (vector && oldBytes > 0)
    {
        unsigned char *newVector = (unsigned char *)malloc(sizeof(unsigned char) * newBytes); // выделение памяти под вектор
        if (newVector)
        {
            for (int i = 0; i < newBytes; i++)
            { // Обнуление вектора
                newVector[i] = 0;
            }
            for (int i = 0; i < oldBytes; i++)
            { // Перенос значения на другой вектор
                newVector[i] = vector[i];
            }
            return newVector;
        }
        else
            return NULL;
    }
    else
        return NULL;
}

// Логическое сложение (OR)
unsigned char *LogicOR(unsigned char *vectorBoolA, unsigned char *vectorBoolB, size_t bitsA, size_t bitsB)
{
    if (vectorBoolA && vectorBoolB && bitsA > 0 && bitsB > 0)
    {
        size_t bytesA = ((bitsA - 1) / 8) + 1; // Количество байтов булева вектора A
        size_t bytesB = ((bitsB - 1) / 8) + 1; // Количество байтов булева вектора B
        size_t maxBytes = bytesA;              // Максимальное количество байтов среди векторов
        if (bytesA > bytesB)
        {
            vectorBoolB = IncreaseLengthVector(vectorBoolB, bytesB, bytesA);
            maxBytes = bytesA;
        }
        else if (bytesB > bytesA)
        {
            vectorBoolA = IncreaseLengthVector(vectorBoolA, bytesA, bytesB);
            maxBytes = bytesB;
        }
        unsigned char *newVector = (unsigned char *)malloc(sizeof(unsigned char) * maxBytes);
        if (newVector == NULL)
            return NULL;
        for (size_t i = 0; i < maxBytes; ++i)
        {
            newVector[i] = vectorBoolA[i] | vectorBoolB[i];
        }
        return newVector;
    }
    else
        return NULL;
}

// Логическое умножение (AND)
unsigned char *LogicAND(unsigned char *vectorBoolA, unsigned char *vectorBoolB, size_t bitsA, size_t bitsB)
{
    if (vectorBoolA && vectorBoolB && bitsA > 0 && bitsB > 0)
    {
        size_t bytesA = ((bitsA - 1) / 8) + 1;
        size_t bytesB = ((bitsB - 1) / 8) + 1;
        size_t maxBytes = bytesA;
        if (bytesA > bytesB)
        {
            vectorBoolB = IncreaseLengthVector(vectorBoolB, bytesB, bytesA);
            maxBytes = bytesA;
        }
        else if (bytesB > bytesA)
        {
            vectorBoolA = IncreaseLengthVector(vectorBoolA, bytesA, bytesB);
            maxBytes = bytesB;
        }
        unsigned char *newVector = (unsigned char *)malloc(sizeof(unsigned char) * maxBytes);
        if (newVector == NULL)
            return NULL;
        for (size_t i = 0; i < maxBytes; ++i)
        {
            newVector[i] = vectorBoolA[i] & vectorBoolB[i];
        }
        return newVector;
    }
}

// Сумма по модулю 2 (XOR)
unsigned char *LogicXOR(unsigned char *vectorBoolA, unsigned char *vectorBoolB, size_t bitsA, size_t bitsB)
{
    if (vectorBoolA && vectorBoolB && bitsA > 0 && bitsB > 0)
    {
        size_t bytesA = ((bitsA - 1) / 8) + 1;
        size_t bytesB = ((bitsB - 1) / 8) + 1;
        size_t maxBytes = bytesA;
        if (bytesA > bytesB)
        {
            vectorBoolB = IncreaseLengthVector(vectorBoolB, bytesB, bytesA);
            maxBytes = bytesA;
        }
        else if (bytesB > bytesA)
        {
            vectorBoolA = IncreaseLengthVector(vectorBoolA, bytesA, bytesB);
            maxBytes = bytesB;
        }
        unsigned char *newVector = (unsigned char *)malloc(sizeof(unsigned char) * maxBytes);
        if (newVector == NULL)
            return NULL;
        for (size_t i = 0; i < maxBytes; ++i)
        {
            newVector[i] = vectorBoolA[i] ^ vectorBoolB[i];
        }
        return newVector;
    }
}

// Инверсия булева вектора
void Inversion(unsigned char *vector, size_t bits)
{
    if (vector && bits > 0)
    {
        size_t bytes = ((bits - 1) / 8) + 1; // Количество байт
        size_t shift = 8 * bytes - bits;     // Количество ячеек для сдвига маски
        for (size_t i = 0; i < bytes; i++)
        {
            vector[i] = ~vector[i];
        }
        unsigned char mask = 255;
        mask = mask << shift;
        vector[bytes - 1] = vector[bytes - 1] & mask;
    }
}

// Сдвиг булева вектора вправа
void ShiftRight(unsigned char *vector, size_t bits, size_t shift)
{
    if (vector && bits > 0)
    {
        size_t bytes = ((bits - 1) / 8) + 1; // Количество байтов
        size_t bytesShift = ((shift - 1) / 8) + 1;
        unsigned char carry = 0; // Перенос из одного байта в другой
        for (int i = 0; i < bytes; i++)
        {
            unsigned char currentByte = vector[i]; // Сохраняем текущий байт
            if (i + 1 == bytesShift && bytesShift > 1)
            {
                vector[i] = (currentByte >> shift % 8) | carry; // Смещаем текущий байт и переносим биты с предыдущего байта
                carry = currentByte;                            // Сохраняем биты, которые перешли в следующий байт
            }
            else if (i + 1 > bytesShift && bytesShift > 1)
            {
                vector[i] = carry;
                carry = currentByte; // Сохраняем биты, которые перешли в следующий байт
            }
            else
            {
                vector[i] = (currentByte >> shift) | carry; // Смещаем текущий байт и переносим биты с предыдущего байта
                if (shift > 8)
                {
                    carry = currentByte << 8; // Сохраняем биты, которые перешли
                }
                else
                    carry = currentByte << (8 - shift); // Сохраняем биты, которые перешли
            }
        }
    }
}

// Сдвиг влево
void ShiftLeft(unsigned char *vector, size_t bits, size_t shift)
{
    if (vector && bits > 0)
    {
        size_t bytes = ((bits - 1) / 8) + 1; // Количество байтов
        size_t bytesShift = ((shift - 1) / 8) + 1;
        int carry = 0; // Перенос из одного байта в другой
        for (int i = bytes - 1; i >= 0; i--)
        {
            unsigned char currentByte = vector[i]; // Сохраняем текущий байт
            if (bytes - 1 - i == bytesShift - 1 && bytesShift > 1)
            {
                vector[i] = (currentByte << shift % 8) | carry; // Смещаем текущий байт и переносим биты с предыдущего байта
                carry = currentByte;                            // Сохраняем биты, которые перешли в следующий байт
            }
            else if (i < bytesShift && bytesShift > 1)
            {
                vector[i] = carry;
                carry = currentByte; // Сохраняем биты, которые перешли в следующий байт
            }
            else
            {
                vector[i] = (currentByte << shift) | carry; // Смещаем текущий байт и переносим биты с предыдущего байта
                if (shift > 8)
                {
                    carry = 0; // Сохраняем биты, которые перешли в следующий байт
                }
                else if (bytesShift == 1)
                {
                    carry = currentByte >> (8 - shift); // Сохраняем биты, которые перешли
                }
            }
        }
    }
}

// Установка бита в на определенной позиции
void SetBit(unsigned char *vector, size_t bits, size_t position)
{
    if (vector && bits > 0)
    {
        size_t bytes = ((bits - 1) / 8) + 1;
        if (position < bytes * 8 - 1)
        {
            size_t bytePosition = position / 8;
            size_t bitPosition = 7 - (position % 8);
            unsigned char mask = 1 << bitPosition;
            vector[bytePosition] |= mask;
        }
    }
}

// Удаление бита на определенной позиции
void ClearBit(unsigned char *vector, size_t bits, size_t position)
{
    if (vector && bits > 0)
    {
        size_t bytes = ((bits - 1) / 8) + 1;
        if (position < bytes * 8 - 1)
        {
            size_t bytePosition = position / 8;
            size_t bitPosition = 7 - (position % 8);
            unsigned char mask = ~(1 << bitPosition);
            vector[bytePosition] &= mask;
        }
    }
}

bool isVectorZero(unsigned char *vec, size_t bits)
{
    size_t bytes = ((bits - 1) / 8) + 1;
    if (vec && bits > 0)
    {
        for (size_t i = 0; i < bytes; i++)
        {
            if (vec[i] != 0)
            {
                return false; // Если найден ненулевой бит, вектор не является нулевым
            }
        }
        return true; // Все биты нулевые, вектор является нулевым
    }
    return false; // Некорректный вектор (равен NULL)
}

unsigned char *CopyVector(unsigned char *vector, size_t bits)
{
    if (vector && bits > 0)
    {
        size_t bytes = ((bits - 1) / 8) + 1;
        unsigned char *newVector = (unsigned char *)malloc(sizeof(unsigned char) * bytes);
        if (newVector)
        {
            for (size_t i = 0; i < bytes; i++)
                newVector[i] = vector[i];
            return newVector;
        }
        else
            return NULL;
    }
    else
        return NULL;
}

void main()
{
    char strVectorA[200]; // Булев вектор А
    printf("Enter a vector A: ");
    fgets(strVectorA, sizeof(strVectorA), stdin); // Ввод текста

    char strVectorB[200]; // Булев вектор В
    printf("Enter a vector B: ");
    fgets(strVectorB, sizeof(strVectorB), stdin); // Ввод текста

    // Удаление символа перехода на новую строку
    strVectorA[strcspn(strVectorA, "\n")] = '\0';
    strVectorB[strcspn(strVectorB, "\n")] = '\0';

    size_t bitsA = strlen(strVectorA); // Количество битов в булевом векторе А
    size_t bitsB = strlen(strVectorB); // Количество битов в булевом векторе В

    unsigned char *vectorBoolA = ConvertStrToVector(strVectorA);
    unsigned char *vectorBoolB = ConvertStrToVector(strVectorB);
    char newline;

    size_t maxBits = 0; // Максимальное количество битов из 2х векторов
    if (bitsA > bitsB)
    {
        maxBits = bitsA;
    }
    else
        maxBits = bitsB;

    if (vectorBoolA && vectorBoolB)
    {
        // Логиеческое сложение
        unsigned char *resultOR = LogicOR(vectorBoolA, vectorBoolB, bitsA, bitsB);
        if (resultOR == NULL)
        {
            printf("Memory allocation error\n");
        }
        else
        {
            printf("Result OR: ");
            PrintBoolVectorToConsole(resultOR, maxBits);
        }
        free(resultOR);

        // Логическое умножение
        unsigned char *resultAND = LogicAND(vectorBoolA, vectorBoolB, bitsA, bitsB);
        if (resultAND == NULL)
        {
            printf("Memory allocation error\n");
        }
        else
        {
            printf("Result AND: ");
            PrintBoolVectorToConsole(resultAND, maxBits);
        }
        free(resultAND);

        // Исключающее или
        unsigned char *resultXOR = LogicXOR(vectorBoolA, vectorBoolB, bitsA, bitsB);
        if (resultXOR == NULL)
        {
            printf("Memory allocation error\n");
        }
        else
        {
            printf("Result XOR: ");
            PrintBoolVectorToConsole(resultXOR, maxBits);
        }
        free(resultXOR);

        // Сдвиг вправа вектора А
        size_t shiftRightVector = 0;
        printf("Enter the number of digits for the motor: ");
        if (!scanf("%zu", &shiftRightVector))
        {
            printf("Input Error\n");
            while ((newline = getchar()) != '\n' && newline != EOF)
            {
                // Просто игнорируем символы
            }
        }
        else
        {
            unsigned char *copyVectorAShiftRight = CopyVector(vectorBoolA, bitsA);
            if (copyVectorAShiftRight)
            {
                ShiftRight(copyVectorAShiftRight, bitsA, shiftRightVector);
                printf("Result Shift right: ");
                PrintBoolVectorToConsole(copyVectorAShiftRight, bitsA);
                free(copyVectorAShiftRight);
            }
        }

        // Сдвиг влево вектора B
        size_t shiftLeftVector = 0;
        printf("Enter the number of digits for the motor: ");
        if (!scanf("%zu", &shiftLeftVector))
        {
            printf("Input Error\n");
            while ((newline = getchar()) != '\n' && newline != EOF)
            {
                // Просто игнорируем символы
            }
        }
        else
        {
            unsigned char *copyVectorBShiftLeft = CopyVector(vectorBoolB, bitsB);
            if (copyVectorBShiftLeft)
            {
                ShiftLeft(copyVectorBShiftLeft, bitsB, shiftLeftVector);
                printf("Result Shift left: ");
                PrintBoolVectorToConsole(copyVectorBShiftLeft, bitsB);
                free(copyVectorBShiftLeft);
            }
        }

        // Установка бита в булевом векторе
        size_t bitSet;
        printf("Enter position to set bit in Vector A: ");
        if (scanf("%zu", &bitSet))
        {
            unsigned char *copyVectorASet = CopyVector(vectorBoolA, bitsA);
            if (copyVectorASet)
            {
                printf("Result set bit strVectorA: ");
                SetBit(copyVectorASet, bitsA, bitSet);
                PrintBoolVectorToConsole(copyVectorASet, bitsA);
            }
            free(copyVectorASet);
        }
        else
        {
            printf("Input Error.\n");
            while ((newline = getchar()) != '\n' && newline != EOF)
            {
                // Просто игнорируем символы
            }
        }

        // Удаление бита в булевом векторе
        size_t bitClear;
        printf("Enter position to clear bit in Vector A: ");
        if (scanf("%zu", &bitClear))
        {
            unsigned char *copyVectorAClear = CopyVector(vectorBoolA, bitsA);
            if (copyVectorAClear)
            {
                printf("Result clear bit Vector A: ");
                ClearBit(copyVectorAClear, bitsA, bitClear);
                PrintBoolVectorToConsole(copyVectorAClear, bitsA);
            }
            free(copyVectorAClear);
        }
        else
        {
            printf("Input Error.\n");
            while ((newline = getchar()) != '\n' && newline != EOF)
            {
                // Просто игнорируем символы
            }
        }
        // Инвертирование вектора
        printf("Result Inversion last vector: ");
        unsigned char *resultInversionVectorA = CopyVector(vectorBoolA, bitsA);
        if (resultInversionVectorA)
        {
            Inversion(resultInversionVectorA, bitsA);
            PrintBoolVectorToConsole(resultInversionVectorA, bitsA);
            free(resultInversionVectorA);
        }

        unsigned char *copyVectorAToZero = CopyVector(vectorBoolA, bitsA);
        if (copyVectorAToZero)
        {
            printf("Resurs shift:\n");
            PrintBoolVectorToConsole(copyVectorAToZero, bitsA);
            while (!isVectorZero(copyVectorAToZero, bitsA))
            {
                ShiftRight(copyVectorAToZero, bitsA, 1);
                PrintBoolVectorToConsole(copyVectorAToZero, bitsA);
            }
        }
        free(copyVectorAToZero);

        unsigned char *copyVectorBToZero = CopyVector(vectorBoolB, bitsB);
        if (copyVectorBToZero)
        {
            printf("Resurs shift:\n ");
            PrintBoolVectorToConsole(copyVectorBToZero, bitsB);
            while (!isVectorZero(copyVectorBToZero, bitsB))
            {
                ShiftLeft(copyVectorBToZero, bitsB, 1);
                PrintBoolVectorToConsole(copyVectorBToZero, bitsB);
            }
        }
        free(copyVectorBToZero);
        free(vectorBoolA);
        free(vectorBoolB);
    }
}