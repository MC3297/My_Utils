#pragma once

#include "token.h"
#include "nodes.h"
#include "create.h"

#include <memory>
#include <vector>
#include <string>

std::unique_ptr<node> construct_syntree(const std::vector<std::string>::const_iterator st, const std::vector<std::string>::const_iterator ed);