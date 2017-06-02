//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_CONDITION_H
#define MINISQL_CONDITION_H

#include "Type_value.h"
#include "Type_info.h"
#include <string>
#include <memory>

enum Relation {
    BIGGER,
    SMALLER,
    EQUAL,
    BIGGER_EQUAL,
    SMALLER_EQUAL,
    NOT_EQUAL
};

class Condition {
public:
    Condition(const std::string &name, const std::string &cmp_value, const std::string &relation);

    ~Condition();

    void build(const Type_info &type_info);

    bool match(const Type_value &value);

    inline std::string name() const {
        return _name;
    }

    inline size_t size() const {
        return _type_value->size();
    }

    inline Type_info type_info() const{
        return _type_value->type_info();
    }

private:
    Relation _relation;
    std::string _name;
    std::shared_ptr<Type_value> _type_value;
    std::string _value;

};

#endif //MINISQL_CONDITION_H
