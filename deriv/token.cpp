#pragma once

#include <vector>
#include <string>
#include <algorithm>

using std::string;
using std::vector;
using std::find;

/*
List of valid tokens
Tentative, maybe include e
*/
const vector<string> valid_tokens = {
    //Operations (increasing precedence)
    "+", "-", "*", "/", "^",

    //Functions
    "sin", "cos", "log",
    
    //Constant Integers
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    
    //Variables
    "x",
    
    //Misc
    "(", ")"
};

/*
Returns a list of tokens from `expr`
Finds longest valid token and pushes
Prioritizes longer tokens, ie "cosh" before "cos"
*/
vector<string> tokenize(const string& expr) {
    vector<string> res;
    
    string cur;
    int i = 0;
    while (i < expr.size()) {
        while (find(valid_tokens.begin(), valid_tokens.end(), cur) == valid_tokens.end()) {
            cur += expr[i];
            i++;
        }
        while (find(valid_tokens.begin(), valid_tokens.end(), cur+expr[i]) != valid_tokens.end()) {
            cur += expr[i];
            i++;
        }
        
        res.push_back(cur);
        cur.clear();
    }
    
    return res;
}