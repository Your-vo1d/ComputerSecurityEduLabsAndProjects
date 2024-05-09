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
BinaryTree::Node *BinaryTree::createBinaryTree(int n)
{
    if (n == 0)
        return nullptr;
    Node *q = new Node(rand() % 100);
    int n1;
    n1 = n / 2;
    q->left = createBinaryTree(n1);
    q->right = createBinaryTree(n - 1 - n1);
    return q;
}

// Конструктор копирования
BinaryTree::BinaryTree(const BinaryTree &T)
{
    if (T.root == nullptr)
        root = nullptr;
    else
    {
        root = new Node(T.root->key);
        root->left = copyTree(T.root->left);
        root->right = copyTree(T.root->right);
    }
}

//Вспомогательный метод для копирования
BinaryTree::Node *BinaryTree::copyTree(Node *q)
{
    if (q == nullptr)
        return nullptr;
    Node *p = new Node(q->key);
    p->left = copyTree(q->left);
    p->right = copyTree(q->right);
    return p;
}

//Деструктор
BinaryTree::~BinaryTree()
{
    deleteTree(root);
}

//Вспомогательный метод для деструктора
void BinaryTree::deleteTree(Node *q)
{
    if (q == nullptr)
        return;
    deleteTree(q->left);
    deleteTree(q->right);
    delete q;
}

//Метод для получения корня
BinaryTree::Node *BinaryTree::getRoot()
{
    return root;
}

//Метод для вывода дерева
void BinaryTree::printTree()
{
    if (root == nullptr)
    {
        std::cout << "Дерево пустое";
        return;
    }
    printTree(root, 0);
}

//Вспомогательный метод вывода дерева
void BinaryTree::printTree(Node *q, int k)
{
    if (q == nullptr)
        return;
    printTree(q->right, k + 3);
    for (int i = 0; i < k; i++)
    {
        std::cout << " ";
    }
    std::cout.width(2);
    std::cout << q->key << std::endl;
    printTree(q->left, k + 3);
}

//Метод обхода Л-К-П
int BinaryTree::LNR(int *a)
{
    int k = 0;
    LNR(root, a, k);
    return k;
}

//Вспомогательный метод для обхода Л-К-П
void BinaryTree::LNR(Node *q, int *a, int &k)
{
    if (q == nullptr)
        return;
    LNR(q->left, a, k);
    a[k++] = q->key;
    LNR(q->right, a, k);
}

//Метод для поиска минимального
int BinaryTree::min()
{
    if (root == nullptr)
    {
        std::cout << "Дерево пустое";
        return -1;
    }
    return min(root);
}

int BinaryTree::min(Node *q)
{
    int m, m1;
    m = q->key;
    if (q->left)
    {
        m1 = min(q->left);
        if (m1 < m)
            m = m1;
    }
    if (q->right)
    {
        m1 = min(q->right);
        if (m1 < m)
            m = m1;
    }
    return m;
}

void BinaryTree::leaves()
{
    if (root == nullptr)
    {
        std::cout << "Дерево пустое";
        return;
    }
    leaves(root);
}

void BinaryTree::leaves(Node *q)
{
    std::list<Node *> unprocessedNodes(1, root);
    while (!unprocessedNodes.empty())
    {
        Node *node = unprocessedNodes.front();
        std::cout << node->key << ' ';
        if (node->left)
        {
            unprocessedNodes.push_back(node->left);
        }
        if (node->right)
        {
            unprocessedNodes.push_back(node->right);
        }
        unprocessedNodes.pop_front();
    }
}
