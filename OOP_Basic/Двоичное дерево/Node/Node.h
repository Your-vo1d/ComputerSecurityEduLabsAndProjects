#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <cstdlib>
class Node {
public:
    int key;
    int balance;
    Node* left, * right;

    Node(int k = 0, Node* l = nullptr, Node* r = nullptr, int b = 0);
    ~Node();
    int getBalance() const;
    void calculateBalance();

private:
    int getHeight(Node* node) const;
};

#endif // NODE_H
