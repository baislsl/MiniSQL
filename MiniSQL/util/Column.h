//
// Created by baislsl on 17-5-29.
//

#ifndef MINISQL_COLUMN_H
#define MINISQL_COLUMN_H


#include "Type_info.h"
#include <iostream>

// no value, but definition
class Column : public Type_info{
public:
    std::string name;
    int attr;

    Column(const std::string &name, const Type_info &value_info, const int attr = 0);

    inline bool find_attr(int _attr) const {
        return (attr & _attr) != 0;
    }

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
        UNIQUE = 1 << 0,
        PRIMARY = 1 << 1,
        INDEX = 1 << 2
    };

};


#endif //MINISQL_COLUMN_H
