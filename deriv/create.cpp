#pragma once

#include "nodes.cpp"

#include <cmath>
#include <stdexcept>

using std::pow;
using std::invalid_argument;

int get_node_val(const unique_ptr<node>& v) {
    if (v->type != node_type::NUMBER) {
        throw invalid_argument("get_node_val: node isnt number_node");
    }
    return static_cast<number_node*>(v.get())->value;
}

/*
Returns a ptr to node with specifications via parameters
`l` and `r` parameters have default arguments instead of overloading create_node()
edit: might just overload
Call `create_node(op, l, r)` for operation nodes
Call `create_node(func, arg)` for function nodes
Call `create_node(tok)` for any other valid type
Returns `nullptr` if token doesn't fit any category
*/

unique_ptr<node> create_node(const string& tok) {
    
    if (is_integer_token(tok)) {
        return make_unique<number_node>(stoi(tok));
    }
    if (is_variable_token(tok)) {
        return make_unique<variable_node>(tok);
    }
    return nullptr;
}

unique_ptr<node> create_node(const string& tok, unique_ptr<node> arg) {
    
    if (is_func_token(tok)) {
        return make_unique<func_node>(tok, move(arg));
    }
    return nullptr;
}

unique_ptr<node> create_node(const string& tok, unique_ptr<node> l, unique_ptr<node> r) {
    
    //combining constants
    if (l->type == node_type::NUMBER && r->type == node_type::NUMBER) {
        int x = get_node_val(l);
        int y = get_node_val(r);
        
        if (tok == "+") return make_unique<number_node>(x+y);
            
        //kinda iffy cuz neg nums arent supported
        if (tok == "-") return make_unique<number_node>(x-y);
        
        if (tok == "*") return make_unique<number_node>(x*y);
        
        if (tok == "/" && x%y == 0) return make_unique<number_node>(x/y);
        
        if (tok == "^") {//should maybe check if int overflow and throw error
            return make_unique<number_node>(pow(x,y));
        }
    }
    
    //some identities
    if (tok == "+") {
        if (l->type == node_type::NUMBER && get_node_val(l) == 0) {
            return move(r);
        }
        if (r->type == node_type::NUMBER && get_node_val(r) == 0) {
            return move(l);
        }
    }
    
    if (tok == "-") {
        if (r->type == node_type::NUMBER && get_node_val(r) == 0) {
            return move(l);
        }
    }
    
    if (tok == "*") {
        if ((l->type == node_type::NUMBER && get_node_val(l) == 0) ||
        (r->type == node_type::NUMBER && get_node_val(r) == 0)) {
            return make_unique<number_node>(0);
        }
        
        if (l->type == node_type::NUMBER && get_node_val(l) == 1) {
            return move(r);
        }
        if (r->type == node_type::NUMBER && get_node_val(r) == 1) {
            return move(l);
        }
    }
    
    if (tok == "/") {
        if (r->type == node_type::NUMBER && get_node_val(r) == 0) {
            throw invalid_argument("can't divide by 0");
        }
        
        if (l->type == node_type::NUMBER && get_node_val(l) == 0) {
            return make_unique<number_node>(0);
        }
        if (r->type == node_type::NUMBER && get_node_val(r) == 1) {
            return move(l);
        }
    }
    
    if (tok == "^") {
        //what is 0^0 equal to?
        if ((l->type == node_type::NUMBER && get_node_val(l) == 0) ||
        (r->type == node_type::NUMBER && get_node_val(r) == 0)) {
            throw invalid_argument("create_node: 0^0 undefined");
        }
        
        if (l->type == node_type::NUMBER && get_node_val(l) == 0) {
            return make_unique<number_node>(0);
        }
        if (l->type == node_type::NUMBER && get_node_val(l) == 1) {
            return make_unique<number_node>(1);
        }
        if (r->type == node_type::NUMBER && get_node_val(r) == 0) {
            return make_unique<number_node>(1);
        }
        if (r->type == node_type::NUMBER && get_node_val(r) == 1) {
            return move(l);
        }
    }
    
    
    if (is_operator_token(tok)) {
        return make_unique<op_node>(tok, move(l), move(r));
    }
    return nullptr;
}