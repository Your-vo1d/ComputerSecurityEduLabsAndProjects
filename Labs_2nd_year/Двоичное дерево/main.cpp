#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "BinaryTree/BinaryTree.h"
#include "BinarySearchTree/BinarySearchTree.h"
#include "BalancedSearchTree/BalancedSearchTree.h"

int main() {
    std::cout << "____________Деревья поиска____________" << std::endl;
    std::cout << std::endl;

    // Создание объектов BinarySearchTree
    BinarySearchTree initialSearchTree(10), copiedSearchTree(initialSearchTree), emptySearchTree;
    BinarySearchTree copySearchTree;
    copySearchTree = initialSearchTree;
    // Вывод информации о BinarySearchTree
    std::cout << "Дерево поиска, созданное конструктором с параметром (кол-во элементов дерева)" << std::endl;
    initialSearchTree.printTree();
    std::cout << "Дерево поиска, равное дереву поиска, созданного конструктором с параметром (кол-ва элементов)" << std::endl;
    copySearchTree.printTree();
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
    std::cout << "Обход К-Л-П: ";
    int arraySearchLNR[10];
    for (int i = 0; i < 10; i++)
        arraySearchLNR[i] = 0;
    initialSearchTree.NLR(arraySearchLNR);
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
    Node *nodeToRemove = nullptr;
    std::cin >> keyToAdd;
    nodeToRemove = initialSearchTree.findNode(initialSearchTree.getRoot(), keyToAdd);
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
    std::cout << "____________Деревья поиска____________" << std::endl;
    BalancedSearchTree tmp(10);
    tmp.printTree();
        std::cout << "____________Деревья поиска____________" << std::endl;
        std::cout << "____________Деревья поиска____________" << std::endl;
    return 0;
}