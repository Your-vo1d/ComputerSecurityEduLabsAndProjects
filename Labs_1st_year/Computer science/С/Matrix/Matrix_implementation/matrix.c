#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Функиця rand_in_range возвращает случайное целочисленное значение в диапазоне
int rand_in_range(int min, int max)
{
    if (min == max)
    { // Если границы равны, то возвращаем любую из границ
        return min;
    }
    if (min > max)
    { // Если начальная граница больше чем финишная граница
        int tmp = min;
        min = max;
        max = tmp;
    }
    return (rand() % (max - min + 1) + min);
}


//Создание и заполнение матрицы
int **CreateMatrix(size_t row, size_t column, int min, int max)
{
    if (row == 0 || column == 0)
        return NULL;
    int **dmas = (int **)malloc(row * sizeof(int *)); //Выделение памяти для строк матрицы
    if (dmas) //Проверка, выделили ли память для матрицы
    {
        for (int i = 0; i < row; i++)
        {
            dmas[i] = (int *)malloc(column * sizeof(int));//Выделение памяти для элементов матрицы
        }
        
        for (int i = 0; i < row; i++) //Заполнение матрицы 
        {
            if (dmas[i]) //Проверка, выделили ли память для строки
            {
                for (int j = 0; j < column; j++) //Проход по элементам
                {
                    int temp;
                    temp = 0;
                    int check_scanf = scanf("%d", &temp);
                    if (check_scanf)
                    {
                        dmas[i][j] = temp;
                    }
                     else
                    {
                    for(int k = 0; k < row; k++)
                    {
                        free(dmas[k]);
                    }
                    free(dmas);
                    return NULL;
                    }
                    //dmas[i][j] = rand_in_range(min, max); //Заполнение ячейки рандомным целочисленным числом
                }
            }
            else
            {
                for(int k = 0; k < row; k++)
                    {
                        free(dmas[k]);
                    }
                free(dmas);
                return NULL;
            }
        } 
    }
    else
        return NULL;
    return dmas;
}

//Вывод матрицы
void PrintMatrix(int **dmas, size_t row, size_t column)
{
    if (dmas)
    {
        for (int i = 0; i < row; i++)
        {
            if (dmas[i])
            {
                for (int j = 0; j < column; j++)
                {
                    printf("%d ", dmas[i][j]);
                }
                printf("\n");
            }
            else
                printf("Can't show  row the matrix\n");
        }
        printf("\n");
    }
    else
        printf("Can't show the matrix\n");
}

//Проверка симметричности относительно главной диагонали
int check_simmetry(int **dmas, size_t row, size_t column)
{
    if (dmas) //Проверка существования матрицы
    {
        if (row == column)
        {
            for (int i = 0; i < row; i++)
            {
                if (dmas[i]) //Проверка существования строки
                {
                    for (int j = 0; j < column; j++)
                    {
                        if (dmas[i][j] != dmas[j][i]) //Если элементы не симметричны, то меняем флаг
                        {
                            return 0;
                        }
                    }
                }
                else
                    return -1;
            }
        }
        else
            return -2;
    }
    else
        return -1;

    return 1;
}

//Подсчет количество особых элементов
int numbers_special_elements(int **dmas, size_t row, size_t column)
{
    int count = 0; //Переменная - счетчик для подсчета количества специальных элементов
    int sum = 0; //Переменная для хранения суммы элементов i-ой строки
    if (dmas) //Проверка существования матрицы
    {
        for (int i = 0; i < row; i++) 
        {
            if (dmas[i])
            {
                for (int j = 0; j < column; j++)
                {
                    sum += dmas[i][j]; //Нахождение суммы элементов строки
                }
                for (int k = 0; k < column; k++)
                {
                    if (dmas[i][k] > sum - dmas[i][k]) //Проверка элемента на соответствие особому
                    {
                        
                        printf("%d ", dmas[i][k]);
                        count += 1;
                    }
                }
                sum = 0; //Обнуление суммы перед переходом к другой строке
            }
            else {
                count = -1;
                return count;
            }
        }
    }
    else
        count = -1;
    return count;
}

int sum_numbers_in_zone(int **dmas, size_t row, size_t column, int *summa)
{
    *summa = 0;
    int mid = row / 2;
    if (dmas && summa)
    {
        if (row == column)
        {
            for (int i = 0; i < mid; i++)
            {
                if (dmas[i])
                {
                    for (int j = i; j < column - i; j++)
                    {
                        *summa += dmas[i][j];
                        *summa += dmas[row - 1 - i][j];
                    }
                }
                else
                    return 0;
            }
            if (row % 2 != 0)
            {
                *summa += dmas[mid][mid];
            }
        }
        else
            return -1;
    }
    else
        return 0;
    return 1;
}


int main()
{
    srand(time(NULL));        // запуск ГПСЧ
    int start = 0;            // Инициализация начала диапазона рандомных чисел
    int finish = 0;           // Инициализация конца диапазона рандомных чисел
    size_t count_rows = 0;    // Инициализация количество строк в матрице
    size_t count_columns = 0; // Инициализация количество столбцов в матрице

    printf("Enter a count of rows and column: ");
    int result_scanf_rc = scanf("%zu %zu", &count_rows, &count_columns); //Ввод количества строк и стобцов матрицы
    if (result_scanf_rc == 2) //Проверка корректности вводимых данных
    {

        //printf("Enter the beginning and end of a range of numbers: ");
        //int result_scanf_sf = scanf("%d%d", &start, &finish); //Ввод диапазона значений элементов матрицы
        //if (result_scanf_sf == 2) //Проверка корректности ввода диапазона значений элементов
        //{ 
            int **matrix = CreateMatrix(count_rows, count_columns, start, finish); //Создание матрицы
            if (matrix) //Проверка существования матрицы
            {
                //PrintMatrix(matrix, count_rows, count_columns); //Вывод матрицы

                //Задание №1
                int result_check_sim = check_simmetry(matrix, count_rows, count_columns); //Проверка симметричности

                if (result_check_sim == 1) //Если флаг 1, то матрица квадратная и симметричная относительно главной диагонали
                {
                    printf("The matrix is quadratic and symmetric.\n");
                }
                else if (result_check_sim == 0) //Если флаг 0, то матрицы квадратная и несимметричная
                {
                    printf("The matrix is quadratic, but not symmetry.\n");
                }
                else if(result_check_sim == - 2)
                {
                    printf("The matrix isn't quadratic.\n");
                }
                else //Иначе матрицы не создана
                    printf("The matrix was not created.\n");

                //Задание №2
                int count_special_numbers = numbers_special_elements(matrix, count_rows, count_columns); //Вызов функции подсчета количества особых чисел
                if (count_special_numbers != -1) //Если функция отработала адекватно
                {
                    printf("Count special numbers: %d\n", count_special_numbers);
                }
                else
                    printf("Matrix isn't created.\n");

                //Задание №3
                int sum = 0;
                int check_sum_in_zone = sum_numbers_in_zone(matrix, count_rows, count_columns, &sum);
                if (check_sum_in_zone == 1)
                {
                    printf("Sum in zone =%d\n", sum);
                }
                else if (check_sum_in_zone == -1)
                    printf("The matrix isn't quadratic\n");
                else
                    printf("Matrix isn't created or pointer to the sum is empty.\n");


                for (int i = 0; i < count_rows; i++)
                {
                    free(matrix[i]);
                }
                free(matrix);
            }
            else
            {
                printf("Error create matrix.\n");
            }
        //}
        //else
        //    printf("Error entering the beginnin or end of a range of random numbers. \n");
    }
    else if (result_scanf_rc == 2 && (count_columns == 0 && count_rows == 0))
    {
        printf("Not point to create matrix with 0 rows/columns.\n");
    }
    else
        printf("Error entering the number of rows or columns of the matrix.\n");

    return 0;
}
