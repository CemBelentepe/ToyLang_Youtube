#pragma once
#include <string>

#include "Value.hpp"

class Variable
{
public:
    std::string name;
    Value value;

    Variable(const std::string& name);
};