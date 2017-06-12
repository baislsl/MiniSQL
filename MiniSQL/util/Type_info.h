//
// Created by baislsl on 17-5-29.
//

#ifndef MINISQL_TYPE_INFO_H
#define MINISQL_TYPE_INFO_H

#include <string>
#include <cstddef>

enum class Type_name{
    INT, CHAR, FLOAT
};
// value type, no data and name
class Type_info{
public:

    Type_info();

    Type_info(std::string type_name, size_t size = 32);

    Type_info(Type_name _type_name, size_t size = 32);

    inline Type_name type_name() const{
        return _type_name;
    }

    inline size_t size() const{
        return _size;
    }

    std::string get_type_name() const;

    std::string full_type_name() const {
        switch (_type_name){
            case Type_name::INT    : return "int";
            case Type_name::FLOAT  : return "float";
            case Type_name::CHAR   :
                return "char(" + my_itoa(_size) + ")";
        }
    }

protected:
    Type_name _type_name;
    size_t _size;

    static std::string my_itoa(size_t size){ // 1 <= size <= 255
        if(size < 10) return std::string(1, size + '0');
        else if(size < 100)
            return std::string(1, size/10 + '0') + std::string(1, size%10 + '0');
        else
            return std::string(1, size/100 + '0') + std::string(1, size/10 + '0') + std::string(1, size%10 + '0');
    }
};

#endif //MINISQL_TYPE_INFO_H
