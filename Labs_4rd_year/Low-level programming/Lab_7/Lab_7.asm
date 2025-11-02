; majority_full.asm
; 32-бит Linux, NASM
; Мажоритарная функция: 1, если единиц > нулей
; Ввод: строка ТОЛЬКО из 0 и 1, затем n
; ОШИБКА, если в векторе есть символ ≠ 0/1

section .bss
    buf  resb 128     ; буфер ввода
    vec  resd 1       ; вектор
    len  resd 1       ; длина n

section .data
    p1   db "Введите булев вектор (0 и 1): ",0
    p1l  equ $-p1
    p2   db "Введите длину n: ",0
    p2l  equ $-p2
    err  db "ОШИБКА: только 0 и 1!",10
    errl equ $-err
    nl   db 10

section .text
global _start

_start:
    ; приглашение 1
    mov eax,4
    mov ebx,1
    mov ecx,p1
    mov edx,p1l
    int 0x80

    ; чтение строки
    mov eax,3
    mov ebx,0
    mov ecx,buf
    mov edx,128
    int 0x80

    ; парсинг: строка → EDX, проверка на 0/1
    xor edx,edx
    xor ecx,ecx
    mov esi,buf
.parse:
    mov al,[esi]
    inc esi
    cmp al,10
    je .endp
    cmp al,'0'
    je .ok0
    cmp al,'1'
    je .ok1
    ; ошибка: символ не 0 и не 1
    mov eax,4
    mov ebx,1
    mov ecx,err
    mov edx,errl
    int 0x80
    mov eax,1
    mov ebx,1
    int 0x80          ; выход с кодом 1
.ok0:
    ; бит = 0 → ничего
    jmp .next
.ok1:
    mov ebx,1
    shl ebx,cl
    or  edx,ebx
.next:
    inc ecx
    cmp ecx,32
    jb .parse
.endp:
    mov [vec],edx

    ; приглашение 2
    mov eax,4
    mov ebx,1
    mov ecx,p2
    mov edx,p2l
    int 0x80

    ; чтение n
    mov eax,3
    mov ebx,0
    mov ecx,buf
    mov edx,128
    int 0x80

    ; строка → число
    xor eax,eax
    mov esi,buf
.conv:
    mov bl,[esi]
    inc esi
    cmp bl,10
    je .done
    sub bl,'0'
    cmp bl,9
    ja .conv_err
    imul eax,10
    add eax,ebx
    jmp .conv
.conv_err:
    mov eax,4
    mov ebx,1
    mov ecx,err
    mov edx,errl
    int 0x80
    mov eax,1
    mov ebx,2
    int 0x80
.done:
    test eax,eax
    jz .zero_n
    cmp eax,32
    ja .zero_n
    mov [len],eax

    ; вызов
    mov edx,[vec]
    mov ecx,[len]
    call majority

    ; вывод
    add al,'0'
    mov [buf],al
    mov eax,4
    mov ebx,1
    mov ecx,buf
    mov edx,1
    int 0x80

    mov eax,4
    mov ebx,1
    mov ecx,nl
    mov edx,1
    int 0x80

    mov eax,1
    xor ebx,ebx
    int 0x80

.zero_n:
    mov eax,4
    mov ebx,1
    mov ecx,err
    mov edx,errl
    int 0x80
    mov eax,1
    mov ebx,3
    int 0x80


; majority: EDX=вектор, ECX=n → AL=1/0
majority:
    test ecx,ecx
    jz .zero
    xor ebx,ebx
    mov esi,edx
.loop:
    test esi,1
    jz .skip
    inc ebx
.skip:
    shr esi,1
    dec ecx
    jnz .loop
    mov eax,ebx
    shl eax,1
    cmp eax,[len]
    seta al
    ret
.zero:
    xor eax,eax
    ret