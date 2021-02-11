#pragma once
#include <string>
#include <iostream>
#include <variant>

enum class TypeTag
{
    ERR,
    BOOL,
    NUMBER,
    STRING
};

class Value
{
public:
    TypeTag tag;
    std::variant<void*, bool, double, std::string> data;

    Value()
        : tag(TypeTag::ERR), data(nullptr)
    { }

    Value(bool val)
        : tag(TypeTag::BOOL), data(val)
    { }

    Value(double val)
    : tag(TypeTag::NUMBER), data(val)
    { }

    Value(std::string val)
    : tag(TypeTag::STRING), data(val)
    { }

    void print() const
    {
        std::cout << *this;
    }
	friend std::ostream& operator<<(std::ostream& os, const Value& val)
    {
        switch (val.tag)
        {
        case TypeTag::BOOL:
            os << (std::get<bool>(val.data) ? "true" : "false");
            break;
        case TypeTag::NUMBER:
            os << std::get<double>(val.data);
            break;
        case TypeTag::STRING:
            os << std::get<std::string>(val.data);
            break;
        default:
            os << "ERROR";
        }
        return os;
    }
};