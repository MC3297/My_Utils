#include "token.h"

#include <algorithm>
#include <cstddef>
#include <cctype>
#include <stdexcept>


static bool is_number(const std::string& tok) {
    if (tok.empty()) return false;
    for (char c: tok) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

bool is_operator_token(const std::string& tok) {
    return std::find(operator_tokens.begin(), operator_tokens.end(), tok) != operator_tokens.end();
}

bool is_integer_token(const std::string& tok) {
    return is_number(tok);
}

bool is_variable_token(const std::string& tok) {
    return std::find(variable_tokens.begin(), variable_tokens.end(), tok) != variable_tokens.end();
}

bool is_func_token(const std::string& tok) {
    return std::find(function_tokens.begin(), function_tokens.end(), tok) != function_tokens.end();
}

bool is_misc_token(const std::string& tok) {
    return std::find(misc_tokens.begin(), misc_tokens.end(), tok) != misc_tokens.end();
}


//doesnt include negative numbers as valid tokens
bool is_valid_token(const std::string& tok) {
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
int precedence_of(const std::string& op) {
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
std::vector<std::string> tokenize(const std::string& expr) {
    std::vector<std::string> res;
    
    std::string cur;
    std::size_t i = 0;
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
            throw std::invalid_argument("tokenize: invalid expr");
        }
        
        res.push_back(cur);
        cur.clear();
    }
    
    return res;
}