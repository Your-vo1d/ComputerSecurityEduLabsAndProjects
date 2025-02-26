#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct list {
    int data; //Поле для хранение элемента
    struct list* next; //Указатель на следующий участок списка
};

// Функция для создания нового элемента списка
struct list* createList(int data) {
    struct list* newList = (struct list*)malloc(sizeof(struct list));
    if (newList == NULL) { //Указатель нулевой
        return NULL;
    }
    newList->data = data;
    newList->next = NULL;
    return newList;
}

// Функция для добавления элемента в начало списка
bool addToHead(struct list** head, int data) {
    if (head)
    {
        struct list* newList = createList(data);
        if (newList == NULL) { //Указатель нулевой
            return false;
        }
        newList->next = *head; //Ссылка на следующий элемент нового элемента - текущая верхушка
        *head = newList; //Текущий вверхний элемент  - новый добавленный
        return true;
    }
    return false;
}

// Функция для добавления элемента в конец списка
bool addToEnd(struct list** head, int data) {
    if (head)
    {
        struct list* newList = createList(data); //Выделение памяти для нового элемента
        if (newList == NULL) { //Указатель нулевой
            return false;
        }
        if (*head == NULL) { //Если список пуст
            *head = newList;
        } else {
            struct list* lastList = *head;
            while (lastList->next != NULL) {
                lastList = lastList->next;
            }
            lastList->next = newList;
        }
        return true;
    }
    return false;
}

// Функция для вставки элемента перед заданным элементом
bool addBefore(struct list** head, int key, int data) {
    if (head)
    {
        if (*head == NULL) {
            return false;
        }
        if ((*head)->data == key) {
            return addToHead(head, data);
        }
        struct list* currList = *head;
        while (currList->next != NULL && currList->next->data != key) {
            currList = currList->next;
        }
        if (currList->next == NULL) { //Элемент не найден
            return false;
        }
        struct list* newList = createList(data);
        if (newList == NULL) {
            return false;
        }
        newList->next = currList->next;
        currList->next = newList;
        return true;
    }
    return false;
}

// Функция для вставки элемента после заданного элемента
bool addAfter(struct list** head, int key, int data) {
    if (head)
    {
        if (*head == NULL) { // Cписок пуст. Невозможно вставить элемент
            return false;
        }
        struct list* currList = *head;
        while (currList != NULL && currList->data != key) {
            currList = currList->next;
        }
        if (currList == NULL) { //Заданный элемент не найден. Невозможно вставить элемент
            return false;
        }
        struct list* newList = createList(data);
        if (newList == NULL) {
            return false;
        }
        newList->next = currList->next;
        currList->next = newList;
        return true;
    }
    return false;
}

// Функция для удаления элемента из начала списка
bool deleteFirst(struct list** head) {
    if (head)
    {
        if (*head == NULL) { //Список пуст. Невозможно удалить элемент
            return false;
        }
        struct list* temp = *head;
        *head = (*head)->next;
        free(temp);
        return true;
    }
    return false;
}

// Функция для удаления элемента из конца списка
bool deleteLast(struct list** head) {
    if (head)
    {
        if (*head == NULL) { //Список пуст. Невозможно удалить элемент
            return false;
        }
        if ((*head)->next == NULL) { //Если список состоит из 1 элемента
            free(*head);
            *head = NULL;
            return true;
        }
        struct list* prevList = NULL; //Предыдущий элемент
        struct list* currList = *head;//Текущий элемент
        while (currList->next != NULL) {
            prevList = currList;
            currList = currList->next;
        }
        free(currList);
        prevList->next = NULL;
        return true;
    }
    return false;
}

// Функция для удаления элемента по заданной позиции
bool deleteAt(struct list** head, int position) {
    if (head)
    {
        if (position <= 0) {
            return false;
        }
        if (*head == NULL) { //Список пуст. Невозможно удалить элемент
            return false;
        }
        if (position == 1) {
            return deleteFirst(head);
        }
        int count = 1; //Текущая позиция
        struct list* currList = *head; //Текущий элемент списка
        struct list* prevList = NULL; //Предыдущий элемент списка
        while (currList != NULL && count < position) {
            prevList = currList;
            currList = currList->next;
            count++;
        }
        if (currList == NULL) { //Заданная позиция превышает длину списка. Невозможно удалить элемент
            return false;
        }
        prevList->next = currList->next;
        free(currList);
        return true;
    }
    return false;
}

// Функция для вставки элемента по заданной позиции
bool insertAt(struct list** head, int position, int data) {
    if (head)
    {
    if (position == 1 || *head == NULL) { //Вставка в 1 позицию - в верхний элемент
        return addToHead(head, data);
    }
    int count = 1; //Текущая позиция
    struct list* currList = *head; //Текущий элемент списка
    while (currList != NULL && count < position - 1) {
        currList = currList->next;
        count++;
    }
    if (currList == NULL) { //Заданная позиция превышает длину списка. Невозможно вставить элемент
        return false;
    }
    struct list* newList = createList(data);
    if (newList == NULL) { //Указатель нулевой
        return false;
    }
    newList->next = currList->next;
    currList->next = newList;
    return true;
    }
    return false;
}

// Функция для поиска элемента в списке
bool search(struct list* head, int key, int* pos) {
    if (head == NULL) { //Список пуст. Невозможно найти элемент
        return false;
    }
    struct list* currList = head;
    int position = 1;
    while (currList != NULL) {
        if (currList->data == key) {
            *pos = position;
            return true;
        }
        currList = currList->next;
        position++;
    }
    return false; //Элемент не найден
}

// Функция для отображения элементов списка
void displayList(struct list* head) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }
    struct list* currList = head;
    printf("Elements: ");
    while (currList != NULL) {
        printf("%d ", currList->data);
        currList = currList->next;
    }
    printf("\n");
}

void freeList(struct list** head) {
    head = NULL;
}

bool deleteByValue(struct list** head, int value) {
    if (head)
    {
        if (*head == NULL) {
            return false;
        }
        struct list* currNode = *head;
        struct list* prevNode = NULL;

        if (currNode->data == value) {
            *head = currNode->next;
            free(currNode);
            return true;
        }

        while (currNode != NULL && currNode->data != value) {
            prevNode = currNode;
            currNode = currNode->next;
        }

        if (currNode == NULL) {
            return false;
        }

        prevNode->next = currNode->next;
        free(currNode);
        return true;
    }
    return false;
}


int main() {
    struct list* head = NULL;
    int choice, data, position;
    bool checkScanf;
    bool checkInput;
    do {
        printf("\n");
        printf("Select an operation:\n");
        printf("1. Add an item to the top of the list\n");
        printf("2. Add an item to the end of the list\n");
        printf("3. Insert an element before the specified element\n");
        printf("4. Insert an element after the specified element\n");
        printf("5. Delete the first item in the list\n");
        printf("6. Delete the last item in the list\n");
        printf("7. Delete an item at a given position\n");
        printf("8. Display list items\n");
        printf("9. Destroy list\n");
        printf("10. Insert an element at a given position\n");
        printf("11. Find an item in the list\n");
        printf("12. Deleting by key.\n");
        printf("13. Init\n");        
        printf("14. Exit\n");
        printf("Enter your choice: ");
        checkScanf = scanf("%d", &choice);

            switch (choice) {
                case 1:
                        printf("Enter the value of the new element: ");
                        checkScanf = scanf("%d", &data);
                        if (!checkScanf) {
                            printf("Incorrect input.\n");
                            break;
                        }
                        else if (addToHead(&head, data)) {
                            printf("The item has been added to the top of the list.\n");
                        }
                        else
                            printf("Error when adding an element.\n");
                    break;
                case 2:
                    printf("Enter the value of the new element: ");
                    checkScanf = scanf("%d", &data);
                    if (!checkScanf) {
                        printf("Incorrect input.\n");
                        break;
                    }
                    else if (addToEnd(&head, data)) {
                        printf("The item is added to the end of the list.\n");
                    }
                    else
                        printf("Error when adding an element.\n");
                    break;
                case 3:
                    printf("Enter the element before which you want to insert a new element: ");
                    checkScanf = scanf("%d", &data);
                    if (!checkScanf) {
                        printf("Incorrect input.\n");
                        break;
                    }
                    printf("Enter the value of the new element: ");
                    checkInput = scanf("%d", &position);
                    if (!checkInput) {
                        printf("Incorrect input.\n");
                        break;
                    }
                    if (addBefore(&head, data, position))
                        printf("The element is inserted before the specified element.\n");
                    else
                        printf("Error when adding an element.\n");
                    break;
                case 4:
                    printf("Enter the element after which you want to insert a new element: ");
                    checkScanf = scanf("%d", &data);
                    if (!checkScanf) {
                        printf("Incorrect input.\n");
                        break;
                    }
                    printf("Enter the value of the new element: ");
                    checkInput = scanf("%d", &position);
                    if (!checkInput) {
                        printf("Incorrect input.\n");
                        break;
                    }
                    if (addAfter(&head, data, position))
                        printf("The element is inserted after the specified element.\n");
                    else
                        printf("Error when inserting an element.\n");
                    break;
                case 5:
                    if (deleteFirst(&head))
                        printf("The first item has been removed from the list.\n");
                    else
                        printf("Error when deleting an element.\n");
                    break;
                case 6:
                    if (deleteLast(&head))
                        printf("The last item has been removed from the list.\n");
                    else
                        printf("Error when deleting an element.\n");
                    break;
                case 7:
                    printf("Enter the position of the item you want to delete: ");
                    checkScanf = scanf("%d", &position);
                    if (!checkScanf) {
                        printf("Incorrect input.\n");
                        break;
                    }
                    if (deleteAt(&head, position))
                        printf("The element was deleted at the specified position.\n");
                    else
                        printf("Error when deleting an element.\n");
                    break;
                case 8:
                    displayList(head);
                    break;
                case 9:
                    freeList(&head);
                    printf("FDS");
                    break;
                case 10:
                    printf("Enter the position where you want to insert the new element: ");
                    if (scanf("%d", &position) != 1) {
                        printf("Incorrect input.\n");
                        break;
                    }
                    printf("Enter the value of the new element: ");
                    if (scanf("%d", &data) != 1) {
                        printf("Incorrect input.\n");
                        break;
                    }
                    if (insertAt(&head, position, data))
                        printf("The element is inserted at the specified position.\n");
                    else
                        printf("Error when inserting an element.\n");
                    break;
                case 11:
                    int  position;
                    printf("Enter the item you want to find: ");
                    checkScanf = scanf("%d", &data);
                    if (!checkScanf) {
                        printf("Incorrect input.\n");
                        break;
                    }
                    if (!search(head, data, &position))
                        printf("The item was not found in the list.\n");
                    else
                        printf("Position: %d", position);
                    break;
                case 12:
                
                    printf("Enter the value to delete: ");
                    checkScanf = scanf("%d", &data);
                    if (checkScanf != 1) {
                        printf("Invalid input. Please try again.\n");
                        break;
                    }
                    if (deleteByValue(&head, data))
                        printf("Element deleted from the list.\n");
                    else
                        printf("Error occurred while deleting element.\n");
                    break;
                case 13:
                    int element;
                    checkScanf = scanf("%d",&element);
                    if(checkScanf)
                    {
                        head = createList(element);
                        printf("Complited.\n");
                    }
                    else
                        printf("Unsuccess.\n");
                case 14:
                    printf("Exit.\n");
                    break;
                default:
                    printf("Incorrect input..\n");
                    break;
            }
        while (getchar() != '\n'); 
    } while (choice != 14);

    freeList(&head);

    return 0;
}