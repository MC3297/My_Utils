#include "token.cpp"
#include "nodes.cpp"
#include "create.cpp"
#include "deriv.cpp"
#include "syntree.cpp"

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::invalid_argument;


int main(int argc, char *argv[]) {
    if (argc == 1) {
        throw invalid_argument("requires expression");
    }
    string expr = argv[1];
    
    vector<string> tokens = tokenize(expr);
    
    unique_ptr<node> tree = construct_syntree(tokens.begin(), tokens.end());
    
    unique_ptr<node> d1 = tree->deriv();
    d1->print("sin");//just so result doesnt have a parenthesis
    cout << '\n';
}