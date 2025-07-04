#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <cstddef>
#include <cctype>
#include <stdexcept>

using std::string;
using std::vector;
using std::find;
using std::size_t;
using std::all_of;
using std::isdigit;
using std::invalid_argument;

const vector<string> operator_tokens = {"+", "-", "*", "/", "^"};
const vector<string> function_tokens = {"sin", "cos", "log"};
const vector<string> digit_tokens = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
const vector<string> variable_tokens = {"x"};
const vector<string> misc_tokens = {"(", ")"};

static bool is_number(const string& tok) {
    if (tok.empty()) return false;
    for (char c: tok) {
        if (!isdigit(c)) return false;
    }
    return true;
}


bool is_operator_token(const string& tok) {
    return find(operator_tokens.begin(), operator_tokens.end(), tok) != operator_tokens.end();
}

bool is_integer_token(const string& tok) {
    return is_number(tok);
}

bool is_variable_token(const string& tok) {
    return find(variable_tokens.begin(), variable_tokens.end(), tok) != variable_tokens.end();
}

bool is_func_token(const string& tok) {
    return find(function_tokens.begin(), function_tokens.end(), tok) != function_tokens.end();
}

bool is_misc_token(const string& tok) {
    return find(misc_tokens.begin(), misc_tokens.end(), tok) != misc_tokens.end();
}


//doesnt include negative numbers as valid tokens
bool is_valid_token(const string& tok) {
    return is_operator_token(tok) || 
    is_integer_token(tok) || 
    is_variable_token(tok) || 
    is_func_token(tok) ||
    is_misc_token(tok);
}

/*
Assigns each binary operator a numerical precedence
Higher precendence means larger number
*/
int precedence_of(const string &op) {
    if (op == "^") return 3;
    if (op == "*") return 2;
    if (op == "/") return 2;
    if (op == "+") return 1;
    if (op == "-") return 1;
    return 0;
}

/*
Returns a list of tokens from `expr`
Finds longest valid token and pushes
Prioritizes longer tokens, ie "cosh" before "cos"
*/
vector<string> tokenize(const string& expr) {
    vector<string> res;
    
    string cur;
    size_t i = 0;
    while (i < expr.size()) {
        while (i < expr.size() && !is_valid_token(cur)) {
            cur += expr[i];
            i++;
        }
        while (i < expr.size() && is_valid_token(cur+expr[i])) {
            cur += expr[i];
            i++;
        }
        if (cur.empty()) {
            throw invalid_argument("tokenize: invalid expr");
        }
        
        res.push_back(cur);
        cur.clear();
    }
    
    return res;
}