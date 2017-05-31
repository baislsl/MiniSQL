//
// Created by baislsl on 17-5-29.
//

#include "Type_name.h"
#include <iostream>

#ifndef MINISQL_COLUMN_H
#define MINISQL_COLUMN_H
// no value, but definition
class Column : public Type_info{
public:
    std::string name;
    int attr;

    Column(const std::string &name, const Type_info &value_info, const int attr = 0)
            : name(name), Type_info(value_info), attr(attr) {}

    inline void add_attribute(int _attr) {
        attr |= _attr;
    }

    inline void remove_attribute(int _attr) {
        attr &= ~_attr;
    }

    inline size_t type_size() const {
        return size();
    }

    inline Type_info value_type() const {
        return Type_info(_type_name, type_size());
    }

    enum {
        UNQUE = 1 << 0,
        PRIMARY = 1 << 1,
        INDEX = 1 << 2
    };

};


#endif //MINISQL_COLUMN_H
