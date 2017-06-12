//
// Created by baislsl on 17-6-2.
//
#include <cfloat>
#include "Type_value.h"
#include "Util_exception.h"
#include "../Interpreter/Interpreter_exception.h"

Type_value::~Type_value() {}

Type_value::Type_value() {}

Type_value::Type_value(const Type_value &type_value)
        :Type_info(type_value.type_name(), type_value.size()) {
    if(_type_name == Type_name::CHAR){
        memcpy(value.cc, type_value.value.cc, _size + 1);
    }else{
        value = type_value.value;
    }
}

Type_value &Type_value::operator=(const Type_value &type_value) {
    this->_type_name = type_value._type_name;
    this->_size = type_value._size;
    if(_type_name == Type_name::CHAR){
        memcpy(value.cc, type_value.value.cc, _size + 1);
    }else{
        value = type_value.value;
    }
}

Type_value::Type_value(const Type_info &type_info, const char *data)
        :Type_info(type_info.type_name(), type_info.size()){
    switch (_type_name){
        case Type_name::INT     :
            memcpy(&value, data, 4);
            break;
            // value.integer = (int)(*data); break;
        case Type_name::FLOAT   :
            memcpy(&value, data, 8);
            break;
            // value.ff = (double)(*data); break;
        case Type_name::CHAR    :
            strncpy(value.cc, data, _size);
            value.cc[_size] = 0;
            break;
    }
}

Type_value::Type_value(const Type_info &type_info, const std::string &value_str)
        :Type_info(type_info.type_name(), type_info.size()){
    parse_value(value_str);
}

bool operator<(const Type_value &a, const Type_value &b) {
    if(a._type_name != b._type_name)
        throw Data_compare_error("Can no compare data with different type!");
    switch (a._type_name){
        case Type_name::INT:
            return a.value.integer < b.value.integer;
        case Type_name::FLOAT:
            return a.value.ff < b.value.ff;
        case Type_name::CHAR:
            return strcmp(a.value.cc, b.value.cc) < 0;
    }
}

bool operator==(const Type_value &a, const Type_value &b) {
    if(a._type_name != b._type_name)
        throw Data_compare_error("Can no compare data with different type!");
    switch (a._type_name){
        case Type_name::INT:
            return a.value.integer == b.value.integer;
        case Type_name::FLOAT:
            return fabs(a.value.ff - b.value.ff) <= DBL_MIN;
        case Type_name::CHAR:
            return strcmp(a.value.cc, b.value.cc) == 0;
    }
}

bool operator>(const Type_value &a, const Type_value &b) {
    return b < a;
}

bool operator<=(const Type_value &a, const Type_value &b) {
    return !(b < a);
}

bool operator>=(const Type_value &a, const Type_value &b) {
    return !(a < b);
}

bool operator!=(const Type_value &a, const Type_value &b) {
    return !(a == b);
}

std::ostream &operator<<(std::ostream &out, const Type_value &type_value) {
    switch (type_value._type_name){
        case Type_name ::INT    : out << type_value.value.integer; break;
        case Type_name ::FLOAT  : out << type_value.value.ff; break;
        case Type_name ::CHAR   :
            out << std::string(type_value.value.cc); break;
    }
    return out;

}

void Type_value::parse_value(const std::string &value_str) {
    char a[300];
    switch (_type_name){
        case Type_name::INT :
            strcpy(a, value_str.c_str());
            sscanf( a, "%d", &value.integer);
            break;
        case Type_name::FLOAT :
            strcpy(a, value_str.c_str());
            sscanf(a, "%lf",  &value.ff);
            break;
        case Type_name::CHAR :
            strcpy(a, value_str.c_str());
            if(value_str.size() > _size){
                throw Parse_error("too long char values for " + value_str);
            }
            sscanf(a, "%s",  value.cc);
            break;

    }
}

size_t Type_value::print_length() const {
    if(_type_name == Type_name::CHAR){
        size_t ans = 0;
        while(value.cc[ans]) ans++;
        return ans;
    }else{
        return 10;
    }
}


