#include "Node.h"

Node::Node(int k, Node* l, Node* r, int b) {
    key = k;
    left = l;
    right = r;
    balance = b;
}

Node::~Node() {
    left = nullptr;
    right = nullptr;
}

int Node::getBalance() const {
    return balance;
}

void Node::calculateBalance() {
    int leftHeight = (left == nullptr) ? 0 : getHeight(left);
    int rightHeight = (right == nullptr) ? 0 : getHeight(right);
    balance = rightHeight - leftHeight;
}

int Node::getHeight(Node* node) const {
    if (node == nullptr)
        return 0;
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    return 1 + std::max(leftHeight, rightHeight);
}
