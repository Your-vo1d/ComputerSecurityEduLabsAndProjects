#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Создание структуры стек
struct stack 
{
    int *top; // Указатель последнего элемента стека
    int *base; //Указатель на 1 элемент стека
    size_t size; //Определяем размер стека
};


//Функция создание стека
struct stack *createStack(size_t size)
{
    struct stack *stack = (struct stack*)malloc(sizeof(struct stack)); //Выделение памяти для стека
    if (stack) //Проверка выделения памяти
    {
        stack -> base = (int*)malloc(size  *sizeof(int)); //Выделение памяти для элементов стека
        if (stack->base) //Проверка выделение памяти для элементов стека
        {
            stack -> top = NULL; //Стек пустой
            stack -> size = size; //Размер стека
        }
        else
        {
            free(stack); //Освобождение памяти
            return NULL;
        }
    }
    else 
        return NULL;
    return stack;
}


//Проверка массива на пустоту
bool isEmpty(struct stack *stack)
{
    if (stack)
    {
        return (stack -> top == NULL);
    }
    else
        return false;
}


//Функция занесение элемента в стек
bool push(struct stack *stack, int item)
{
    if (stack)//Проверка существования стека
    {
        if (!stack -> base)//Проверка выделения памяти для элементов стека
        {
            return false;
        }
        else
        {
            if (stack -> top == stack -> base + stack -> size - 1) //Проверка заполнения стека
            {
                return false;
            }
            if (stack -> top == NULL)
            {
                stack -> top = stack -> base;
                *(stack -> top) = item;
            }
            else
                *(++stack -> top) = item; //Внесение элемента в стек
            return true;
        }
    }
    else
        return false;
}


//Функция удаления вершины стека
bool pop(struct stack *stack, int *item)
{
    if (stack)//Проверка существования стека
    {
        if (!stack -> base)//Проверка выделения памяти для элементов стека
        {
            return false;
        }
        else
        {
            if (isEmpty(stack)) //Проверка заполнения стека
            {
                return false;
            }
            if (item)
            {
                *item = *(stack -> top); //Внесение элемента в стек
                if (stack -> top != stack -> base)
                {
                    stack -> top--;
                }
                else
                    stack -> top = NULL;
                return true;
            }
            else
                return false;
        }
    }
    else
        return false;
}


//Функция просмотра вершины стека
bool seeTop(struct stack *stack, int *topS)
{
    if (stack)
    {
        if (!stack -> base)
        {
            return false;
        }
        else
        {
            if(!isEmpty(stack))
            {
                if (topS)
                {
                    *topS = *(stack -> top);
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
    }
    else
        return false;
}


//Функция проверки инициализации стека
bool isInit(struct stack *stack)
{
    return (stack && stack -> base);
}


//Функция уничтожения стека
void destroy(struct stack **stack)
{
    if (stack)
    {
    free((*stack) -> base);
    (*stack) -> base = NULL;
    (*stack) -> size = 0;
    (*stack) -> top = NULL;
    free(*stack);
    *stack = NULL;
    }
}


//Функция вывода элементов стека
void printStack(struct stack *stack)
{
    if (stack)
    {
        if (!isEmpty(stack))
        {
            int *temp;
            temp = stack -> base;
            while (temp <= stack -> top)
            {
                printf("%d ", *temp);
                temp++;
            }
        }
        else
            printf("Stack is empty.\n");
    }
    else
        printf("Stack not created.\n");
    
}


//Функция очистки стека
void clearStack(struct stack *stack)
{
    stack -> top = NULL;
}
void main()
{
    struct stack *St;
    size_t size; //Переменная для хранения размера стека
    printf("Enter a size of stack: "); //Сообщение приглашение
    int resultScanfSize = 0; //Переменная для проверка ввода размера стека
    resultScanfSize = scanf("%zu", &size); //Считывание размера стека
    bool cycle_contin = true;

    if (resultScanfSize && size > 0) //Проверка корректности ввода размера стека
    {
        St = createStack(size); //Создание стека
        if (St)
        {
            while (cycle_contin)
            {
                    int answer = 0;
                    printf("\n");
                    printf("1. Check empty stack\n");
                    printf("2. Push element to stack\n");
                    printf("3. Pop element from stack\n");
                    printf("4. Clear stack\n");
                    printf("5. Print stack\n");
                    printf("6. Destroy stack\n");
                    printf("7. Print top stack\n");
                    printf("8. Exit\n\n");

                    printf("Enter your choice: ");
                    scanf("%d", &answer);
                    printf("%d", answer);
                    printf("\n");
                        switch (answer)
                        {
                        case 1:
                        {
                            bool checkEmpty;
                            checkEmpty = isEmpty(St);
                            if (checkEmpty)
                            {
                                printf("Stack is empty.\n");
                            }
                            else if (St)
                            {
                                printf("Stack isn't empty.\n");
                            }
                            else
                                printf("No stack.\n");
                            break;
                        }
                        case 2:
                        {
                            int num = 0; // Переменная для вводимого элемента
                            printf("Enter element to add to stack: "); //Сообщение - приглашение
                            int check_scanf_push = scanf("%d", &num); //Считывание вводимого числа
                            if (check_scanf_push)
                            {    
                                bool check_push;
                                check_push = push(St, num);
                                if (check_push)
                                {
                                    printf("Success!\n");
                                }
                                else if (St)
                                {
                                    printf("Unsuccess!\n");
                                }
                                else
                                printf("No stack.\n");
                            }
                            else{
                                printf("Error input elemet.\n");
                                cycle_contin = false;
                            }
                            break;
                        }
                        case 3:
                        {
                            int item = 0; // Переменная, в которой будет хранится элемент, взятый со стека
                            bool check_pop;
                            check_pop = pop(St, &item);
                            if (check_pop)
                            {
                                printf("Success! Elemets - %d\n", item);
                            }
                            else if (St)
                            {
                                printf("Unsuccess!\n");
                            }
                            else
                                printf("No stack.\n");
                            break;
                        }
                        case 4:
                        {
                            if (St)
                            {
                            clearStack(St);
                            printf("Completed\n");
                            }
                            else
                                printf("No stack.\n");
                            break;
                            
                        }
                        case 5:
                        {
                            if (St)
                            {
                            printStack(St);
                            }
                            else
                                printf("No stack.\n");
                            break;
                        }
                        case 6:
                        {
                            if (St)
                            {
                                destroy(&St);
                                printf("Completed\n");
                            }
                            else
                                printf("No stack");
                            break;
                        }
                        case 7:
                        {
                            int topItem; //Переменная для хранения верхнего элемента стека
                            bool checkSeeTop;
                            checkSeeTop = seeTop(St, &topItem);
                            if (checkSeeTop)
                            {
                                printf("Success! Elemet - %d. ", topItem);
                            }
                            else if (isEmpty(St))
                            {
                                printf("Stack is empty.\n");
                            }
                            else if (St)
                            {
                                printf("Unsuccess!\n");
                            }
                            else
                                printf("No stack");
                            break;
                        }         
                        case 8:
                        {
                            printf("Exit");
                            if (St)
                            {
                            free((St) -> base);
                            (St) -> base = NULL;
                            (St) -> size = 0;
                            (St) -> top = NULL;
                            free(St);
                            St = NULL;
                            }
                            cycle_contin = false;
                            break;
                        }

                        default: //Ввод ошибочного значения
                        {
                            printf("Error input\n"); //Сообщение об ошибке
                            cycle_contin = false;
                            break;
                        }

                        }
            }
            }
        else
            {
                printf("Error create stack.\n");
            }
    }
    else
        printf("Error entering stack size.\n");
}