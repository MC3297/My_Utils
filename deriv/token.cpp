#pragma once

#include <vector>
#include <string>
#include <algorithm>

using std::string;
using std::vector;
using std::find;

const vector<string> operator_tokens = {"+", "-", "*", "/", "^"};
const vector<string> function_tokens = {"sin", "cos", "log"};
const vector<string> integer_tokens = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

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

bool is_valid_token(const string& tok) {
    return find(valid_tokens.begin(), valid_tokens.end(), tok) != valid_tokens.end();
}

bool is_operator_token(const string& tok) {
    return find(operator_tokens.begin(), operator_tokens.end(), tok) != operator_tokens.end();
}

bool is_integer_token(const string& tok) {
    return find(integer_tokens.begin(), integer_tokens.end(), tok) != integer_tokens.end();
}

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
        while (!is_valid_token(cur)) {
            cur += expr[i];
            i++;
        }
        while (is_valid_token(cur+expr[i])) {
            cur += expr[i];
            i++;
        }
        
        res.push_back(cur);
        cur.clear();
    }
    
    return res;
}