#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <cstdlib>

class Node {
public:
    int key;
    int height;
    Node* left;
    Node* right;

    Node(int k = 0, Node* l = nullptr, Node* r = nullptr, int b = 1);
    ~Node();
};

#endif // NODE_H
