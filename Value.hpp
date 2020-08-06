#pragma once
#include <string>
#include <iostream>

enum class TypeTag
{
    BOOL,
    NUMBER,
    STRING
};

class Value
{
public:
    TypeTag tag;
    union Data
    {
        bool valBool;
        double valNumber;
        char* valString;
    } data;

    Value(bool val)
        : tag(TypeTag::BOOL)
    {
        data.valBool = val;
    }

    Value(double val)
    : tag(TypeTag::NUMBER)
    {
        data.valNumber = val;
    }

    Value(std::string val)
    : tag(TypeTag::STRING)
    {
        data.valString = new char[val.size() + 1];
        val.copy(data.valString, val.size(), 0);
        data.valString[val.size()] = 0;
    }

    void print() const
    {
        switch(tag)
        {
        case TypeTag::BOOL:
            std::cout << data.valBool;
            break;
        case TypeTag::NUMBER:
            std::cout << data.valNumber;
            break;
        case TypeTag::STRING:
            std::cout << data.valString;
            break;
        }
    }
};