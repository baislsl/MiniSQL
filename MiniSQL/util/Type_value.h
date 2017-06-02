//
// Created by baislsl on 17-5-29.
//

#ifndef MINISQL_VALUE_TYPE_H
#define MINISQL_VALUE_TYPE_H

#include "Type_info.h"
#include <cstring>
#include <iostream>
#include <cmath>
#include <values.h>
// with value
class Type_value : public Type_info{

public:

    Type_value();

    Type_value(const Type_info &type_info, const char* data);

    Type_value(const Type_info &type_info, const std::string &value_str);

    Type_value(const std::string &_type_name, const std::string &value_str)
        :Type_info(_type_name){
        parse_value(value_str);
    }

    inline Type_info type_info() const {
        return Type_info(_type_name, _size);
    }

    inline char* data() const {
        return (char*)(&value);
    }

    friend bool operator<(const Type_value &a, const Type_value &b);

    friend bool operator==(const Type_value &a, const Type_value &b);

    friend bool operator>(const Type_value &a, const Type_value &b);
    friend bool operator<=(const Type_value &a, const Type_value &b);
    friend bool operator>=(const Type_value &a, const Type_value &b);
    friend bool operator!=(const Type_value &a, const Type_value &b);

    friend std::ostream &operator<<(std::ostream &out, const Type_value &type_value);

private:
    union {
        int integer;
        double ff;
        char cc[256];
    }value;

    void parse_value(const std::string &value_str);
};

#endif //MINISQL_VALUE_TYPE_H
