#include <iostream>
#include <cstdlib>
#include <ctime>
#include "BinaryTree.h"

int main() {
    srand(time(0));
    BinaryTree T(10), T1(T), T2;
    T.printTree();
    std::cout << T.min() << std::endl;
    T.leaves();
    std::cout << std::endl;
    int a[10];
    T.LNR(a);
    for(int i = 0; i < 10; i++) {
        std::cout << a[i] << " ";
    }
    std::cout <<"sada"<< std::endl;
    std::cout << std::endl;

    return 0;
}
