#include <vector>
#include <string>
#include <iostream>

#include "token.cpp"
#include "nodes.cpp"
#include "syntree.cpp"

using std::string;
using std::vector;
using std::cin;
using std::cout;

//notes
#include <bits/stdc++.h>
/*
g++ -Wall -D_GLIBCXX_DEBUG deriv.cpp && ./a.out
no spaces
x^2*cos(x)+sin(x)/x
3*x-4*(x+1)/(x)
*/


int main() {
    string expr = "2*3*x+sin(x^5)*cos(3)-x";//"(x+2)*x+3/(x^5)";//"1+(12*(x-4)/x)";
    vector<string> tokens = tokenize(expr);
    cout << tokens << '\n';
    
    unique_ptr<node> tree = construct_syntree(tokens.begin(), tokens.end());
    tree->print();
}