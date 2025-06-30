#include <vector>
#include <string>
#include <iostream>

#include "token.cpp"
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
    string expr = "12*x-4/x";
    vector<string> tokens = tokenize(expr);
    cout << tokens << '\n';
}