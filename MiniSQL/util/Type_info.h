//
// Created by baislsl on 17-5-29.
//

#ifndef MINISQL_TYPE_INFO_H
#define MINISQL_TYPE_INFO_H

#include "../Interpreter/Error.h"

enum class Type_name{
    INT, CHAR, FLOAT
};
// value type, no data and name
class Type_info{
public:

    Type_info();;

    Type_info(std::string type_name, size_t size = 32);

    Type_info(Type_name _type_name, size_t size = 32);

    inline Type_name type_name() const{
        return _type_name;
    }

    inline size_t size() const{
        return _size;
    }

    std::string get_type_name() const;

protected:
    Type_name _type_name;
    size_t _size;
};

#endif //MINISQL_TYPE_INFO_H
