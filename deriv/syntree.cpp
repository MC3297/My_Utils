#pragma once

#include <string>
#include <memory>

using std::string;
// using std::shared_ptr;
using std::unique_ptr;
using std::move;

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
    virtual ~node() = default;
};

/*
`node` subclass
Stores a constant integer
Integers for now, could become a double if extended
*/
struct number_node : public node {
    int value;

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

    variable_node(const string& varName):
        node(node_type::VARIABLE),
        name(varName) {}
};

/*
`node` subclass
Represents binary operators like *, +, etc
Requires a left and right node
*/
struct binary_op_node : public node {
    string op;
    unique_ptr<node> left;
    unique_ptr<node> right;

    binary_op_node(const string& _op, unique_ptr<node> l, unique_ptr<node> r):
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
struct unary_op_node : public node {
    string op;
    unique_ptr<node> arg;

    unary_op_node(const string& _op, unique_ptr<node> c):
        node(node_type::UNARY_OP),
        op(_op),
        arg(move(c)) {}
};


