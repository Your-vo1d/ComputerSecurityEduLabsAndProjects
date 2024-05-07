#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

class Node {
    int key;
    Node *left, *right;

public:
    Node(int k = 0, Node *l = nullptr, Node *r = nullptr) {
        key = k;
        left = l;
        right = r;
    }
    friend class BinaryTree;
    friend class SearchTree;
};

class BinaryTree {
    Node *root;

public:
    BinaryTree() {
        root = nullptr;
    }

    BinaryTree(int n);
    Node *createBinaryTree(int n);
    BinaryTree(const BinaryTree &);
    Node *copyTree(Node *);
    Node *getRoot() {
        return root;
    }
    void printTree();
    void printTree(Node *, int);
    int LNR(int *);
    void LNR(Node *, int *, int &);
    int min();
    int min(Node *);
    void leaves();
    void leaves(Node *);
};

BinaryTree::BinaryTree(int n) {
    root = createBinaryTree(n);
}

Node *BinaryTree::createBinaryTree(int n) {
    if (n == 0)
        return nullptr;
    Node *q = new Node(rand() % 100);
    int n1;
    n1 = n / 2;
    q->left = createBinaryTree(n1);
    q->right = createBinaryTree(n - 1 - n1);
    return q;
}

BinaryTree::BinaryTree(const BinaryTree &T) {
    if (T.root == nullptr)
        root = nullptr;
    else {
        root = new Node(T.root->key);
        root->left = copyTree(T.root->left);
        root->right = copyTree(T.root->right);
    }
}

Node *BinaryTree::copyTree(Node *q) {
    if (q == nullptr)
        return nullptr;
    Node *p = new Node(q->key);
    p->left = copyTree(q->left);
    p->right = copyTree(q->right);
    return p;
}

void BinaryTree::printTree() {
    if (root == nullptr) {
        cout << "Дерево пустое";
        return;
    }
    printTree(root, 0);
}

void BinaryTree::printTree(Node *q, int k) {
    if (q == nullptr)
        return;
    printTree(q->right, k + 3);
    for (int i = 0; i < k; i++) {
        cout << " ";
    }
    cout.width(2);
    cout << q->key << endl;
    printTree(q->left, k + 3);
}

int BinaryTree::LNR(int *a) {
    int k = 0;
    LNR(root, a, k);
    return k;
}

void BinaryTree::LNR(Node *q, int *a, int &k) {
    if (q == nullptr)
        return;
    LNR(q->left, a, k);
    a[k++] = q->key;
    LNR(q->right, a, k);
}

int BinaryTree::min() {
    if (root == nullptr) {
        cout << "Дерево пустое";
        return -1;
    }
    return min(root);
}

int BinaryTree::min(Node *q) {
    int m, m1;
    m = q->key;
    if (q->left) {
        m1 = min(q->left);
        if (m1 < m)
            m = m1;
    }
    if (q->right) {
        m1 = min(q->right);
        if (m1 < m)
            m = m1;
    }
    return m;
}

void BinaryTree::leaves() {
    if (root == nullptr) {
        cout << "Дерево пустое";
        return;
    }
    leaves(root);
}

void BinaryTree::leaves(Node *q) {
    if (q == nullptr)
        return;
    if (!(q->left) && !(q->right)) {
        cout << q->key << " ";
        return;
    }
    leaves(q->left);
    leaves(q->right);
}

int main() {
    srand(time(0));
    BinaryTree T(10), T1(T), T2;
    T.printTree();
    cout << "Минимальное значение в дереве: " << T.min() << endl;
    cout << "Листья дерева: ";
    T.leaves();
    cout << endl;
    int a[10];
    int size = T.LNR(a);
    cout << "Результат ЛКП обхода: ";
    for (int i = 0; i < size; i++) {
        cout << a[i] << " ";
    }
    cout << endl;
    T2.printTree();
    cout << endl;
    return 0;
}
