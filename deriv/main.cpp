#include "token.h"
#include "nodes.h"
#include "create.h"
#include "syntree.h"

#include <vector>
#include <string>
#include <cstring>
#include <cstdio>

const char* differentiate(const char* input) {
    std::string expr(input);
    
    std::vector<std::string> tokens = tokenize(expr);
    
    std::unique_ptr<node> tree = construct_syntree(tokens.begin(), tokens.end());
    
    std::unique_ptr<node> d1 = tree->deriv();
    
    std::string derivative;
    d1->print("sin", derivative);//just so result doesnt have a parenthesis
    
    char* output = new char[derivative.length() + 1];
    std::strcpy(output, derivative.c_str());
    return output;
}

int main(int argc, char *argv[]) {
    std::printf("%s\n", differentiate(argv[1]));
}