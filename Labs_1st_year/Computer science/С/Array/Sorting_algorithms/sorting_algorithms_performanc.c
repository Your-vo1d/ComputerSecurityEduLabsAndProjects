#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Функция rand_in_range возвращает случайный int
int rand_in_range(int min, int max)
{
    if (min == max){
        return min;
    }
    if (min > max){
        int tmp = min;
        min = max;
        max = tmp;
    }
    return rand() % ( max - min + 1 ) + min;
}

// Функция check_for_sorted_array проверяет отсортирован ли массив, возвращает true если массив отсортирован и false если нет
bool check_for_sorted_array(int array[], int size)
{
    for (int i = 0; i < size - 1; i++){
        if (array[i] > array[i+1]){
            return false;
        }
    }
    return true;
}

// Функция create_array возвращает указатель на начало области памяти массива
int *create_array(size_t array_size, int min, int max)
{
    int *array = malloc(array_size * sizeof(int));
    if (array == NULL){
         return NULL;
    }
    for (size_t i = 0; i < array_size; i++){
       array[i] = rand_in_range(min, max);
    }
    return array;
}

// Функция copy_array возвращает указатель на начало копии массива
int *copy_array(int *array, size_t size)
{
    int *array_copy = malloc(size * sizeof(int));
    if (array_copy){
        for (size_t i = 0; i < size; i++){
            array_copy[i] = array[i];
        }
    }
    return array_copy;
}

// Функция сортировки пузырьком
void bubble_sort(int *array, size_t size)
{
    if (array){
        for (size_t i = 0; i < size - 1; i++){
            for (size_t j = 0; j < size - 1 - i; j++) {
                if (array[j] > array[j + 1]) {
                    int tmp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = tmp;
                }
            }
        }
    }
}

// Функция сортировки прямого выбора
void selection_sort(int *array, size_t size)
{
    if (array){
        for (size_t i = 0; i < size - 1; i++) {
            size_t min_idx = i;
            for (size_t j = i + 1; j < size; j++) {
                if (array[j] < array[min_idx]) {
                    min_idx = j;
                }
            }
            int temp = array[min_idx];
            array[min_idx] = array[i];
            array[i] = temp;
        }
    }
}

// Функция сортировки прямого вставки
void insertion_sort(int *array, size_t size)
{
    if (array){
        for (size_t i = 1; i < size; i++){
            int new_element = array[i];
            int location = i - 1;
            while (location >= 0 && array[location] > new_element){
                array[location + 1] = array[location];
                location--;
            }
            array[location + 1] = new_element;
        }
    }
}

int main()
{
    srand((unsigned int) time(NULL)); // Запуск ГПСЧ

    printf("Введите размер массива:\n");
    int array_size;
    int input_result = scanf("%d", &array_size);

    if (input_result == 0 || array_size <= 0){
        printf("Размер массива должен быть целым числом больше 0.\n");
        return EXIT_FAILURE;
    }

    int *array = create_array(array_size, 1, 100);
    if (!array){
        printf("Ошибка выделения памяти.\n");
        return EXIT_FAILURE;
    }

    int *array_copy1 = copy_array(array, array_size);
    if (!array_copy1){
        free(array);
        return EXIT_FAILURE;
    }

    clock_t start = clock();
    bubble_sort(array_copy1, array_size);
    clock_t end = clock();
    double bubble_sort_time = (double)(end - start) / CLOCKS_PER_SEC;
    if (check_for_sorted_array(array_copy1, array_size)) 
        printf("Bubble sort: отсортировано за %f секунд\n", bubble_sort_time);
    else 
        printf("Bubble sort: не отсортировано\n");

    int *array_copy2 = copy_array(array, array_size);
    if (!array_copy2){
        free(array);
        free(array_copy1);
        return EXIT_FAILURE;
    }

    start = clock();
    selection_sort(array_copy2, array_size);
    end = clock();
    double selection_sort_time = (double)(end - start) / CLOCKS_PER_SEC;
    if (check_for_sorted_array(array_copy2, array_size)) 
        printf("Selection sort: отсортировано за %f секунд\n", selection_sort_time);
    else 
        printf("Selection sort: не отсортировано\n");

    int *array_copy3 = copy_array(array, array_size);
    if (!array_copy3){
        free(array);
        free(array_copy1);
        free(array_copy2);
        return EXIT_FAILURE;
    }

    start = clock();
    insertion_sort(array_copy3, array_size);
    end = clock();
    double insertion_sort_time = (double)(end - start) / CLOCKS_PER_SEC;
    if (check_for_sorted_array(array_copy3, array_size)) 
        printf("Insertion sort: отсортировано за %f секунд\n", insertion_sort_time);
    else 
        printf("Insertion sort: не отсортировано\n");

    // Освобождение памяти
    free(array);
    free(array_copy1);
    free(array_copy2);
    free(array_copy3);

    return 0;
}
