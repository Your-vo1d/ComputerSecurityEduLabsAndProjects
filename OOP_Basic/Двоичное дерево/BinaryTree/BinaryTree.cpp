#include "BinaryTree.h"

// Конструктор по умолчаниб
BinaryTree::BinaryTree()
{
    root = nullptr;
}

// Конструктор по кол-ву элементов
BinaryTree::BinaryTree(int n)
{
    root = createBinaryTree(n);
}

// Вспомогательный метод создания дерева
BinaryTree::Node *BinaryTree::createBinaryTree(int numNodes)
{
    if (numNodes == 0)
        return nullptr;
    Node *newNode = new Node(rand() % 100);
    int leftSubtreeSize = numNodes / 2;
    newNode->left = createBinaryTree(leftSubtreeSize);
    newNode->right = createBinaryTree(numNodes - 1 - leftSubtreeSize);
    return newNode;
}

// Конструктор копирования
BinaryTree::BinaryTree(const BinaryTree &other)
{
    if (other.root == nullptr)
        root = nullptr;
    else
    {
        root = new Node(other.root->key);
        root->left = copyTree(other.root->left);
        root->right = copyTree(other.root->right);
    }
}

// Вспомогательный метод для копирования
BinaryTree::Node *BinaryTree::copyTree(Node *sourceNode)
{
    if (sourceNode == nullptr)
        return nullptr;
    Node *copiedNode = new Node(sourceNode->key);
    copiedNode->left = copyTree(sourceNode->left);
    copiedNode->right = copyTree(sourceNode->right);
    return copiedNode;
}

// Деструктор
BinaryTree::~BinaryTree()
{
    deleteTree(root);
}

// Вспомогательный метод для деструктора
void BinaryTree::deleteTree(Node *sourceNode)
{
    if (sourceNode == nullptr)
        return;
    deleteTree(sourceNode->left);
    deleteTree(sourceNode->right);
    delete sourceNode;
}

// Метод для получения корня
BinaryTree::Node *BinaryTree::getRoot()
{
    return root;
}

// Метод для вывода дерева
void BinaryTree::printTree()
{
    if (root == nullptr)
    {
        std::cout << "Дерево пустое" << std::endl;
        return;
    }
    printTree(root, 0);
    std::cout << std::endl;
    std::cout << std::endl;
}

// Вспомогательный метод вывода дерева
void BinaryTree::printTree(Node *currentNode, int indentation)
{
    if (currentNode == nullptr)
        return;
    printTree(currentNode->right, indentation + 3);
    for (int i = 0; i < indentation; i++)
    {
        std::cout << " ";
    }
    std::cout.width(2);
    std::cout << currentNode->key << std::endl;
    printTree(currentNode->left, indentation + 3);
}

// Метод обхода Л-К-П
int BinaryTree::LNR(int *a)
{
    int count = 0;
    LNR(root, a, count);
    return count;
}

// Вспомогательный метод для обхода Л-К-П
void BinaryTree::LNR(Node *currentNode, int *array, int &index)
{
    if (currentNode == nullptr)
        return;
    LNR(currentNode->left, array, index);
    array[index++] = currentNode->key;
    LNR(currentNode->right, array, index);
}

// Метод для поиска минимального
int BinaryTree::min()
{
    if (root == nullptr)
    {
        std::cout << "Дерево пустое";
        return -1;
    }
    return min(root);
}

// Вспомогательный метод для поиска минимального ключа
int BinaryTree::min(Node *currentNode)
{
    int currentMin, leftMin, rightMin;
    currentMin = currentNode->key;
    if (currentNode->left)
    {
        leftMin = min(currentNode->left);
        if (leftMin < currentMin)
            currentMin = leftMin;
    }
    if (currentNode->right)
    {
        rightMin = min(currentNode->right);
        if (rightMin < currentMin)
            currentMin = rightMin;
    }
    return currentMin;
}

// Метод для поиска минимального
int BinaryTree::max()
{
    if (root == nullptr)
    {
        std::cout << "Дерево пустое";
        return -1;
    }
    return max(root);
}

// Метод поиска максимального
int BinaryTree::max(Node *currentNode)
{
    int currentMax, leftMax, rightMax;
    currentMax = currentNode->key;
    if (currentNode->left)
    {
        leftMax = max(currentNode->left);
        if (leftMax > currentMax)
            currentMax = leftMax;
    }
    if (currentNode->right)
    {
        rightMax = max(currentNode->right);
        if (rightMax > currentMax)
            currentMax = rightMax;
    }
    return currentMax;
}

// Метод обхода дерева по уровням
void BinaryTree::leaves()
{
    if (root == nullptr)
    {
        std::cout << "Дерево пустое";
        return;
    }
    leaves(root);
    std::cout << std::endl;
}

// Вспомогательный метод обхода дерева по уровням
void BinaryTree::leaves(Node *q) {
    std::vector<Node *> unprocessedNodes(1, root);
    while (!unprocessedNodes.empty()) {
        Node *node = unprocessedNodes.back();
        unprocessedNodes.pop_back();
        std::cout << node->key << ' ';
        if (node->left) {
            unprocessedNodes.push_back(node->left);
        }
        if (node->right) {
            unprocessedNodes.push_back(node->right);
        }
    }
}