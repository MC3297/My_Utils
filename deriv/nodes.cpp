#pragma once

#include "token.cpp"

#include <string>
#include <memory>

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
    
    virtual void print() = 0;
    virtual ~node() = default;
};

/*
`node` subclass
Stores a constant integer
Integers for now, could become a double in the future
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

    variable_node(const string& var):
        node(node_type::VARIABLE),
        name(var) {}
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
    string func;
    unique_ptr<node> arg;
    
    void print() override {
        std::cout << "(func: " << func << ")\n";
        std::cout << "(arg: "; arg->print();
    }

    func_node(const string& _func, unique_ptr<node> c):
        node(node_type::UNARY_OP),
        func(_func),
        arg(move(c)) {}
};

unique_ptr<node> create_node(const string& tok, unique_ptr<node> l = nullptr, unique_ptr<node> r = nullptr) {
    if (is_operator_token(tok)) {
        
        //optimizations and simplifying
        if (l && r && l->type == node_type::NUMBER && r->type == node_type::NUMBER) {
            int x = static_cast<number_node*>(l.get())->value;
            int y = static_cast<number_node*>(r.get())->value;
            if (tok == "+") return make_unique<number_node>(x+y);
                
            //kinda iffy cuz neg nums arent supported
            if (tok == "-") return make_unique<number_node>(x-y);
            
            if (tok == "*") return make_unique<number_node>(x*y);
            
            if (tok == "/" && x%y == 0) return make_unique<number_node>(x/y);
        }
        
        return make_unique<op_node>(tok, move(l), move(r));
    }
    if (is_func_token(tok)) {
        return make_unique<func_node>(tok, move(l));
    }
    if (is_integer_token(tok)) {
        return make_unique<number_node>(stoi(tok));
    }
    if (is_variable_token(tok)) {
        return make_unique<variable_node>(tok);
    }
    return nullptr;
}