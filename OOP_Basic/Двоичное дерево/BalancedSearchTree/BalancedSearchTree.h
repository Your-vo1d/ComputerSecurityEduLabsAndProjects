#ifndef BALANCEDSEARCHTREE_H
#define BALANCEDSEARCHTREE_H

#include "../BinarySearchTree/BinarySearchTree.h"

class BalancedSearchTree : public BinarySearchTree
{
public:
    BalancedSearchTree() : BinarySearchTree() {}
    BalancedSearchTree(int n);
    BalancedSearchTree(const std::vector<int> &vec);

    // Переопределение метода добавления узла
    virtual Node *addNode(Node *root, int key) override;

private:
    // Вспомогательные методы для балансировки дерева
    Node *balance(Node *root, bool &isFixed);
    Node *rotateLeft(Node *node);
    Node *rotateRight(Node *node);
    Node *rotateRightLeft(Node *node);
    Node *rotateLeftRight(Node *node);
    int getBalance(Node *node);
};

#endif
