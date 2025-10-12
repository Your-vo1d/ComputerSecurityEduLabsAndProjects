section .data
    prompt_size db 'Enter array size: ', 0
    prompt_size_len equ $ - prompt_size
    
    prompt_element db 'Enter element: ', 0
    prompt_element_len equ $ - prompt_element
    
    found_msg db 'Found consecutive same sign elements: ', 0
    found_msg_len equ $ - found_msg
    
    not_found_msg db 'No consecutive same sign elements found.', 10, 0
    not_found_msg_len equ $ - not_found_msg
    
    error_msg db 'Error: Invalid input!', 10, 0
    error_msg_len equ $ - error_msg
    
    space db ' ', 0
    newline db 10, 0

section .bss
    array_size resd 1
    array resd 100        ; массив до 100 элементов
    buffer resb 12
    temp_buffer resb 12
    found_index resd 1    ; индекс первого найденного элемента

section .text
    global _start

_start:
    ; Ввод размера массива
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_size
    mov edx, prompt_size_len
    int 0x80
    
    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 12
    int 0x80
    
    call atoi
    mov [array_size], eax
    
    ; Проверка размера массива
    cmp eax, 2
    jl exit_not_found     ; если размер < 2, не может быть двух элементов
    cmp eax, 100
    jg input_error        ; если размер > 100, ошибка
    
    ; Ввод элементов массива
    mov esi, array        ; указатель на массив
    mov ecx, [array_size] ; счетчик элементов
    
input_loop:
    push ecx
    push esi
    
    ; Вывод приглашения
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt_element
    mov edx, prompt_element_len
    int 0x80
    
    ; Ввод элемента
    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 12
    int 0x80
    
    call atoi
    pop esi
    mov [esi], eax        ; сохранение элемента в массиве
    add esi, 4            ; переход к следующему элементу (4 байта)
    
    pop ecx
    loop input_loop
    
    ; Поиск двух идущих подряд элементов одного знака
    call find_consecutive_same_sign
    test eax, eax
    jz not_found
    
    ; Вывод сообщения "Found" и найденных чисел
    call print_found_elements
    jmp exit_program

not_found:
    ; Вывод сообщения "Not found"
    mov eax, 4
    mov ebx, 1
    mov ecx, not_found_msg
    mov edx, not_found_msg_len
    int 0x80
    jmp exit_program

exit_not_found:
    mov eax, 4
    mov ebx, 1
    mov ecx, not_found_msg
    mov edx, not_found_msg_len
    int 0x80
    jmp exit_program

input_error:
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

; Функция поиска двух идущих подряд элементов одного знака
; Выход: EAX = 1 если найдены, 0 если не найдены
find_consecutive_same_sign:
    push ebx
    push ecx
    push esi
    push edi
    
    mov esi, array           ; указатель на начало массива
    mov ecx, [array_size]
    dec ecx                  ; проходим до предпоследнего элемента
    
    cmp ecx, 0
    jle no_consecutive       ; если элементов меньше 2
    
    mov edi, 0               ; индекс текущего элемента
    
check_loop:
    ; Получаем текущий элемент
    mov eax, [esi]          ; текущий элемент
    mov ebx, [esi + 4]      ; следующий элемент
    
    ; Проверяем знак текущего элемента
    call get_sign
    push eax                ; сохраняем знак текущего элемента
    
    ; Проверяем знак следующего элемента
    mov eax, ebx
    call get_sign
    
    pop edx                 ; восстанавливаем знак текущего элемента
    
    ; Сравниваем знаки
    cmp edx, eax
    jne next_check          ; если знаки разные
    
    ; Сохраняем индекс найденной пары
    mov [found_index], edi
    jmp found_consecutive
    
next_check:
    ; Переход к следующей паре
    add esi, 4
    inc edi
    loop check_loop
    
no_consecutive:
    xor eax, eax
    jmp find_end
    
found_consecutive:
    mov eax, 1

find_end:
    pop edi
    pop esi
    pop ecx
    pop ebx
    ret

; Функция вывода найденных элементов
print_found_elements:
    push ebx
    push ecx
    push esi
    
    ; Вывод сообщения "Found"
    mov eax, 4
    mov ebx, 1
    mov ecx, found_msg
    mov edx, found_msg_len
    int 0x80
    
    ; Вывод первого элемента пары
    mov esi, array
    mov ecx, [found_index]
    shl ecx, 2              ; умножаем индекс на 4 (размер int)
    add esi, ecx            ; получаем указатель на первый элемент
    
    mov eax, [esi]          ; первый элемент
    call print_int
    
    ; Вывод пробела
    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80
    
    ; Вывод второго элемента пары
    mov eax, [esi + 4]      ; второй элемент
    call print_int
    
    ; Вывод перевода строки
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80
    
    pop esi
    pop ecx
    pop ebx
    ret

; Функция определения знака числа
; Вход: EAX = число
; Выход: EAX = 1 (положительный), -1 (отрицательный), 0 (ноль)
get_sign:
    cmp eax, 0
    jg positive
    jl negative
    mov eax, 0              ; ноль
    ret
positive:
    mov eax, 1
    ret
negative:
    mov eax, -1
    ret

; Функция atoi - преобразование строки в число
atoi:
    push ebx
    push ecx
    push edx
    push esi
    
    xor eax, eax
    xor ecx, ecx
    mov esi, buffer
    xor ebx, ebx            ; флаг отрицательности
    
    ; Проверка на отрицательное число
    mov cl, byte [esi]
    cmp cl, '-'
    jne atoi_loop
    mov ebx, 1              ; установка флага отрицательности
    inc esi

atoi_loop:
    mov cl, byte [esi]
    cmp cl, 10              ; конец строки (LF)
    je atoi_done
    cmp cl, 0               ; конец строки (NULL)
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
    ; Обработка отрицательного числа
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
    
    ; Проверка на отрицательное число
    test eax, eax
    jns pi_convert_loop
    neg eax                 ; делаем число положительным
    push eax
    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80
    mov eax, '-'
    mov [temp_buffer], al
    mov eax, 4
    mov ebx, 1
    mov ecx, temp_buffer
    mov edx, 1
    int 0x80
    pop eax
    
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