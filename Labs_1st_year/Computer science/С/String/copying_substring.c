#include <stdio.h>
#include<string.h>
#include<stdbool.h>

bool ChangeText(char *someText, char *someStr)
{
    if (someText && someStr){
        char *strA = someText; // Указатель на 1 элемыент текста
        char *strB = someStr;  // Указатель на 1 элемент подстроки

        while(*strA++ = *strB++);                    //Замена текста на подстроку
        return true;
    }
    return false;
}

int main()
{
    char someText[100];          // Массив для текста
    char someStr[100];           // Массив для подстроки

    printf("Enter the text: ");
    fgets(someText, 100, stdin); // Ввод текста
    printf("Enter the substring: ");
    fgets(someStr, 100, stdin);  // Ввод подстроки

     // Remove new line characters from input strings
    someText[strcspn(someText, "\n")] = 0;
    someStr[strcspn(someStr, "\n")] = 0;
    bool copySuccess = ChangeText(someText, someStr);

    if (copySuccess){
            printf("Copying was successful. Result:%s", someText);
    }
    else
        printf("Copying failed.");

    return 0;
}