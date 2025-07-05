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
x^2*cos(x)+sin(x)/x
3*x-4*(x+1)/(x)
*/


int main() {
    string expr = "6*x^3+2*x";
    vector<string> tokens = tokenize(expr);
    
    unique_ptr<node> tree = construct_syntree(tokens.begin(), tokens.end());
    
    unique_ptr<node> d1 = tree->deriv();
    d1->print("sin");
    cout << '\n';
}