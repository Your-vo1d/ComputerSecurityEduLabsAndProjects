#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Функция сортировки прямой вставкой на C
void c_insertion_sort(int* arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        // Сдвигаем элементы arr[0..i-1], которые больше key
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Функция сортировки прямой вставкой на ассемблере (исправленная)
void asm_insertion_sort(int* arr, int n) {
    if (n <= 1) return;  // Защита от маленьких массивов
    
    // Используем встроенный ассемблер GCC с правильным использованием регистров
    __asm__ volatile (
        // Сохраняем регистры, которые будем использовать
        "push %%rbx\n\t"
        "push %%r12\n\t"
        "push %%r13\n\t"
        "push %%r14\n\t"
        
        // Инициализация
        "mov %%rdi, %%r12\n\t"     // r12 = указатель на массив
        "mov %%rsi, %%r13\n\t"     // r13 = n
        "mov $1, %%r14\n\t"        // i = 1
        
        // Внешний цикл: for (i = 1; i < n; i++)
        ".outer_loop:\n\t"
        // key = arr[i]
        "movl (%%r12, %%r14, 4), %%eax\n\t"
        
        // j = i - 1
        "mov %%r14, %%rbx\n\t"
        "dec %%rbx\n\t"
        
        // Внутренний цикл: while (j >= 0 && arr[j] > key)
        ".inner_loop:\n\t"
        "cmp $0, %%rbx\n\t"
        "jl .insert_key\n\t"
        
        // Сравнение arr[j] и key
        "movl (%%r12, %%rbx, 4), %%ecx\n\t"
        "cmp %%eax, %%ecx\n\t"
        "jle .insert_key\n\t"
        
        // arr[j+1] = arr[j]
        "movl %%ecx, 4(%%r12, %%rbx, 4)\n\t"
        
        // j--
        "dec %%rbx\n\t"
        "jmp .inner_loop\n\t"
        
        ".insert_key:\n\t"
        // arr[j+1] = key
        "movl %%eax, 4(%%r12, %%rbx, 4)\n\t"
        
        // i++
        "inc %%r14\n\t"
        "cmp %%r13, %%r14\n\t"
        "jl .outer_loop\n\t"
        
        ".end_sort:\n\t"
        // Восстанавливаем регистры
        "pop %%r14\n\t"
        "pop %%r13\n\t"
        "pop %%r12\n\t"
        "pop %%rbx\n\t"
        
        : // выходные операнды (нет)
        : "D" (arr), "S" ((long)n)  // rdi = arr, rsi = n
        : "rax", "rcx", "r12", "r13", "r14", "rbx", "memory", "cc"
    );
}

// Функция проверки упорядоченности массива
int is_sorted(int* arr, int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1]) {
            return 0; // Не отсортирован
        }
    }
    return 1; // Отсортирован
}

// Функция заполнения массива случайными числами
void fill_random(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000; // Числа от 0 до 999
    }
}

// Функция копирования массива
void copy_array(int* src, int* dest, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// Функция печати массива
void print_array(int* arr, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) {
            printf(", ");
        }
        // Перенос строки для больших массивов
        if ((i + 1) % 10 == 0 && i < n - 1) {
            printf("\n ");
        }
    }
    printf("]\n");
}

int main() {
    int n;
    int *arr_c, *arr_asm, *arr_original;
    clock_t start, end;
    double time_c, time_asm;
    
    // Инициализация генератора случайных чисел
    srand(time(NULL));
    
    // k. Ввод количества элементов массива
    printf("Введите количество элементов массива: ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Количество элементов должно быть положительным!\n");
        return 1;
    }
    
    // Выделение памяти под массивы
    arr_c = (int*)malloc(n * sizeof(int));
    arr_asm = (int*)malloc(n * sizeof(int));
    arr_original = (int*)malloc(n * sizeof(int));
    
    if (arr_c == NULL || arr_asm == NULL || arr_original == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }
    
    // Заполнение массива случайными числами
    fill_random(arr_original, n);
    
    // Копируем исходный массив для обеих сортировок
    copy_array(arr_original, arr_c, n);
    copy_array(arr_original, arr_asm, n);
    
    printf("\n=== ИСХОДНЫЙ МАССИВ ===\n");
    printf("Количество элементов: %d\n", n);
    if (n <= 50) {
        printf("Элементы: ");
        print_array(arr_original, n);
    } else {
        printf("Первые 50 элементов: ");
        print_array(arr_original, 50);
        printf("... (показано 50 из %d элементов)\n", n);
    }
    
    // I. Сортировка на C с подсчетом времени
    printf("\n=== СОРТИРОВКА НА C ===\n");
    printf("Массив до сортировки: ");
    if (n <= 50) {
        print_array(arr_c, n);
    } else {
        printf("[показано 50 из %d]\n", n);
        print_array(arr_c, 50);
    }
    
    start = clock();
    c_insertion_sort(arr_c, n);
    end = clock();
    time_c = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Массив после сортировки: ");
    if (n <= 50) {
        print_array(arr_c, n);
    } else {
        printf("[показано 50 из %d]\n", n);
        print_array(arr_c, 50);
    }
    printf("Время выполнения: %.6f секунд\n", time_c);
    
    // m. Сортировка на ассемблере с подсчетом времени
    printf("\n=== СОРТИРОВКА НА АССЕМБЛЕРЕ ===\n");
    printf("Массив до сортировки: ");
    if (n <= 50) {
        print_array(arr_asm, n);
    } else {
        printf("[показано 50 из %d]\n", n);
        print_array(arr_asm, 50);
    }
    
    start = clock();
    asm_insertion_sort(arr_asm, n);
    end = clock();
    time_asm = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Массив после сортировки: ");
    if (n <= 50) {
        print_array(arr_asm, n);
    } else {
        printf("[показано 50 из %d]\n", n);
        print_array(arr_asm, 50);
    }
    printf("Время выполнения: %.6f секунд\n", time_asm);
    
    // n. Проверка массивов на упорядоченность
    printf("\n=== ПРОВЕРКА УПОРЯДОЧЕННОСТИ ===\n");
    printf("Массив после сортировки на C: %s\n", 
           is_sorted(arr_c, n) ? "✓ ОТСОРТИРОВАН" : "✗ НЕ ОТСОРТИРОВАН");
    printf("Массив после сортировки на ассемблере: %s\n", 
           is_sorted(arr_asm, n) ? "✓ ОТСОРТИРОВАН" : "✗ НЕ ОТСОРТИРОВАН");
    
    // o. Сравнение времени выполнения
    printf("\n=== СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ ===\n");
    printf("Время сортировки на C: %.6f секунд\n", time_c);
    printf("Время сортировки на ассемблере: %.6f секунд\n", time_asm);
    
    if (time_c > 0 || time_asm > 0) {
        if (time_asm < time_c) {
            double speedup = (time_c - time_asm) / time_c * 100;
            printf("Ассемблер быстрее на: %.2f%%\n", speedup);
        } else if (time_c < time_asm) {
            double speedup = (time_asm - time_c) / time_asm * 100;
            printf("C быстрее на: %.2f%%\n", speedup);
        } else {
            printf("Время выполнения одинаковое\n");
        }
        if (time_asm != 0) {
            printf("Отношение времени (C/ASM): %.3f\n", time_c / time_asm);
        }
    }
    
    // Дополнительная проверка: сравнение результатов обеих сортировок
    int arrays_equal = 1;
    for (int i = 0; i < n; i++) {
        if (arr_c[i] != arr_asm[i]) {
            arrays_equal = 0;
            printf("Расхождение на позиции %d: C=%d, ASM=%d\n", i, arr_c[i], arr_asm[i]);
            break;
        }
    }
    printf("Результаты сортировок %s\n", 
           arrays_equal ? "совпадают" : "не совпадают!");
    
    // Освобождение памяти
    free(arr_c);
    free(arr_asm);
    free(arr_original);
    
    return 0;
}