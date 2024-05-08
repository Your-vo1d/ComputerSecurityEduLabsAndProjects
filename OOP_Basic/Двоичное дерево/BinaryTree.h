#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <list>

class BinaryTree
{
protected:
    class Node
    {
    public:
        int key;
        Node *left, *right;

        Node(int k = 0, Node *l = nullptr, Node *r = nullptr)
        {
            key = k;
            left = l;
            right = r;
        }

        ~Node()
        {
            left = nullptr;
            right = nullptr;
        }
    };

    Node *root;

public:
    BinaryTree();
    BinaryTree(int n);
    BinaryTree(const BinaryTree &);
    ~BinaryTree(); 
    Node *getRoot();
    void printTree();
    int LNR(int *);
    int min();
    int max();
    void leaves();

private:
    Node *createBinaryTree(int n);
    Node *copyTree(Node *);
    void deleteTree(Node *);
    void printTree(Node *, int);
    void LNR(Node *, int *, int &);
    int min(Node *);
    void leaves(Node *);
};

#endif // BINARYTREE_H
