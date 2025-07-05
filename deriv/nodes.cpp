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
    
    number_node(int val):
        node(node_type::NUMBER),
        value(val) {}
    
    unique_ptr<node> deriv() override;
    
    unique_ptr<node> clone() override {
        return make_unique<number_node>(value);
    }
    
    void print() override {
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
    
    void print() override {
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
    
    void print() override {
        left->print();
        cout << op;
        right->print();
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
    
    void print() override {
        cout << func << "(";
        arg->print();
        cout << ")";
    }

};

/*
Returns a ptr to node with specifications via parameters
`l` and `r` parameters have default arguments instead of overloading create_node()
edit: might just overload
Call `create_node(op, l, r)` for operation nodes
Call `create_node(func, arg)` for function nodes
Call `create_node(tok)` for any other valid type
Returns `nullptr` if token doesn't fit any category
*/
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

unique_ptr<node> number_node::deriv() {
    return create_node("0");
}
unique_ptr<node> variable_node::deriv() {
    return create_node("1");
}
unique_ptr<node> op_node::deriv() {
    if (op == "+") {
        return create_node("+", left->deriv(), right->deriv());
    }
    if (op == "-") {
        return create_node("-", left->deriv(), right->deriv());
    }
    if (op == "*") {
        return create_node("+", 
        create_node("*", left->deriv(), right->clone()),
        create_node("*", left->clone(), right->deriv()));
    }
    if (op == "/") {
        return create_node("/",
            create_node("-",
                create_node("*", left->deriv(), right->clone()),
                create_node("*", left->clone(), right->deriv())
            ),
            create_node("*", right->clone(), right->clone())
        );
    }
    if (op == "^") {
        if (right->type == node_type::NUMBER) {
            int pow = static_cast<number_node*>(right.get())->value;
            return create_node("*",
            create_node(std::to_string(pow)),
            create_node("^", left->clone(), create_node(std::to_string(pow-1))));
        }
    }
    return nullptr;
}
unique_ptr<node> func_node::deriv() {
    if (func == "sin") {
        return create_node("*", create_node("cos", arg->clone()), arg->deriv());
    }
    if (func == "cos") {
        return create_node("-",
            create_node("0"),
            create_node("*",
                create_node("sin", arg->clone()),
                arg->deriv()
            )
        );
    }
    if (func == "log") {
        return create_node("/", arg->clone(), arg->deriv());
    }
    return nullptr;
}