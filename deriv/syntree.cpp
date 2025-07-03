#pragma once

#include "token.cpp"

#include <vector>
#include <string>
#include <memory>
#include <stack>
#include <map>

using std::vector;
using std::string;
using std::stoi;
// using std::shared_ptr;
using std::next;
using std::unique_ptr;
using std::make_unique;
using std::move;
using std::stack;
using std::map;

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
Specifies the type of node
*/
enum class node_type {
    NUMBER,
    VARIABLE,
    BINARY_OP,
    UNARY_OP
};

/*
General `node` struct of syntree
*/
struct node {
    node_type type;

    node(node_type t) : type(t) {}
    
    virtual void print() = 0;
    virtual ~node() = default;
};

/*
`node` subclass
Stores a constant integer
Integers for now, could become a double if extended
*/
struct number_node : public node {
    int value;
    
    void print() override {
        std::cout << "(num: " << value << ")\n";
    }
    
    number_node(int val):
        node(node_type::NUMBER),
        value(val) {}
};

/*
`node` subclass
Stores a variable name
"x" for now, can include other names if extended to multivariable expressions
*/
struct variable_node : public node {
    string name;
    
    void print() override {
        std::cout << "(var: " << name << ")\n";
    }

    variable_node(const string& varName):
        node(node_type::VARIABLE),
        name(varName) {}
};

/*
`node` subclass
Represents binary operators like *, +, etc
Requires a left and right node
*/
struct op_node : public node {
    string op;
    unique_ptr<node> left;
    unique_ptr<node> right;
    
    void print() override {
        std::cout << "(op: " << op << ")\n";
        std::cout << "\nleft: "; left->print();
        std::cout << "\nright: "; right->print();
    }

    op_node(const string& _op, unique_ptr<node> l, unique_ptr<node> r):
        node(node_type::BINARY_OP),
        op(_op),
        left(move(l)),
        right(move(r)) {}
};

/*
`node` subclass
Represents unary operators or single variable functions like sin, log, etc
Requires an argument node
*/
struct func_node : public node {
    string FUNC;
    unique_ptr<node> arg;
    
    void print() override {
        std::cout << "(func: " << FUNC << ")\n";
        std::cout << "(arg: "; arg->print();
    }

    func_node(const string& _FUNC, unique_ptr<node> c):
        node(node_type::UNARY_OP),
        FUNC(_FUNC),
        arg(move(c)) {}
};

unique_ptr<node> create_node(const string& tok) {
    if (is_integer_token(tok)) {
        return make_unique<number_node>(stoi(tok));
    }
    if (is_operator_token(tok)) {
        return make_unique<op_node>(tok, nullptr, nullptr);
    }
    if (is_variable_token(tok)) {
        return make_unique<variable_node>(tok);
    }
    return nullptr;
}

unique_ptr<node> recurse_syntree(vector<string>::const_iterator st, vector<string>::const_iterator ed, 
const map<vector<string>::const_iterator, vector<string>::const_iterator>& match) {
    
    using c_it = vector<string>::const_iterator;
    
    stack<string> ops;
    stack<unique_ptr<node>> terms;
    
    for (c_it it = st; it != ed; ++it) {
        
        if (*it == "(") {
            // std::cout << "PARENTHESIS\n";
            // std::cout << *(it+1) << ' ' << *(match.at(it)) << '\n';
            terms.push(recurse_syntree(it+1, match.at(it), match));
            //std::cout << "recurse: "; terms.top()->print();
            it = match.at(it);
        }
        else if (is_operator_token(*it)) {
            while (!ops.empty() && precedence_of(ops.top()) >= precedence_of(*it)) {
                
                unique_ptr<op_node> tmp = make_unique<op_node>(ops.top(), nullptr, nullptr);
                ops.pop();
                tmp->right = move(terms.top());
                terms.pop();
                tmp->left = move(terms.top());
                terms.pop();
                
                terms.push(move(tmp));
            }
            ops.push(*it);
        }
        else if (is_func_token(*it)) {
            unique_ptr<func_node> tmp = make_unique<func_node>(*it, nullptr);
            tmp->arg = recurse_syntree(it+2, match.at(it+1), match);
            it = match.at(it+1);
            terms.push(move(tmp));
        }
        else {
            terms.push(create_node(*it));
        }
    }
    
    while (!ops.empty()) {
        
        unique_ptr<op_node> tmp = make_unique<op_node>(ops.top(), nullptr, nullptr);
        ops.pop();
        tmp->right = move(terms.top());
        terms.pop();
        tmp->left = move(terms.top());
        terms.pop();
        
        terms.push(move(tmp));
    }
    
    if (terms.size() != 1) {
        std::cout << "somesing wong" << terms.size() << '\n';
    }

    unique_ptr<node> res = move(terms.top());
    terms.pop();
    return res;
}

unique_ptr<node> construct_syntree(const vector<string>::const_iterator st, const vector<string>::const_iterator ed) {
    using c_it = vector<string>::const_iterator;
    
    //first create map that matches "(" iterators to ")" iterators
    map<c_it, c_it> match;
    
    //stack to store "("
    stack<c_it> keys;
    for (c_it it = st; it != ed; ++it) {
        if (*it == "(") {
            keys.push(it);
        }
        else if (*it == ")") {
            match[keys.top()] = it;
            keys.pop();
        }
    }
    
    return recurse_syntree(st, ed, match);
}
