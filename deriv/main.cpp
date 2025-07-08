#include "token.h"
#include "nodes.h"
#include "create.h"
#include "syntree.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>


int main(int argc, char *argv[]) {
    if (argc == 1) {
        throw std::invalid_argument("requires expression");
    }
    std::string expr = argv[1];
    
    std::vector<std::string> tokens = tokenize(expr);
    
    std::unique_ptr<node> tree = construct_syntree(tokens.begin(), tokens.end());
    
    std::unique_ptr<node> d1 = tree->deriv();
    d1->print("sin");//just so result doesnt have a parenthesis
    std::cout << '\n';
}