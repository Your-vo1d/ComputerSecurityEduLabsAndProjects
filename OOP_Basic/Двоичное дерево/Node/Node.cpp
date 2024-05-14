#include "Node.h"

Node::Node(int k, Node* l, Node* r, int b) {
    key = k;
    left = l;
    right = r;
    balance = b;
}

Node::~Node() {}

int Node::getBalance() {
    return balance;
}