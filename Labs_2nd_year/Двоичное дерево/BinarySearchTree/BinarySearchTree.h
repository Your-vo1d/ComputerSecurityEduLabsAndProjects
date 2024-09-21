#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "../BinaryTree/BinaryTree.h"

class BinarySearchTree : public BinaryTree
{
public:
    BinarySearchTree() : BinaryTree() {}
    BinarySearchTree(int n);
    BinarySearchTree(const std::vector<int> &vec);
    virtual int min();
    virtual int max();
    virtual Node *addNode(Node *root, int key);
    virtual Node *removeNode(Node *root, int key);
    Node *findNode(Node *root, int key);
    Node *minNode(Node *root);
};

#endif // BINARYSEARCHTREE_H
