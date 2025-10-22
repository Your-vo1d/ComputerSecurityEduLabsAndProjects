section .data
    prompt_n       db 'Enter system size n: '
    prompt_n_len   equ $ - prompt_n
    
    prompt_matrix  db 'Enter matrix elements (upper triangular):', 10
    prompt_matrix_len equ $ - prompt_matrix
    
    prompt_vector  db 'Enter right-hand side vector:', 10
    prompt_vector_len equ $ - prompt_vector
    
    result_msg     db 'Solution: ', 0
    result_msg_len equ $ - result_msg
    
    error_msg      db 'Error: Division by zero!', 10
    error_msg_len  equ $ - error_msg
    
    space          db ' '
    newline        db 10

section .bss
    n           resd 1
    matrix      resd 100    ; матрица 10x10
    vector      resd 10     ; вектор правых частей
    solution    resd 10     ; вектор решения
    buffer      resb 12
    temp_buffer resb 12

section .text
    global _start

_start:
    ; Ввод размерности системы
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_n
    mov edx, prompt_n_len
    int 0x80
    
    call read_int
    mov [n], eax
    
    ; Проверка размерности
    cmp eax, 1
    jl exit_error
    cmp eax, 10
    jg exit_error
    
    ; Ввод матрицы
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_matrix
    mov edx, prompt_matrix_len
    int 0x80
    
    mov esi, matrix        ; указатель на матрицу
    mov ecx, 0             ; индекс строки i

input_matrix_outer:
    mov edx, 0             ; индекс столбца j

input_matrix_inner:
    ; Пропускаем ввод для нижнего треугольника (j < i)
    cmp edx, ecx
    jl skip_input
    
    ; Вывод приглашения для элемента
    push ecx
    push edx
    push esi
    
    call print_matrix_prompt
    call read_int
    
    pop esi
    pop edx
    pop ecx
    
    mov [esi], eax         ; сохранение элемента
    jmp next_input

skip_input:
    ; Заполняем нулем нижний треугольник
    mov dword [esi], 0

next_input:
    add esi, 4             ; следующий элемент
    inc edx
    cmp edx, [n]
    jl input_matrix_inner
    
    inc ecx
    cmp ecx, [n]
    jl input_matrix_outer
    
    ; Ввод вектора правых частей
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_vector
    mov edx, prompt_vector_len
    int 0x80
    
    mov esi, vector
    mov ecx, [n]
    mov edx, 0             ; индекс элемента

input_vector_loop:
    push ecx
    push edx
    push esi
    
    call print_vector_prompt
    call read_int
    
    pop esi
    pop edx
    pop ecx
    
    mov [esi], eax
    add esi, 4
    inc edx
    loop input_vector_loop
    
    ; Решение треугольной системы
    call solve_triangular_system
    
    ; Вывод результата
    mov eax, 4
    mov ebx, 1
    mov ecx, result_msg
    mov edx, result_msg_len
    int 0x80
    
    mov esi, solution
    mov ecx, [n]

output_solution:
    push ecx
    push esi
    
    mov eax, [esi]
    call print_int
    
    ; Вывод пробела
    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80
    
    pop esi
    pop ecx
    
    add esi, 4
    loop output_solution
    
    ; Перевод строки
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80
    
    jmp exit_program

exit_error:
    mov eax, 4
    mov ebx, 1
    mov ecx, error_msg
    mov edx, error_msg_len
    int 0x80
    mov ebx, 1
    jmp exit_program

exit_program:
    mov eax, 1
    mov ebx, 0
    int 0x80

; Функция решения треугольной системы (обратный ход)
solve_triangular_system:
    push ebx
    push ecx
    push edx
    push esi
    push edi
    
    mov ecx, [n]
    dec ecx                 ; i = n-1

solve_outer:
    ; Загружаем правую часть b[i]
    mov eax, vector
    mov ebx, ecx
    shl ebx, 2              ; умножаем на 4 (размер int)
    add eax, ebx
    mov edx, [eax]          ; edx = b[i]
    
    ; Вычитаем сумму a[i][j] * x[j] для j > i
    mov edi, [n]
    dec edi                 ; j = n-1

sum_loop:
    cmp edi, ecx
    jle end_sum_loop
    
    ; Вычисляем адрес a[i][j]
    push ecx
    mov eax, ecx            ; eax = i
    imul eax, [n]           ; eax = i * n
    add eax, edi            ; eax = i * n + j
    shl eax, 2              ; умножаем на 4
    mov eax, [matrix + eax] ; eax = a[i][j]
    
    ; Загружаем x[j]
    mov ebx, [solution + edi * 4] ; ebx = x[j]
    
    ; Умножаем и вычитаем
    imul eax, ebx           ; a[i][j] * x[j]
    sub edx, eax            ; вычитаем из правой части
    pop ecx
    
    dec edi
    jmp sum_loop

end_sum_loop:
    ; Вычисляем адрес a[i][i]
    push ecx
    mov eax, ecx            ; eax = i
    imul eax, [n]           ; eax = i * n
    add eax, ecx            ; eax = i * n + i
    shl eax, 2              ; умножаем на 4
    mov ebx, [matrix + eax] ; ebx = a[i][i] (делитель)
    
    ; Проверка деления на ноль
    test ebx, ebx
    jz division_error
    
    ; Делим edx / ebx
    mov eax, edx            ; eax = делимое
    cdq                     ; расширяем eax в edx:eax
    idiv ebx                ; eax = результат, edx = остаток
    
    ; Сохраняем решение x[i]
    mov [solution + ecx * 4], eax
    pop ecx
    
    dec ecx
    cmp ecx, 0
    jge solve_outer
    
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    ret

division_error:
    mov eax, 4
    mov ebx, 1
    mov ecx, error_msg
    mov edx, error_msg_len
    int 0x80
    mov ebx, 1
    jmp exit_program

; Функция вывода приглашения для элемента матрицы
print_matrix_prompt:
    push eax
    push ebx
    push ecx
    push edx
    
    ; Выводим "A["
    mov eax, 'A'
    call print_char
    mov eax, '['
    call print_char
    
    ; Выводим номер строки
    mov eax, ecx
    call print_int
    
    ; Выводим "]["
    mov eax, ']'
    call print_char
    mov eax, '['
    call print_char
    
    ; Выводим номер столбца
    mov eax, edx
    call print_int
    
    ; Выводим "]: "
    mov eax, ']'
    call print_char
    mov eax, ':'
    call print_char
    mov eax, ' '
    call print_char
    
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret

; Функция вывода приглашения для элемента вектора
print_vector_prompt:
    push eax
    push ebx
    push ecx
    push edx
    
    ; Выводим "b["
    mov eax, 'b'
    call print_char
    mov eax, '['
    call print_char
    
    ; Выводим индекс
    mov eax, edx
    call print_int
    
    ; Выводим "]: "
    mov eax, ']'
    call print_char
    mov eax, ':'
    call print_char
    mov eax, ' '
    call print_char
    
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret

; Функция чтения целого числа
read_int:
    push ebx
    push ecx
    push edx
    push esi
    
    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 12
    int 0x80
    
    call atoi
    
    pop esi
    pop edx
    pop ecx
    pop ebx
    ret

; Функция atoi
atoi:
    push ebx
    push ecx
    push edx
    push esi
    
    xor eax, eax
    xor ecx, ecx
    mov esi, buffer
    xor ebx, ebx
    
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
    pop esi
    pop edx
    pop ecx
    pop ebx
    ret

atoi_error:
    mov eax, 0
    pop esi
    pop edx
    pop ecx
    pop ebx
    ret

; Функция print_int
print_int:
    push ebx
    push ecx
    push edx
    push esi
    push edi
    
    mov edi, temp_buffer + 11
    mov byte [edi], 0
    mov ecx, 10
    
    ; Проверка на отрицательное число
    test eax, eax
    jns pi_convert_loop
    push eax
    mov eax, '-'
    call print_char
    pop eax
    neg eax
    
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

; Функция вывода символа
print_char:
    push eax
    push ebx
    push ecx
    push edx
    
    mov [temp_buffer], al
    mov eax, 4
    mov ebx, 1
    mov ecx, temp_buffer
    mov edx, 1
    int 0x80
    
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret