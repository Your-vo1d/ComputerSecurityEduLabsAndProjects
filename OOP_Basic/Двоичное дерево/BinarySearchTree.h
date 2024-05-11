#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "BinaryTree.h"
#include <vector>

class BinarySearchTree : public BinaryTree
{
public:
    BinarySearchTree() : BinaryTree() {}
    BinarySearchTree(int n);
    virtual int min();
    virtual int max();
    virtual BinaryTree::Node *addNode(Node* root, int key);
    virtual Node *removeNode(Node* root, int key);
    Node *findNode(Node* root, int key);
    Node *minNode(Node* root);
};

#endif // BINARYSEARCHTREE_H
