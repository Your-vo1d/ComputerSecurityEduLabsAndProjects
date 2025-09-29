section .data
    prompt_one_x db 'Enter positon X for №1: ', 24
    prompt_one_x_len equ $ - prompt_one_x
    prompt_one_y db 'Enter positon Y for №1: ', 24
    prompt_one_y_len equ $ - prompt_one_y  

    prompt_result   db 'Result: '
    prompt_result_len equ $ - prompt_result
    space    db ' '
    newline  db 10
    minus    db '-'

section .bss
    one_x resd 1 
    one_y resd 1
    two_x resd 1
    two_y resd 1
    result resd 1
    buffer resb 12
    temp_buffer resb 12

section .text
    global _start

_start:
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_one_x
    mov edx, prompt_one_x_len
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 12
    int 0x80
    call atoi
    mov [one_x], eax
    

    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_one_y
    mov edx, prompt_one_x_len
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 12
    int 0x80
    call atoi
    mov [one_y], eax

    mov eax, [one_x]
    mov ebx, [one_y]
    cmp eax, 0
    je center_or_x
    cmp ebx, 0
    je center_or_y
    mov eax, 3
    mov [result], eax
    jmp print_result

center_or_x:
    cmp ebx, 0
    je center
    mov eax, 1
    mov [result], eax
    jmp print_result 

center_or_y:
    cmp eax, 0
    je center
    mov eax, 2
    mov [result], eax
    jmp print_result

center:
    mov eax, 0
    mov [result], eax
    jmp print_result

print_result:
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_result
    mov edx, prompt_result_len
    int 0x80

    mov eax, [result]
    call print_int

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