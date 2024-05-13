#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include "../Node/Node.h"

class BinaryTree {
public:
    Node* root;
    BinaryTree();
    BinaryTree(int n);
    BinaryTree(const BinaryTree&);
    ~BinaryTree();
    BinaryTree operator=(const BinaryTree& other);
    Node* getRoot();
    void printTree();
    int NLR(int*);
    virtual int min();
    virtual int max();
    void leaves();

private:
    void copyNodes(Node* src, Node*& dest);
    Node* createBinaryTree(int n);
    Node* copyTree(Node*);
    void deleteTree(Node*);
    void printTree(Node*, int);
    void NLR(Node*, int*, int&);
    int min(Node*);
    int max(Node*);
    void leaves(Node*);
};

#endif // BINARYTREE_H
