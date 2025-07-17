#pragma once

#include "token.h"
#include "nodes.h"
#include "create.h"

#include <memory>
#include <vector>
#include <string>

/*
Returns the root node ptr to the syntree
Parses the token list from `st` to `ed`
Includes `st`, excludes `ed` (as usual)
*/
std::unique_ptr<node> construct_syntree(const std::vector<std::string>::const_iterator st, const std::vector<std::string>::const_iterator ed);