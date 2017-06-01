//
// Created by baislsl on 17-6-2.
//

#include "Condition.h"


bool Condition::match(const Type_value &value) {
    switch (_relation){
        case BIGGER:
            return value > *_type_value;
        case SMALLER:
            return value < *_type_value;
        case EQUAL:
            return value == *_type_value;
        case BIGGER_EQUAL:
            return value >= *_type_value;
        case SMALLER_EQUAL:
            return value <= *_type_value;
        case NOT_EQUAL:
            return value != *_type_value;
    }
}

void Condition::build(const Type_info &type_info) {
    _type_value = std::shared_ptr<Type_value>(new Type_value(type_info, _value));
}

Condition::~Condition() {
}

Condition::Condition(const std::string &name, const std::string &relation, const std::string &cmp_value)
        : _name(name), _value(cmp_value) {
    if (relation == ">") {
        _relation = Relation::BIGGER;
    } else if (relation == ">=") {
        _relation = Relation::BIGGER_EQUAL;
    } else if (relation == "<") {
        _relation = Relation::SMALLER;
    } else if (relation == "<=") {
        _relation = Relation::SMALLER_EQUAL;
    } else if (relation == "=") {
        _relation = Relation::EQUAL;
    } else if (relation == "<>") {
        _relation = Relation::NOT_EQUAL;
    } else {
        throw Parse_error("No operation as " + relation);
    }
}