section .rodata ; Раздел .rdata в котором хранятся константы

hello_world: db "Hello world!", 0xA ; Объявляем переменную hello_word размером 1 байт и содержащий в памяти строку 'Hello world!'

section .text ; Раздел .text, где находится код программы

global _start ; Компоновщик находит символ _start и начинает выполнение программы
_start: ; Старт программы

    mov eax, 4 ; Вызов системной функции под номером 4 (0x04 - write())
    mov ebx, 1 ; Дескриптор файла (1 = стандартный вывод)
    mov ecx, hello_world ;Указываем адрес переменной
    mov edx, 13 ;  Указываем длину в байтах
    int 0x80 ;Системный вызов
    
    mov eax, 4 ; Вызов системной функции под номером 4 (0x04 - write())
    int 0x80 ;Системный вызов
    
    mov eax, 4 ; Вызов системной функции под номером 4 (0x04 - write())
    int 0x80 ;Системный вызов
    
    mov eax, 1
    int 80