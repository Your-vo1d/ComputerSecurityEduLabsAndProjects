#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// Создание структуры стек
struct stack {
    int *top;   // Указатель последнего элемента стека
    int *base;  // Указатель на первый элемент стека
    size_t size; // Размер стека
};

// Функция создания стека
struct stack *createStack(size_t size) {
    struct stack *stack = (struct stack *)malloc(sizeof(struct stack)); // Выделение памяти для стека
    if (stack) { // Проверка выделения памяти
        stack->base = (int *)malloc(size * sizeof(int)); // Выделение памяти для элементов стека
        if (stack->base) { // Проверка выделения памяти для элементов стека
            stack->top = stack->base - 1; // Инициализация вершины стека
            stack->size = size; // Размер стека
        } else {
            free(stack); // Освобождение памяти
            return NULL;
        }
    } else {
        return NULL;
    }
    return stack;
}

// Проверка массива на пустоту
bool isEmpty(struct stack *stack) {
    return stack && stack->top < stack->base;
}

// Функция занесения элемента в стек
bool push(struct stack *stack, int item) {
    if (!stack || !stack->base) // Проверка существования стека и выделения памяти для элементов стека
        return false;

    if (stack->top >= stack->base + stack->size - 1) // Проверка заполнения стека
        return false;

    *(++stack->top) = item; // Внесение элемента в стек
    return true;
}

// Функция удаления вершины стека
bool pop(struct stack *stack, int *item) {
    if (!stack || !stack->base || isEmpty(stack)) // Проверка существования стека, выделения памяти для элементов стека и заполненности стека
        return false;

    *item = *(stack->top--); // Извлечение элемента из стека
    return true;
}

// Функция уничтожения стека
void destroy(struct stack **stack) {
    if (stack && *stack) {
        free((*stack)->base);
        free(*stack);
        *stack = NULL;
    }
}

// Функция проверки приоритета операторов
int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

// Функция преобразования инфиксного выражения в постфиксное
void infixToPostfix(char *infix, char *postfix) {
    struct stack *stack = createStack(strlen(infix));
    int k = 0;
    for (int i = 0; infix[i]; i++) {
        if (infix[i] == ' ') {
            continue;
        }
        if (isalnum(infix[i])) {
            postfix[k++] = infix[i];
        } else if (infix[i] == '(') {
            push(stack, infix[i]);
        } else if (infix[i] == ')') {
            int temp;
            while (!isEmpty(stack) && *(stack->top) != '(') {
                pop(stack, &temp);
                postfix[k++] = (char)temp;
            }
            pop(stack, &temp); // Удаление '(' из стека
        } else {
            while (!isEmpty(stack) && precedence(infix[i]) <= precedence(*(stack->top))) {
                int temp;
                pop(stack, &temp);
                postfix[k++] = (char)temp;
            }
            push(stack, infix[i]);
        }
    }
    while (!isEmpty(stack)) {
        int temp;
        pop(stack, &temp);
        postfix[k++] = (char)temp;
    }
    postfix[k] = '\0';
    destroy(&stack);
}

// Функция вычисления значения постфиксного выражения
int evaluatePostfix(char *postfix) {
    struct stack *stack = createStack(strlen(postfix));
    if (!stack) return 0;
    for (int i = 0; postfix[i]; i++) {
        if (isdigit(postfix[i])) {
            push(stack, postfix[i] - '0');
        } else {
            int val1, val2;
            pop(stack, &val2);
            pop(stack, &val1);
            switch (postfix[i]) {
                case '+': push(stack, val1 + val2); break;
                case '-': push(stack, val1 - val2); break;
                case '*': push(stack, val1 * val2); break;
                case '/': push(stack, val1 / val2); break;
            }
        }
    }
    int result;
    pop(stack, &result);
    destroy(&stack);
    return result;
}

// Функция проверки выражения
bool checkExpression(char *expression) {
    char *equalSign = strchr(expression, '=');

    if (!equalSign) {
        return false;
    }

    *equalSign = '\0';

    char *left = expression;
    char *right = equalSign + 1;

    char postfixLeft[200], postfixRight[200];

    infixToPostfix(left, postfixLeft);
    infixToPostfix(right, postfixRight);

    int valueLeft = evaluatePostfix(postfixLeft);
    int valueRight = evaluatePostfix(postfixRight);

    *equalSign = '=';

    return valueLeft == valueRight;
}

int main() {
    printf("Enter an expression:\n");
    char expression[200]; // Массив для хранения выражения
    fgets(expression, 200, stdin); // Ввод текста

    // Удаление символа перехода на новую строку
    expression[strcspn(expression, "\n")] = 0;

    if (checkExpression(expression)) {
        printf("Is correct.\n");
    } else {
        printf("Isn't correct.\n");
    }

    return 0;
}
