#pragma once

#include "nodes.cpp"

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