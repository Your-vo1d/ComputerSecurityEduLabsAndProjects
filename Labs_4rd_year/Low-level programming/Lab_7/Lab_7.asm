section .data
    prompt db "Enter boolean vector (only 0 and 1, no spaces): ", 0
    result_one db "Majority function = 1", 0
    result_zero db "Majority function = 0", 0
    error_msg db "Error: Invalid input! Only characters '0' and '1' are allowed.", 0
    newline db 10, 0

section .bss
    input resb 100      ; Буфер для ввода
    count_ones resd 1   ; Счетчик единиц
    count_zeros resd 1  ; Счетчик нулей

section .text
    global _start

_start:
    ; Вывод приглашения
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt
    mov edx, 48
    int 0x80

    ; Чтение ввода с клавиатуры
    mov eax, 3
    mov ebx, 0
    mov ecx, input
    mov edx, 100
    int 0x80

    ; Инициализация счетчиков
    mov dword [count_ones], 0
    mov dword [count_zeros], 0

    ; Обработка введенного вектора
    mov esi, input      ; Указатель на начало строки

process_loop:
    mov al, [esi]       ; Читаем текущий символ
    
    ; Проверка на конец строки
    cmp al, 0
    je check_result
    cmp al, 10
    je check_result
    
    ; Проверка корректности символа
    cmp al, '0'
    je found_zero
    cmp al, '1'
    je found_one
    
    ; Если символ не '0' и не '1' - ошибка
    jmp input_error

found_zero:
    inc dword [count_zeros]
    jmp next_char

found_one:
    inc dword [count_ones]
    jmp next_char

next_char:
    inc esi             ; Переходим к следующему символу
    jmp process_loop

input_error:
    ; Вывод сообщения об ошибке
    mov eax, 4
    mov ebx, 1
    mov ecx, error_msg
    mov edx, 64
    int 0x80
    
    ; Новая строка и завершение с ошибкой
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 2
    int 0x80
    
    mov eax, 1
    mov ebx, 1          ; Код возврата 1 (ошибка)
    int 0x80

check_result:
    ; Проверяем, что был введен хотя бы один символ
    mov eax, [count_ones]
    mov ebx, [count_zeros]
    add eax, ebx
    cmp eax, 0
    je input_error      ; Если нет ни единиц, ни нулей - ошибка

    ; Сравниваем количество единиц и нулей
    mov eax, [count_ones]
    mov ebx, [count_zeros]
    cmp eax, ebx
    jg output_one       ; Если единиц больше нулей
    jmp output_zero     ; Иначе

output_one:
    ; Вывод результата "1"
    mov eax, 4
    mov ebx, 1
    mov ecx, result_one
    mov edx, 21
    int 0x80
    jmp exit_success

output_zero:
    ; Вывод результата "0"
    mov eax, 4
    mov ebx, 1
    mov ecx, result_zero
    mov edx, 21
    int 0x80

exit_success:
    ; Вывод новой строки
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 2
    int 0x80

    ; Завершение программы успешно
    mov eax, 1
    xor ebx, ebx        ; Код возврата 0 (успех)
    int 0x80