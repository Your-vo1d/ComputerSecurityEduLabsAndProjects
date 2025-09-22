section .data
    prompt_a db 'Enter A: '
    prompt_a_len equ $ - prompt_a
    prompt_b db 'Enter B: '
    prompt_b_len equ $ - prompt_b
    prompt_error_negative_number db 'Error: negative value', 10
    prompt_error_negative_number_len equ $ - prompt_error_negative_number
    prompt_result db 'Result: '
    prompt_result_len equ $ - prompt_result
    space    db ' '
    newline  db 10

section .bss
    a resd 1 
    b resd 1
    result resd 1
    buffer resb 12
    temp_buffer resb 12

section .text
    global _start
_start:
    ; Вывод приглашения для ввода A
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_a 
    mov edx, prompt_a_len
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 12
    int 0x80
    call atoi
    mov [a], eax

    ; Вывод приглашения для ввода B
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_b
    mov edx, prompt_b_len
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 12
    int 0x80
    call atoi
    mov [b], eax

    mov eax, [a]
    xor edx, edx        ; Clear EDX before division
    mov ebx, [b]
    div ebx             ; EAX = quotient, EDX = remainder
    mov [result], edx

    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_result
    mov edx, 8
    int 0x80

    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx

    mov eax, [result]
    call print_int
    

        mov eax, 1          ; номер системного вызова exit
    xor ebx, ebx        ; код возврата 0
    int 0x80            ; вызов ядра

; Функция atoi - преобразование строки в число
atoi:
    xor eax, eax
    xor ecx, ecx
    xor ebx, ebx
    mov esi, buffer
    
    ; Проверка на отрицательное число
    mov cl, byte [esi]
    cmp cl, '-'
    je error_negative_number
    jmp atoi_loop

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
    ret

atoi_error:
    xor eax, eax
    ret

error_negative_number:
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_error_negative_number
    mov edx, prompt_error_negative_number_len
    int 0x80

    mov eax, 1          ; номер системного вызова exit
    xor ebx, ebx        ; код возврата 0
    int 0x80            ; вызов ядра

print_int:
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
    
    jmp pi_output_number
    
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
    ret