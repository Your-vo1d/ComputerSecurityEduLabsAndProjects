#include <stdio.h>
#include <stdbool.h>

// Функция для удаления символов из строки someText, которые содержатся в строке symbolsToRemove
int DeleteSymbols(char *text, char *symbolsToRemove) {
    char *ptrSymbols = symbolsToRemove;
    bool symbolTable[256] = {0}; // Таблица для отметки символов, которые нужно удалить

    // Заполнение таблицы символов, которые нужно удалить
    while (*ptrSymbols) {
        symbolTable[*ptrSymbols++] = true;
    }

    int readIndex = 0;
    int writeIndex = 0;

    // Проход по строке и удаление нужных символов
    while (text[readIndex]) {
        if (!symbolTable[(unsigned char)text[readIndex]]) {
            text[writeIndex] = text[readIndex];
            writeIndex++;
        }
        readIndex++;
    }
    text[writeIndex] = '\0'; // Завершаем строку нулевым символом
    printf("%s", text);
}

int main() {
    char text[100];
    char symbolsToRemove[100];

    // Чтение строк из стандартного ввода
    fgets(text, 100, stdin);
    fgets(symbolsToRemove, 100, stdin);

    // Удаление символов
    DeleteSymbols(text, symbolsToRemove);
    return 0;
}
