#pragma once

#include "token.h"
#include "nodes.h"

#include <memory>
#include <string>

/*
Returns the integer value of a number_node
Will throw an error if `v` isn't a number_node
*/
int get_node_val(const std::unique_ptr<node>& v);


/*
Returns a node (rvalue) categorized by `tok`
Meant for numbers, variables
Returns `nullptr` for anything else
*/
std::unique_ptr<node> create_node(const std::string& tok);

/*
Returns a func node (rvalue)
Returns `nullptr` for anything else
*/
std::unique_ptr<node> create_node(const std::string& tok, std::unique_ptr<node>&& arg);

/*
Returns an op_node (rvalue)
Returns `nullptr` for anything else
Note: contains various heuristic simplification rules
Eg handling various operator identities like +0 or *1
*/
std::unique_ptr<node> create_node(const std::string& tok, std::unique_ptr<node>&& l, std::unique_ptr<node>&& r);
