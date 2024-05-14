#include "BinarySearchTree.h"

BinarySearchTree::BinarySearchTree(int n)
{
    root = nullptr; // Инициализируем корень как nullptr

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

BinarySearchTree::BinarySearchTree(const std::vector<int> &vec)
{
    root = nullptr;
    for (int key : vec)
    {
        root = addNode(root, key);
    }
}

Node *BinarySearchTree::addNode(Node *root, int key)
{
    Node *newNode = new Node(key);

    if (root == nullptr)
    {
        return newNode;
    }

    Node *current = root;
    Node *parent = nullptr;

    while (current != nullptr)
    {
        parent = current;
        if (key < current->key)
        {
            current = current->left;
        }
        else if (key > current->key)
        {
            current = current->right;
        }
        else
        { // Если ключ уже существует, освобождаем выделенную память и возвращаем исходный корень
            delete newNode;
            return root;
        }
    }

    if (key < parent->key)
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }

    return root;
}

Node *BinarySearchTree::removeNode(Node *root, int key)
{
    Node *current = root;
    Node *parent = nullptr;

    // Поиск узла для удаления
    while (current != nullptr && current->key != key)
    {
        parent = current;
        if (key < current->key)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    // Если узел для удаления не найден
    if (current == nullptr)
    {
        return root;
    }

    // У узла нет либо одного, либо обоих дочерних узлов
    if (current->left == nullptr)
    {
        if (parent == nullptr)
        { // Удаляемый узел - корень дерева
            root = current->right;
        }
        else if (current == parent->left)
        {
            parent->left = current->right;
        }
        else
        {
            parent->right = current->right;
        }
        delete current;
    }
    else if (current->right == nullptr)
    {
        if (parent == nullptr)
        { // Удаляемый узел - корень дерева
            root = current->left;
        }
        else if (current == parent->left)
        {
            parent->left = current->left;
        }
        else
        {
            parent->right = current->left;
        }
        delete current;
    }
    else
    { // У узла есть два дочерних узла
        // Находим наименьший узел в правом поддереве (минимальный узел в правом поддереве)
        Node *successor = current->right;
        while (successor->left != nullptr)
        {
            parent = successor;
            successor = successor->left;
        }

        // Копируем данные наименьшего узла в текущий узел
        current->key = successor->key;

        // Удаляем наименьший узел в правом поддереве
        if (parent != nullptr)
        {
            parent->left = removeNode(parent->left, successor->key);
        }
        else
        {
            current->right = removeNode(current->right, successor->key);
        }
    }

    return root;
}

Node *BinarySearchTree::minNode(Node *node)
{
    // Находим самый левый узел, чтобы найти минимальный узел в дереве
    Node *current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

Node *BinarySearchTree::findNode(Node *node, int key)
{
    while (node != nullptr)
    {
        if (node->key == key)
        {
            return node;
        }
        if (key < node->key)
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }
    return nullptr; // Возвращаем nullptr, если узел не найден
}

int BinarySearchTree::min()
{
    if (root == nullptr)
    {
        std::cout << "Пустое дерево" << std::endl;
        return -1;
    }

    // Ищем самый левый узел, это будет минимальное значение в дереве
    Node *current = root;
    while (current->left != nullptr)
    {
        current = current->left;
    }

    return current->key;
}

int BinarySearchTree::max()
{
    if (root == nullptr)
    {
        std::cout << "Пустое дерево" << std::endl;
        return -1;
    }

    // Ищем самый правый узел, это будет максимальное значение в дереве
    Node *current = root;
    while (current->right != nullptr)
    {
        current = current->right;
    }

    return current->key;
}
