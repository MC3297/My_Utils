#pragma once

#include "token.h"

#include <iostream>
#include <string>
#include <memory>

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
    
    virtual std::unique_ptr<node> deriv() = 0;
    virtual std::unique_ptr<node> clone() = 0;
    virtual void print(const std::string& prev_op, std::string& res) = 0;
    virtual bool equal(const std::unique_ptr<node>& t) = 0;
    virtual ~node() = default;
};

/*
`node` subclass
Stores a constant integer
Integers only
*/
struct number_node : public node {
    int value;
    
    number_node(int val):
        node(node_type::NUMBER),
        value(val) {}
    
    std::unique_ptr<node> deriv() override;
    
    std::unique_ptr<node> clone() override {
        return std::make_unique<number_node>(value);
    }
    
    void print(const std::string& prev_op, std::string& res) override {
        res += std::to_string(value);
    }
    
    bool equal(const std::unique_ptr<node>& t) override {
        return t->type == node_type::NUMBER && static_cast<number_node*>(t.get())->value == value;
    }
};

/*
`node` subclass
Stores a variable name
"x" for now, can include other names if extended to multivariable expressions
*/
struct variable_node : public node {
    std::string name;
    
    variable_node(const std::string var):
        node(node_type::VARIABLE),
        name(var) {}
    
    std::unique_ptr<node> deriv() override;
    
    std::unique_ptr<node> clone() override {
        return std::make_unique<variable_node>(name);
    }
    
    void print(const std::string& prev_op, std::string& res) override {
        res += name;
    }
    
    bool equal(const std::unique_ptr<node>& t) override {
        return t->type == node_type::VARIABLE && static_cast<variable_node*>(t.get())->name == name;
    }
};

/*
`node` subclass
Represents binary operators like *, +, etc
Requires a left and right node
*/
struct op_node : public node {
    std::string op;
    std::unique_ptr<node> left;
    std::unique_ptr<node> right;
    
    op_node(const std::string _op, std::unique_ptr<node>&& l, std::unique_ptr<node>&& r):
        node(node_type::BINARY_OP),
        op(_op),
        left(std::move(l)),
        right(std::move(r)) {}
    
    std::unique_ptr<node> deriv() override;
    
    std::unique_ptr<node> clone() override {
        return std::make_unique<op_node>(op, left->clone(), right->clone());
    }
    
    void print(const std::string& prev_op, std::string& res) override {
        bool no_parenthesis = is_func_token(prev_op) ||
        (is_operator_token(prev_op) && precedence_of(prev_op) <= precedence_of(op));
        
        if (!no_parenthesis) res += "(";
        left->print(op, res);
        res += op;
        right->print(op, res);
        if (!no_parenthesis) res += ")";
    }

    bool equal(const std::unique_ptr<node>& t) override {
        return t->type == node_type::BINARY_OP && 
            static_cast<op_node*>(t.get())->op == op &&
            equal(left) &&
            equal(right);
    }
};

/*
`node` subclass
Represents unary operators or single variable functions like sin, log, etc
Requires an argument node
*/
struct func_node : public node {
    std::string func;
    std::unique_ptr<node> arg;
    
    func_node(const std::string _func, std::unique_ptr<node>&& c):
        node(node_type::UNARY_OP),
        func(_func),
        arg(std::move(c)) {}
    
    std::unique_ptr<node> deriv() override;
    
    std::unique_ptr<node> clone() override {
        return std::make_unique<func_node>(func, arg->clone());
    }
    
    void print(const std::string& prev_op, std::string& res) override {
        res += func+"(";
        arg->print(func, res);
        res += ")";
    }

    bool equal(const std::unique_ptr<node>& t) override {
        return t->type == node_type::UNARY_OP && 
            static_cast<func_node*>(t.get())->func == func &&
            equal(arg);
    }
};