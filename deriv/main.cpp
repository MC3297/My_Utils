#include <vector>
#include <string>
#include <iostream>

#include "token.cpp"
#include "nodes.cpp"
#include "create.cpp"
#include "deriv.cpp"
#include "syntree.cpp"

using std::string;
using std::vector;
using std::cin;
using std::cout;

//notes
#include <bits/stdc++.h>
/*
g++ -Wall -D_GLIBCXX_DEBUG main.cpp && ./a.out
no spaces
variables must be in x
6x^2 bad, 6*x^2 good
*/


int main() {
    string expr = "sin(x^5)";
    
    vector<string> tokens = tokenize(expr);
    
    unique_ptr<node> tree = construct_syntree(tokens.begin(), tokens.end());
    
    unique_ptr<node> d1 = tree->deriv();
    d1->print("sin");
    cout << '\n';
}