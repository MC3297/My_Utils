#pragma once

#include "token.cpp"

#include <iostream>
#include <string>
#include <memory>

using std::cout;
using std::string;
using std::stoi;
using std::unique_ptr;
using std::make_unique;
using std::move;

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
    
    virtual unique_ptr<node> deriv() = 0;
    virtual unique_ptr<node> clone() = 0;
    virtual void print(const string& prev_op) = 0;
    virtual ~node() = default;
};

/*
`node` subclass
Stores a constant integer
Integers for now, could become a double in the future
*/
struct number_node : public node {
    int value;
    
    number_node(int val):
        node(node_type::NUMBER),
        value(val) {}
    
    unique_ptr<node> deriv() override;
    
    unique_ptr<node> clone() override {
        return make_unique<number_node>(value);
    }
    
    void print(const string& prev_op) override {
        cout << value;
    }
    
};

/*
`node` subclass
Stores a variable name
"x" for now, can include other names if extended to multivariable expressions
*/
struct variable_node : public node {
    string name;
    
    variable_node(const string var):
        node(node_type::VARIABLE),
        name(var) {}
    
    unique_ptr<node> deriv() override;
    
    unique_ptr<node> clone() override {
        return make_unique<variable_node>(name);
    }
    
    void print(const string& prev_op) override {
        cout << name;
    }

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
    
    op_node(const string _op, unique_ptr<node> l, unique_ptr<node> r):
        node(node_type::BINARY_OP),
        op(_op),
        left(move(l)),
        right(move(r)) {}
    
    unique_ptr<node> deriv() override;
    
    unique_ptr<node> clone() override {
        return make_unique<op_node>(op, left->clone(), right->clone());
    }
    
    void print(const string& prev_op) override {
        bool no_parenthesis = is_func_token(prev_op) ||
        (is_operator_token(prev_op) && precedence_of(prev_op) <= precedence_of(op));
        
        if (!no_parenthesis) cout << "(";
        left->print(op);
        cout << op;
        right->print(op);
        if (!no_parenthesis) cout << ")";
    }

};

/*
`node` subclass
Represents unary operators or single variable functions like sin, log, etc
Requires an argument node
*/
struct func_node : public node {
    string func;
    unique_ptr<node> arg;
    
    func_node(const string _func, unique_ptr<node> c):
        node(node_type::UNARY_OP),
        func(_func),
        arg(move(c)) {}
    
    unique_ptr<node> deriv() override;
    
    unique_ptr<node> clone() override {
        return make_unique<func_node>(func, arg->clone());
    }
    
    void print(const string& prev_op) override {
        cout << func << "(";
        arg->print(func);
        cout << ")";
    }

};