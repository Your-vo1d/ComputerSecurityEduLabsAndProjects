section .data
    prompt_number db 'Enter number: ', 0
    prompt_number_len equ $ - prompt_number

    prompt_sum   db 'Sum: ', 0
    prompt_sum_len equ $ - prompt_sum
    
    prompt_mul   db 'Product: ', 0
    prompt_mul_len equ $ - prompt_mul
    
    newline  db 10
    minus    db '-'
    
    error_msg db 'Error: Number must be 3-digit!', 10, 0
    error_msg_len equ $ - error_msg

section .bss
    number resd 1
    result_sum resd 1
    result_mul resd 1
    buffer resb 12
    temp_buffer resb 12

section .text
    global _start

_start:
    ; Вывод приглашения
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_number
    mov edx, prompt_number_len
    int 0x80

    ; Ввод числа
    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 12
    int 0x80
    
    ; Проверяем длину ввода перед преобразованием
    call check_input_length
    test eax, eax
    jz error
    
    ; Преобразование строки в число
    call atoi
    mov [number], eax

    ; Дополнительная проверка диапазона
    call check_three_digit
    test eax, eax
    jz error

    ; Вычисление суммы и произведения цифр
    call calculate_sum_and_product

    ; Вывод результатов
    call print_results

    ; Завершение программы
    mov eax, 1
    xor ebx, ebx
    int 0x80

; Проверка длины ввода - должна быть 3 или 4 символа (без учета \n)
check_input_length:
    mov esi, buffer
    xor ecx, ecx
    
count_length:
    mov al, byte [esi + ecx]
    cmp al, 10    ; новая строка
    je check_count
    cmp al, 0     ; нуль-терминатор
    je check_count
    inc ecx
    cmp ecx, 12
    jl count_length
    
check_count:
    ; Для положительных: 3 цифры
    ; Для отрицательных: 1 знак + 3 цифры = 4 символа
    cmp ecx, 3
    je valid_length
    cmp ecx, 4
    jne invalid_length
    
    ; Если 4 символа, проверяем что первый это минус
    mov al, byte [buffer]
    cmp al, '-'
    je valid_length
    
invalid_length:
    xor eax, eax
    ret
    
valid_length:
    mov eax, 1
    ret

; Проверка что число трехзначное
check_three_digit:
    mov eax, [number]
    
    ; Проверяем отрицательные числа -999 до -100
    cmp eax, -999
    jl not_three_digit
    cmp eax, -100
    jle is_three_digit
    
    ; Проверяем положительные числа 100-999
    cmp eax, 100
    jl not_three_digit
    cmp eax, 999
    jle is_three_digit
    
not_three_digit:
    xor eax, eax
    ret
    
is_three_digit:
    mov eax, 1
    ret

; Вычисление суммы и произведения цифр
calculate_sum_and_product:
    mov eax, [number]
    
    ; Берем абсолютное значение для работы с цифрами
    test eax, eax
    jns positive_num
    neg eax
    
positive_num:
    ; Разбиваем число на цифры
    mov ecx, 10
    
    ; Третья цифра (единицы)
    xor edx, edx
    div ecx
    mov ebx, edx        ; EBX = цифра единиц
    
    ; Вторая цифра (десятки)
    xor edx, edx
    div ecx
    mov esi, edx        ; ESI = цифра десятков
    
    ; Первая цифра (сотни)
    mov edi, eax        ; EDI = цифра сотен
    
    ; Вычисляем сумму цифр
    mov eax, edi
    add eax, esi
    add eax, ebx
    mov [result_sum], eax
    
    ; Вычисляем произведение цифр
    mov eax, edi
    imul eax, esi
    imul eax, ebx
    mov [result_mul], eax
    
    ret

; Вывод результатов
print_results:
    ; Вывод суммы
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_sum
    mov edx, prompt_sum_len
    int 0x80
    
    mov eax, [result_sum]
    call print_int
    
    ; Перевод строки
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80
    
    ; Вывод произведения
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_mul
    mov edx, prompt_mul_len
    int 0x80
    
    mov eax, [result_mul]
    call print_int
    
    ; Перевод строки
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80
    
    ret

; Функция atoi - преобразование строки в число
atoi:
    xor eax, eax
    xor ecx, ecx
    xor ebx, ebx
    mov esi, buffer
    
    ; Проверка на отрицательное число
    mov cl, byte [esi]
    cmp cl, '-'
    jne atoi_loop
    mov ebx, 1
    inc esi
    
atoi_loop:
    mov cl, byte [esi]
    cmp cl, 10
    je atoi_done
    cmp cl, 0
    je atoi_done
    cmp cl, '0'
    jl atoi_error
    cmp cl, '9'
    jg atoi_error
    
    sub cl, '0'
    
    ; Проверка переполнения для положительных чисел
    test ebx, ebx
    jnz negative_check
    
    ; Для положительных: проверяем что не превысили 999
    cmp eax, 999
    ja atoi_error
    jmp continue_convert
    
negative_check:
    ; Для отрицательных: проверяем что не превысили 999 (по модулю)
    cmp eax, 999
    ja atoi_error
    
continue_convert:
    imul eax, 10
    jo atoi_error      ; проверка переполнения при умножении
    add eax, ecx
    jo atoi_error      ; проверка переполнения при сложении
    
    inc esi
    jmp atoi_loop
    
atoi_done:
    test ebx, ebx
    jz atoi_positive
    neg eax
    jo atoi_error      ; проверка переполнения при инвертировании знака
atoi_positive:
    ret

atoi_error:
    xor eax, eax
    ret

; Функция print_int - вывод числа
print_int:
    push ebx
    push ecx
    push edx
    push esi
    push edi
    
    mov edi, temp_buffer + 11
    mov byte [edi], 0
    mov ecx, 10
    
    ; Проверяем знак числа
    test eax, eax
    jns pi_convert_positive
    
    ; Отрицательное число
    neg eax
    mov ebx, 1
    jmp pi_convert_loop
    
pi_convert_positive:
    mov ebx, 0
    
pi_convert_loop:
    dec edi
    xor edx, edx
    div ecx
    add dl, '0'
    mov byte [edi], dl
    test eax, eax
    jnz pi_convert_loop
    
    ; Выводим знак минуса если нужно
    test ebx, ebx
    jz pi_output_number
    
    mov eax, 4
    mov ebx, 1
    mov ecx, minus
    mov edx, 1
    int 0x80
    
pi_output_number:
    mov esi, edi
    mov edx, temp_buffer + 12
    sub edx, esi
    
    mov eax, 4
    mov ebx, 1
    mov ecx, esi
    int 0x80
    
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    ret

; Обработка ошибки
error:
    mov eax, 4
    mov ebx, 1
    mov ecx, error_msg
    mov edx, error_msg_len
    int 0x80
    
    mov eax, 1
    mov ebx, 1
    int 0x80