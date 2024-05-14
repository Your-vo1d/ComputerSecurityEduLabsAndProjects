#include "BalancedSearchTree.h"
#include <algorithm>

BalancedSearchTree::BalancedSearchTree(int n)
{
    root = nullptr; // Инициализируем корень как nullptr
    bool isFixed = false;
    std::vector<int> keys;
    for (int i = 0; i < n; ++i)
    {
        keys.push_back(rand() % 100); // Генерируем случайные ключи от 0 до 99
    }

    for (int i = 0; i < n; i++)
    {
        root = addNode(root, keys[i]); // Вставляем ключи в дерево поиска
    }
}

Node *BalancedSearchTree::addNode(Node *root, int key)
{
}