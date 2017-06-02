//
// Created by baislsl on 17-6-2.
//

#include "Type_info.h"

Type_info::Type_info(std::string type_name, size_t size) {
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

Type_info::Type_info(Type_name _type_name, size_t size) :_type_name(_type_name){
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

std::string Type_info::get_type_name() const {
    switch (_type_name){
        case Type_name::INT    : return "int";
        case Type_name::FLOAT  : return "float";
        case Type_name::CHAR   : return "char";
    }
}
