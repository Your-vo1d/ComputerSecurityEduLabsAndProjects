section .text ; Начало селекции .text, где находится код программы

global _start ; Начало программы начинается с метки _start
_start: ; Начало программа

    mov esi,0

    Input_num: ; Часть кода, где высчитывается число num с консоли. Нужно для ввода многозначного числа

        mov ecx, str1 ;Чтение 1 символа
        mov edx, 1
        mov ebx, 0
        mov eax, 3
        int 0x80
        
        mov bl, [str1] ; Символ, который только считала программа записываем в регистр
        
        cmp bl, ' ' ; Сравниваем символ с кодом пробела
        je Check_num
        
        je Writing_num ; Если это был пробел, то это знак того, что число n закончилось
        ; А если это не пробел и не цифра, то выдаем ошибку
        cmp bl, '0'
        jl Error_input
        cmp bl, '9'
        jg Error_input
       
        ; Если это была цифра, то мы добавляем ее в конец уже имеющегося числа, проверяя на переполнение
        mov edx, 10
        mov eax,esi
        mul edx
        jo Overflow
       
        sub bl,'0'
        add eax,ebx
        jo Overflow
        mov esi,eax

        jmp Input_num ; Прыжок создает цикл считывания числа n
        
        Check_num:
            cmp esi, 0
            je Error_input

    Writing_num: ; Далее просто записываем получившееся число
    
        mov [num], esi
        xor esi,esi

    Input_power: ; Полностью аналогичный цикл, но уже для числа power
    
        mov eax, 3
        mov ebx, 0
        mov ecx, str1
        mov edx, 1
        int 0x80
        
        mov bl, [str1]
        cmp bl, 10
        je Check_power
        
        cmp bl, '0'
        jl Error_input
        cmp bl, '9'
        jg Error_input
        
        mov edx,10
        mov eax,esi
        mul edx
        jo Overflow
        sub bl,'0'
        add eax,ebx
        jo Overflow
        mov esi,eax
        
        jmp Input_power
                
        Check_power:
            cmp esi, 0
            je Error_input
    
    Writing_power:
    
        mov [power],esi

    Calculating: ; Далее идет вычисление

        xor eax, eax
        xor ebx, ebx
        xor ecx, ecx
        xor edx, edx

; Далее проверяем корректность введеных символов и сравниваем значения с нулем
    mov eax, [num] ; Запись в регистр значения переменной
    mov ebx, [num] 
    mov ecx, [power] ; Запись в регистр значения переменной
    mov edx, eax

        cmp ecx, 0
        je Step_0
        cmp ecx, 1
        je Final
        cmp eax, 1
        je Final
        cmp eax, 0
        je Final
        sub ebx, 1
        sub ecx, 1
        jmp FirstL
        
        Step_0:
        cmp eax, 0
        je No_point
        jmp power_Zero
    
    ; В FirstL идет операция степени через сложение, а в SecondL операция умножения через сложение
    FirstL: ; Метка
        mov esi, ebx ; Записываем в регистр значение другого регистра
        SecondL: ; Метка
            add edx, eax ; Складываем два регистра
            jo Overflow
            dec esi ; Вычитаем единицу из регистра
            jnz SecondL ; Если регистр не равен нулю, то прыжок на метку
        xor eax, eax ; Обнуляем регистр
        mov eax, edx ; Записываем в регистр значение другого регистра
        jo Overflow
        loop FirstL ; Вычитаем из регистра единицу и, если будет больше нуля, то прыжок на метку
    jmp Final ; Безусловный прыжок на метку, чтобы обойти часть кода и не повредить результат

; Запись результата при степени 0
    power_Zero: ; Метка
        xor eax, eax ; Обнуляем регистр
        mov eax, 1 ; Записываем в регистр единицу

; Выводим результат после посимвольной записи в стек
    Final:
        ; Обнуляем регистры для будущего подсчета через них
        xor ecx, ecx
        xor ebx, ebx
        xor edx, edx

        mov ecx, 10 ; Вводим в регистр число 10 для будущего деления и посимвольного ввода в стек
        DoExtractNumber: ; Метка
        ; Делим значение из регистровой пары eax:edx на 10 и переводим остаток в десятичный код
        div ecx
        add edx, dword 48 ; добавляем код нуля, тем самым конвертируем число в символ
        jo Overflow
        ; Заносим остаток от деления в стек и обнуляем регистр для будущего остатка
        push edx
        xor edx, edx
        
        inc ebx ; считаем количество итераций, которое будет указывать на то сколько цифр выделили и положили в стек
        cmp eax, 0 ; сравниваем значение регистра с нулем
        ;Если оно не равно нулю и там осталось какое-то занчение, то продолжаем деление
        
        jnz DoExtractNumber

    mov [tmp], ebx ; Запоминаем количество выделенных цифр из числа
    jo Overflow
    PrintNumber:
        ;Записываем символ из стека в регистр и выводим на экран, тем самым выводим само число
        pop eax
        mov [num], eax ; Записываем в переменную значение регистра
        
        mov eax, 4 ; Системная функция под номером 4
        mov ebx, 1 ; Указываем, что читаем из кода
        mov ecx, num ; Записываем адрес переменной
        mov edx, 1 ; Указываем длину
        int 0x80 ; Системный вызов
        
        dec dword [tmp] ; здесь уменьшаем количество итераций, которые были посчитаны когда выделяли цифру и заносили ее в стек.
        cmp [tmp], dword 0 ; сравниваем со значение 0, выясняем, не пора ли заканчивать вывод
        jnz PrintNumber ; Если значение не равно нулю, то есть в стеке еще есть символы, то повторяем вывод
    
    jmp end ; Безусловный прыжок на метку, чтобы не вывести ошибку и закончить программу
    
    Overflow:;Метка переполнения
        mov eax, 4
        mov ecx, msg2
        mov ebx, 1
        mov edx, len2
        int 0x80
        jmp end

    No_point:
        mov eax, 4
        mov ebx, 1
        mov ecx, msgNo_point
        mov edx, lenNo_Point
        int 0x80
        jmp end
    
    Error_input: ; Часть кода для вывода ошибки при некорректной строке
        mov eax, 4 ; Запись числа 4 в регистр
        mov ecx, msg1 ; Указываем адрес переменной
        mov ebx, 1 ; Дескриптор файла
        mov edx, len1 ; Указываем длину
        int 0x80 ; Системный вызов
    
    end: ; Метка конца
        mov eax, 1 ; Системная функция под номером 1
        int 0x80 ; Системный вызов

section .data ; Начало селекции, где хранятся переменные
    str1 dd 0 ; Выделение 4 байт на переменную str1 и запись в нее нуля
    num dd 0 ; Выделение 4 байта на переменную n и запись в нее нуля
    power dd 0 ; Выделение 4 байт на переменную m и запись в нее нуля
    msg1 db "Error: Bad digits" ; Выделение одного байта на переменную msg1 и запись в нее строки
    len1 equ $ - msg1 ; Подсчет длины строки переменной msg1
    msg2 db "Error: Overflow" ; Выделение одного байта на переменную msg2 и запись в нее строки
    len2 equ $ - msg2 ; Подсчет длины строки переменной msg2
    tmp dd 0 ; Выделение 4 байт на переменную tmp и запись в нее нуля
    msgNo_point db 'Error uncertainty'
    lenNo_Point equ $ - msgNo_point