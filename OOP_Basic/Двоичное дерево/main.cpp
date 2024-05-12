#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "BinaryTree/BinaryTree.h"
#include "BinarySearchTree/BinarySearchTree.h"

int main() {
    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Создание объектов BinaryTree
    BinaryTree initialTree(10), copiedTree(initialTree), emptyTree;

    // Вывод информации о BinaryTree
    std::cout << "Дерево, созданное конструктором с параметром" << std::endl;
    initialTree.printTree();
    std::cout << "Дерево, созданное конструктором копирования" << std::endl;
    copiedTree.printTree();
    std::cout << "Дерево, созданное конструктором по умолчанию:" << std::endl;
    emptyTree.printTree();
    std::cout << "Минимальное значение: " << initialTree.min() << std::endl;
    std::cout << "Максимальное значение: " << initialTree.max() << std::endl;
    std::cout << "Обход по уровням: ";
    initialTree.leaves();
    std::cout << "Обход Л-К-П: ";
    int arrayLNR[10];
    initialTree.LNR(arrayLNR);
    for (int i = 0; i < 10; i++) {
        std::cout << arrayLNR[i] << " ";
    }
    std::cout << std::endl;

    std::cout << std::endl;
    std::cout << "____________Деревья поиска____________" << std::endl;
    std::cout << std::endl;

    // Создание объектов BinarySearchTree
    BinarySearchTree initialSearchTree(10), copiedSearchTree(initialSearchTree), emptySearchTree;

    // Вывод информации о BinarySearchTree
    std::cout << "Дерево поиска, созданное конструктором с параметром (кол-во элементов дерева)" << std::endl;
    initialSearchTree.printTree();
    std::cout << "Дерево поиска, созданное конструктором копирования" << std::endl;
    copiedSearchTree.printTree();
    std::cout << "Дерево поиска, созданное конструктором по умолчанию" << std::endl;
    emptySearchTree.printTree();

    // Создание BinarySearchTree из вектора
    std::vector<int> vec;
    int n;
    std::cout << "Введите размер вектора: ";
    while (!(std::cin >> n) || n <= 0) {
        std::cout << "Некорректный ввод. Пожалуйста, введите положительное целое число: ";
        std::cin.clear(); // Сбрасываем флаг ошибки ввода
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода
    }
    
    // Ввод элементов вектора
    std::cout << "Введите " << n << " элементов: ";
    for (int i = 0; i < n; ++i) {
        int element;
        while (!(std::cin >> element)) {
            std::cout << "Некорректный ввод. Пожалуйста, введите целое число: ";
            std::cin.clear(); // Сбрасываем флаг ошибки ввода
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода
        }
        vec.push_back(element); // Добавляем элемент в конец вектора
    }
    BinarySearchTree treeVector(vec);
    treeVector.printTree();

    // Вывод информации о BinarySearchTree из вектора
    std::cout << "Минимальное значение: " << initialSearchTree.min() << std::endl;
    std::cout << "Максимальное значение: " << initialSearchTree.max() << std::endl;
    std::cout << "Обход по уровням: ";
    initialSearchTree.leaves();
    std::cout << "Обход Л-К-П: ";
    int arraySearchLNR[10];
    initialSearchTree.LNR(arraySearchLNR);
    for (int i = 0; i < 10; i++) {
        std::cout << arraySearchLNR[i] << " ";
    }
    std::cout << std::endl;

    // Добавление узла в дерево
    int keyToAdd = 42; // Введите ключ, который нужно добавить
    initialSearchTree.addNode(initialSearchTree.getRoot(), keyToAdd);
    std::cout << "Узел с ключом " << keyToAdd << " успешно добавлен в дерево." << std::endl;
    initialSearchTree.printTree();

    // Поиск и удаление узла
    BinaryTree::Node *nodeToRemove = initialSearchTree.findNode(initialSearchTree.getRoot(), keyToAdd);
    if (nodeToRemove != nullptr) {
        initialSearchTree.removeNode(initialSearchTree.getRoot(), keyToAdd);
        std::cout << "Узел с ключом " << keyToAdd << " успешно удален из дерева." << std::endl;
    } else {
        std::cout << "Узел с ключом " << keyToAdd << " не найден в дереве." << std::endl;
    }
    initialSearchTree.printTree();

    // Удаление узла из дерева
    int value;
    std::cout << "Введите значение для удаления из дерева: ";
    while (!(std::cin >> value)) {
        std::cout << "Некорректный ввод. Пожалуйста, введите положительное целое число: ";
        std::cin.clear(); // Сбрасываем флаг ошибки ввода
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода
    }
    treeVector.removeNode(treeVector.getRoot(), value);
    treeVector.printTree();

    return 0;
}