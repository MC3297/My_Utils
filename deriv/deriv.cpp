#pragma once

#include "nodes.cpp"
#include "create.cpp"

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
            create_node("^", right->clone(), create_node("2"))
        );
    }
    if (op == "^") {
        if (right->type == node_type::NUMBER) {
            return create_node("*", 
                right->clone(),
                create_node("*",
                    create_node("^", left->clone(), create_node("-", right->clone(), create_node("1"))),
                    left->deriv()
                )
            );
        }
        return create_node("*",
            create_node("^", left->clone(), right->clone()),
            create_node("+", 
                create_node("*", right->deriv(), create_node("log", left->clone())),
                create_node("*", right->clone(), create_node("/", left->deriv(), left->clone()))
            )
        );
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
        return create_node("/", arg->deriv(), arg->clone());
    }
    return nullptr;
}