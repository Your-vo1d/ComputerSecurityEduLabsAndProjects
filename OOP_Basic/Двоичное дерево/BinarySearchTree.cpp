#include "BinarySearchTree.h"

BinarySearchTree::BinarySearchTree(int n)
{
    root = nullptr; // Инициализируем корень как nullptr

    std::vector<int> keys;
    std::cout << std::endl;
    for (int i = 0; i < n; ++i)
    {
        keys.push_back(rand() % 100); // Генерируем случайные ключи от 0 до 99
    }

    for (int i = 0; i < n; i++)
    {
        root = addNode(root, keys[i]); // Вставляем ключи в дерево поиска
    }
}

BinaryTree::Node *BinarySearchTree::addNode(Node *root, int key)
{
    // Если дерево пустое, создаем новый узел
    if (root == nullptr)
    {
        return new Node(key);
    }

    // Если ключ меньше ключа текущего узла, рекурсивно добавляем его в левое поддерево
    if (key < root->key)
    {
        root->left = addNode(root->left, key);
    }
    // Иначе, рекурсивно добавляем ключ в правое поддерево
    else if (key > root->key)
    {
        root->right = addNode(root->right, key);
    }

    // Возвращаем корень поддерева
    return root;
}

BinaryTree::Node *BinarySearchTree::removeNode(Node *root, int key)
{
    // Если дерево пустое, возвращаем nullptr
    if (root == nullptr)
    {
        return root;
    }

    // Ищем узел для удаления
    if (key < root->key)
    {
        root->left = removeNode(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = removeNode(root->right, key);
    }
    else
    { // Найден узел для удаления
        // У узла нет либо одного, либо обоих дочерних узлов
        if (root->left == nullptr)
        {
            Node *temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr)
        {
            Node *temp = root->left;
            delete root;
            return temp;
        }

        // У узла есть два дочерних узла
        // Находим наименьший узел в правом поддереве (минимальный узел в правом поддереве)
        Node *temp = minNode(root->right);

        // Копируем данные наименьшего узла в текущий узел
        root->key = temp->key;

        // Удаляем наименьший узел в правом поддереве
        root->right = removeNode(root->right, temp->key);
    }
    return root;
}

BinaryTree::Node *BinarySearchTree::minNode(Node *node)
{
    // Находим самый левый узел, чтобы найти минимальный узел в дереве
    Node *current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

BinaryTree::Node * BinarySearchTree::findNode(Node *node, int key) {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->key == key) {
        return node;
    }
    if (key < node->key) {
        if (node->left != nullptr) {
            return findNode(node->left, key);
        } else {
            return nullptr;
        }
    } else {
        if (node->right != nullptr) {
            return findNode(node->right, key);
        } else {
            return nullptr;
        }
    }
}


int BinarySearchTree::min()
{
    if (root == nullptr)
    {
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
