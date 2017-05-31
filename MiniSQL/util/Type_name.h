//
// Created by baislsl on 17-5-29.
//

#ifndef MINISQL_TYPE_INFO_H
#define MINISQL_TYPE_INFO_H

#include <ios>
#include "../Interpreter/Error.h"

enum class Type_name{
    INT, CHAR, FLOAT
};
// value type, no data and name
class Type_info{
public:

    Type_info(std::string type_name, size_t size = 32){
        if(type_name == "int"){
            _type_name = Type_name::INT;
            _size = 4;
        }else if(type_name == "float"){
            _type_name = Type_name::FLOAT;
            _size = 8;
        }else if(type_name == "char"){
            _type_name = Type_name::CHAR;
            _size = size;
        }else
            throw Parse_error("No type as " + type_name);
    }

    Type_info(Type_name _type_name, size_t size = 32):_type_name(_type_name){
        switch (_type_name){
            case Type_name::INT     : _size = 4; break;
            case Type_name::FLOAT   : _size = 8; break;
            case Type_name::CHAR    :
                if(size <= 0 || size >= 256){
                    throw "\"char\" type size can not be " + size;
                }
                _size = size;
                break;
        }
    }

    inline Type_name type_name() const{
        return _type_name;
    }

    inline size_t size() const{
        return _size;
    }

    inline std::string get_type_name() const{
        switch (_type_name){
            case Type_name::INT    : return "int";
            case Type_name::FLOAT  : return "float";
            case Type_name::CHAR   : return "char";
        }
    }

protected:
    Type_name _type_name;
    size_t _size;
};

#endif //MINISQL_TYPE_INFO_H
