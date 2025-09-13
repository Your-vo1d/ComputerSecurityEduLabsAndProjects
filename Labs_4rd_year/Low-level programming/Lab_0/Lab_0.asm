section .data
    prompt_number_1 db 'Enter number №1: '
    prompt_number_1_len equ $ - prompt_number_1
    prompt_number_2 db 'Enter number №2: '
    prompt_number_2_len equ $ - prompt_number_2
    prompt_number_3 db 'Enter number №3: '
    prompt_number_3_len equ $ - prompt_number_3
    result   db 'Result: '
    result_len equ $ - result
    space    db ' '
    newline  db 10
    minus    db '-'

section .bss
    number_1 resd 1 
    number_2 resd 1
    number_3 resd 1
    buffer resb 12
    temp_buffer resb 12

section .text
    global _start

_start:
    ; Ввод первого числа
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_number_1
    mov edx, prompt_number_1_len
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 12
    int 0x80
    call atoi
    mov [number_1], eax

    ; Ввод второго числа
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_number_2
    mov edx, prompt_number_2_len
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 12
    int 0x80
    call atoi
    mov [number_2], eax

    ; Ввод третьего числа
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_number_3
    mov edx, prompt_number_3_len
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 12
    int 0x80
    call atoi
    mov [number_3], eax

    ; Нахождение максимального числа
    mov eax, [number_1]
    mov ebx, [number_2]
    mov ecx, [number_3]

    ; Сравнение чисел
    cmp eax, ebx
    jl check_number_2_max
    cmp eax, ecx
    jl check_number_3_max

    ; number_1 - максимальное
    sub ebx, eax
    sub ecx, eax
    jmp save_results

check_number_2_max:
    cmp ebx, ecx
    jl number_3_is_max
    
    ; number_2 - максимальное
    sub eax, ebx
    sub ecx, ebx
    jmp save_results

check_number_3_max:
    cmp ecx, ebx
    jl number_2_is_max
    
    ; number_3 - максимальное
    sub eax, ecx
    sub ebx, ecx
    jmp save_results

number_3_is_max:
    sub eax, ecx
    sub ebx, ecx
    jmp save_results

number_2_is_max:
    sub eax, ebx
    sub ecx, ebx

save_results:
    mov [number_1], eax
    mov [number_2], ebx
    mov [number_3], ecx

    ; Вывод результата
    mov eax, 4
    mov ebx, 1
    mov ecx, result
    mov edx, result_len
    int 0x80

    mov eax, [number_1]
    call print_int

    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80

    mov eax, [number_2]
    call print_int

    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80

    mov eax, [number_3]
    call print_int

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    ; Завершение программы
    mov eax, 1
    xor ebx, ebx
    int 0x80

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
    imul eax, 10
    add eax, ecx
    inc esi
    jmp atoi_loop
    
atoi_done:
    test ebx, ebx
    jz atoi_positive
    neg eax
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
    
    mov edi, temp_buffer + 11   ; используем временный буфер
    mov byte [edi], 0
    mov ecx, 10
    
    ; Проверяем знак числа
    test eax, eax
    jns pi_convert_positive
    
    ; Отрицательное число
    neg eax                     ; делаем положительным для обработки
    mov ebx, 1                  ; флаг отрицательного числа
    jmp pi_convert_loop
    
pi_convert_positive:
    mov ebx, 0                  ; флаг положительного числа
    
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
    ; Вычисляем длину числа
    mov esi, edi
    mov edx, temp_buffer + 12
    sub edx, esi
    
    ; Выводим само число
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