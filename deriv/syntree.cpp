#pragma once

#include "token.cpp"
#include "nodes.cpp"

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stack>
#include <map>
#include <stdexcept>

using std::cout;
using std::vector;
using std::string;
using std::unique_ptr;
using std::make_unique;
using std::move;
using std::stack;
using std::map;
using std::invalid_argument;

using c_it = vector<string>::const_iterator;

/*
Helper func for construct_syntree
Returns ptr to root node of syntax tree
Uses a modified "shunting yard algorithm" to produce syntax tree
`st` and `ed` are passed by value for slight efficiency
*/
unique_ptr<node> recurse_create_syntree(const c_it st, const c_it ed, const map<c_it, c_it>& match) {
    
    stack<string> ops;
    stack<unique_ptr<node>> terms;
    
    for (c_it it = st; it != ed; ++it) {
        
        if (*it == "(") {
            
            //Condense everything between the () into a subtree
            terms.push(recurse_create_syntree(it+1, match.at(it), match));
            it = match.at(it);
        }
        else if (is_operator_token(*it)) {
            
            //Create subtrees from the operators in `ops` stack
            while (!ops.empty() && precedence_of(ops.top()) >= precedence_of(*it)) {
                
                if (terms.size() < 2) {
                    throw "recurse_create_syntree: too many ops, not enough terms";
                }
                
                unique_ptr<node> r = move(terms.top());
                terms.pop();
                unique_ptr<node> l = move(terms.top());
                terms.pop();
                
                terms.push(create_node(ops.top(), move(l), move(r)));
                ops.pop();
            }
            ops.push(*it);
        }
        else if (is_func_token(*it)) {
            
            //Similar to regular parenthesis but puts a func node above root of subtree
            terms.push(create_node(*it, recurse_create_syntree(it+2, match.at(it+1), match)));
            it = match.at(it+1);
        }
        else {
            
            //numbers, variables
            terms.push(create_node(*it));
        }
    }
    
    //Process remaining operators
    while (!ops.empty()) {
        
        if (terms.size() < 2) {
            throw "recurse_create_syntree: too many ops, not enough terms";
        }
        
        unique_ptr<node> r = move(terms.top());
        terms.pop();
        unique_ptr<node> l = move(terms.top());
        terms.pop();
        
        terms.push(create_node(ops.top(), move(l), move(r)));
        ops.pop();
    }
    
    if (terms.size() != 1) {
        throw invalid_argument("recurse_create_syntree: parsing error");
    }

    unique_ptr<node> res = move(terms.top());
    terms.pop();
    return res;
}

/*
Returns ptr to root node of syntax tree
Creates map<c_it, c_it> to match ( to )
Then calls recurse_create_syntree
`st` and `ed` are passed by value for slight efficiency
*/
unique_ptr<node> construct_syntree(const c_it st, const c_it ed) {
    
    //first create map that matches "(" iterators to ")" iterators
    map<c_it, c_it> match;
    
    //stack to store "("
    stack<c_it> keys;
    for (c_it it = st; it != ed; ++it) {
        if (*it == "(") {
            keys.push(it);
        }
        else if (*it == ")") {
            if (keys.empty()) {
                throw invalid_argument("construct_syntree: missing opening parenthesis");
            }
            match[keys.top()] = it;
            keys.pop();
        }
    }
    if (!keys.empty()) {
        throw invalid_argument("construct_syntree: too many opening parenthesis");
    }
    
    return recurse_create_syntree(st, ed, match);
}
