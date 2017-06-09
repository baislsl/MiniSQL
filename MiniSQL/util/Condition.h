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

    Condition(const std::string &name, const std::string &cmp_value, const Relation relation);

    ~Condition();

    void build(const Type_info &type_info);

    bool match(const Type_value &value);

    Condition opposite() const {
        Condition result(_name, _value, opposite_relation(_relation));
        if(is_build){
            result.build(*_type_value);
        }
        return result;
    }

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
    bool is_build = false;

    Relation opposite_relation(const Relation &relation) const ;


};

#endif //MINISQL_CONDITION_H
