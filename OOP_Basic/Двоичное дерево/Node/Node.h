#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <cstdlib>

class Node {
public:
    int key;
    int balance;
    Node* left;
    Node* right;

    Node(int k = 0, Node* l = nullptr, Node* r = nullptr, int b = 0, int h = 0);
    ~Node();
    int getBalance();
private:
    int calculateHeight(Node* node) const;
};

#endif // NODE_H
