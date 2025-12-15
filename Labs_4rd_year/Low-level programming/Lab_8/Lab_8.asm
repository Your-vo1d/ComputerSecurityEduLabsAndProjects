section .data

start_x    dd -0.2
end_x      dd 0.8
step       dd 0.1
one        dd 1.0
epsilon    dd 0.0001
header     db "x",9,9,"f(x)",10,0
fmt_line   db "%.5f",9,9,"%.5f",10,0
prompt     db "Enter x0 (|x0| < 1): ",0
series_l   db "Series: %.5f",10,0
standard_l db "Standard: %.5f",10,0
error_msg  db "Invalid input or |x0| >= 1 — try again",10,0
scanf_fmt  db "%f",0

section .bss

current_x resd 1
x0        resd 1
n         resd 1

section .text

global main
extern printf
extern scanf
extern fflush
extern stdout
main:
        finit 
; === ТАБЛИЦА — остановка по сравнению с end_x + epsilon ===
        push header
        call printf
        add  esp, 4
        fld  dword [start_x]
        fstp dword [current_x]
table_loop:
; f(x) = ln(x + sqrt(1 + x^2))
        fld    dword [current_x]
        fld    st0
        fmulp  st1, st0                ; x²
        fld1   
        faddp  st1, st0                ; 1 + x²
        fsqrt                          ; sqrt(1 + x²)
        fld    dword [current_x]
        faddp  st1, st0                ; x + sqrt
        fldln2 
        fxch   
        fyl2x                          ; ln(...)
        sub    esp, 8
        fstp   qword [esp]             ; f(x)
        sub    esp, 8
        fld    dword [current_x]
        fstp   qword [esp]             ; x
        push   fmt_line
        call   printf
        add    esp, 20
        fld    dword [current_x]
        fadd   dword [step]
        fstp   dword [current_x]
; проверка: если current_x > end_x + epsilon, то выход
        fld    dword [current_x]
        fld    dword [end_x]
        fld    dword [epsilon]
        faddp  st1, st0                ; st0 = end_x + epsilon, st1 = current_x
        fcomip st0, st1                ; сравниваем end_x + epsilon с current_x
        fstp   st0                     ; убрать current_x
        jb     .end_table              ; если end_x + epsilon < current_x, то выход
        jmp    table_loop
.end_table:
; === Конец таблицы ===
; === Цикл ввода x0 с рядом по ε = 0.0001 ===
input_loop:
        push  prompt
        call  printf
        add   esp, 4
        push  dword [stdout]
        call  fflush
        add   esp, 4
        push  x0
        push  scanf_fmt
        call  scanf
        add   esp, 8
        cmp   eax, 1
        jne   input_error
        fld   dword [x0]
        fabs  
        fcomp dword [one]
        fstsw ax
        sahf  
        jae   input_error
; === Ряд Тейлора ln(1+x) по ε = 0.0001 (по модулю члена) ===
        fld dword [x0]                 ; st0 = x (term)
        fld st0                        ; st0 = x (term), st1 = x (sum)
        mov dword [n], 1
.taylor_loop:
        inc   dword [n]
        fchs                           ; st0 = -term
        fmul  dword [x0]               ; st0 = -term * x0
        fild  dword [n]
        fld1  
        fsubp st1, st0                 ; st0 = n-1
        fmulp st1, st0                 ; multiply current term by (n-1)
        fild  dword [n]                ; st0 = n, st1 = new_term * (n-1), st2 = sum
        fdivp st1, st0                 ; st0 = new_term, st1 = sum
                                       ; проверка точности
        fld   st0                      ; duplicate
        fabs  
        fcomp dword [epsilon]
        fstsw ax
        sahf  
        jb    .taylor_done             ; |new_term| < 0.0001 → выход
                                       ; add to sum
        fld   st0                      ; duplicate new_term
        faddp st2, st0                 ; sum += new_term, pop, st0 = new_term, st1 = new_sum
        jmp   .taylor_loop
.taylor_done:
        fstp st0                       ; убрать последний term со стека
                                       ; в st0 теперь sum
        sub  esp, 8
        fstp qword [esp]
        push series_l
        call printf
        add  esp, 12
; стандартное значение
        fld    dword [x0]
        fld1   
        faddp  st1
        fldln2 
        fxch   
        fyl2x  
        sub    esp, 8
        fstp   qword [esp]
        push   standard_l
        call   printf
        add    esp, 12
        jmp    input_loop              ; новый ввод
input_error:
        push error_msg
        call printf
        add  esp, 4
        jmp  input_loop

