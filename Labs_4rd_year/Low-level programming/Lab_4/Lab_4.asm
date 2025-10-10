section .data
    prompt_number db 'Enter number: ', 0
    prompt_number_len equ $ - prompt_number

    success_msg db 'Yes: ', 0
    success_msg_len equ $ - success_msg
    
    fail_msg db 'No', 10, 0
    fail_msg_len equ $ - fail_msg
    
    space db ' ', 0
    newline db 10, 0
    
    error_msg db 'Error: Number must be natural!', 10, 0
    error_msg_len equ $ - error_msg

section .bss
    number resd 1
    a_val resd 1
    b_val resd 1  
    c_val resd 1
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
    
    ; Преобразование строки в число
    call atoi
    mov [number], eax

    ; Проверка что число натуральное
    cmp eax, 1
    jl error

    ; Поиск представления в виде суммы трех квадратов
    call find_three_squares
    test eax, eax
    jz no_solution
    
    ; Вывод результата
    mov eax, 4
    mov ebx, 1
    mov ecx, success_msg
    mov edx, success_msg_len
    int 0x80
    
    ; Вывод найденных чисел
    call print_solution
    jmp exit_program

no_solution:
    ; Вывод сообщения "No"
    mov eax, 4
    mov ebx, 1
    mov ecx, fail_msg
    mov edx, fail_msg_len
    int 0x80
    jmp exit_program

error:
    mov eax, 4
    mov ebx, 1
    mov ecx, error_msg
    mov edx, error_msg_len
    int 0x80
    mov ebx, 1

exit_program:
    mov eax, 1
    int 0x80

; Поиск трех натуральных чисел, квадраты которых в сумме дают заданное число
find_three_squares:
    push ebx
    push esi
    push edi
    
    mov ecx, [number]  ; ECX = n
    
    ; Первое число (a) от 1 до n
    mov ebx, 1
    
first_loop:
    ; Вычисляем a²
    mov eax, ebx
    mul eax
    cmp eax, ecx       ; если a² >= n, выходим
    jge first_loop_end
    
    ; Сохраняем a²
    push eax
    
    ; Второе число (b) от 1 до n
    mov esi, 1
    
second_loop:
    ; Вычисляем b²
    mov eax, esi
    mul eax
    mov edi, eax       ; EDI = b²
    
    ; Проверяем a² + b² < n
    pop eax
    push eax
    add eax, edi
    cmp eax, ecx
    jge second_loop_end
    
    ; Вычисляем c² = n - a² - b²
    mov eax, ecx
    sub eax, [esp]     ; n - a²
    sub eax, edi       ; n - a² - b²
    
    ; Проверяем что c² > 0
    cmp eax, 0
    jle next_second
    
    ; Проверяем, является ли c² полным квадратом
    push ebx
    push esi
    push edi
    call is_perfect_square
    pop edi
    pop esi
    pop ebx
    test eax, eax
    jz next_second
    
    ; Нашли решение! Сохраняем значения
    mov [a_val], ebx
    mov [b_val], esi  
    mov [c_val], eax
    pop eax       ; очистка стека
    mov eax, 1    ; успех
    jmp found_solution
    
next_second:
    inc esi
    jmp second_loop
    
second_loop_end:
    pop eax       ; очистка стека
    inc ebx
    jmp first_loop
    
first_loop_end:
    xor eax, eax  ; не найдено
    
found_solution:
    pop edi
    pop esi
    pop ebx
    ret

; Проверка, является ли число полным квадратом
; Вход: EAX = число для проверки
; Выход: EAX = корень если является квадратом, иначе 0
is_perfect_square:
    push ebx
    push ecx
    push edx
    
    mov ecx, eax  ; сохраняем число
    
    ; Быстрая проверка для 0
    test ecx, ecx
    jz no_root
    
    ; Ищем корень методом грубой силы
    mov ebx, 1
    
ips_loop:
    mov eax, ebx
    mul eax
    cmp eax, ecx
    je found_root
    jg no_root
    inc ebx
    cmp ebx, 10000  ; разумный предел
    jle ips_loop
    
no_root:
    xor eax, eax
    jmp ips_end
    
found_root:
    mov eax, ebx

ips_end:
    pop edx
    pop ecx
    pop ebx
    ret

; Вывод найденного решения
print_solution:
    ; Вывод первого числа
    mov eax, [a_val]
    call print_int
    
    ; Пробел
    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80
    
    ; Вывод второго числа
    mov eax, [b_val]
    call print_int
    
    ; Пробел
    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80
    
    ; Вывод третьего числа
    mov eax, [c_val]
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
    mov esi, buffer
    
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
    
pi_convert_loop:
    dec edi
    xor edx, edx
    div ecx
    add dl, '0'
    mov byte [edi], dl
    test eax, eax
    jnz pi_convert_loop
    
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