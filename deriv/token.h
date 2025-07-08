#pragma once

#include <vector>
#include <string>

const std::vector<std::string> operator_tokens = {"+", "-", "*", "/", "^"};
const std::vector<std::string> function_tokens = {"sin", "cos", "log"};
const std::vector<std::string> digit_tokens = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
const std::vector<std::string> variable_tokens = {"x"};
const std::vector<std::string> misc_tokens = {"(", ")"};

bool is_operator_token(const std::string& tok);

bool is_integer_token(const std::string& tok);

bool is_variable_token(const std::string& tok);
    
bool is_func_token(const std::string& tok);
        
bool is_func_token(const std::string& tok);

bool is_valid_token(const std::string& tok);

int precedence_of(const std::string& op);

std::vector<std::string> tokenize(const std::string& expr);