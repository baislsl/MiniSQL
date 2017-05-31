//
// Created by baislsl on 17-5-29.
//

#ifndef MINISQL_VALUE_TYPE_H
#define MINISQL_VALUE_TYPE_H

#include "Type_name.h"
#include <cstring>
#include <iostream>
// with value
class Type_value : public Type_info{

public:

    Type_value(const Type_info &type_info, const char* data)
            :Type_info(type_info.type_name(), type_info.size()){
        switch (_type_name){
            case Type_name::INT     : value.integer = (int)(*data); break;
            case Type_name::FLOAT   : value.ff = (double)(*data); break;
            case Type_name::CHAR    : strncpy(value.cc, data, _size); break;
        }
    }

    Type_value(const Type_info &type_info, const std::string &value_str)
        :Type_info(type_info.type_name(), type_info.size()){
        parse_value(value_str);
    }

    Type_value(const std::string &_type_name, const std::string &value_str)
        :Type_info(_type_name){
        parse_value(value_str);
    }

    inline char* data() const {
        return (char*)(&value);
    }

    friend std::ostream &operator<<(std::ostream &out, const Type_value &type_value) {
        switch (type_value._type_name){
            case Type_name ::INT    : out << type_value.value.integer; break;
            case Type_name ::FLOAT  : out << type_value.value.ff; break;
            case Type_name ::CHAR   : out << std::string(type_value.value.cc); break;
        }
        return out;

    }

private:
    union {
        int integer;
        double ff;
        char cc[256];
    }value;

    void parse_value(const std::string &value_str){
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
                std::string str = fix(value_str);
                strcpy(a, str.c_str());
                if(str.size() > _size){
                    throw Parse_error("too long char values for " + str);
                }
                sscanf(a, "%s",  value.cc);
                break;

        }
    }

    /**
     * rid the string of quotation marks
     * */
    std::string fix(const std::string& str){
        size_t begin = 0, end = str.size() - 1;
        while(begin <= end && str[begin] <= ' ') begin++;
        while(begin <= end && str[end] <= ' ') end--;
        if(begin < end && str[begin] == str[end] && (str[begin] == '\'' || str[begin] == '\"'))
            return str.substr(begin + 1, end - 1);
        else return str.substr(begin, end + 1);
    }
};

#endif //MINISQL_VALUE_TYPE_H