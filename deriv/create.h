#pragma once

#include "token.h"
#include "nodes.h"

#include <memory>
#include <string>

int get_node_val(const std::unique_ptr<node>& v);

std::unique_ptr<node> create_node(const std::string& tok);

std::unique_ptr<node> create_node(const std::string& tok, std::unique_ptr<node> arg);

std::unique_ptr<node> create_node(const std::string& tok, std::unique_ptr<node> l, std::unique_ptr<node> r);

