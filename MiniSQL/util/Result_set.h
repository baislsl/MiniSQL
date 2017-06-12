//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_RESULT_SET_H
#define MINISQL_RESULT_SET_H


#include <iostream>
#include <vector>
class Column;
class Type_value;

class Result_set {
public:
    std::vector<std::vector<Type_value>> data;
    std::vector<Column> value_set;

    Result_set();

    Result_set(const std::vector<Column> &attributes);

    friend std::ostream &operator<<(std::ostream &out, const Result_set &result_set);

    static std::ostream &line(std::ostream &out, size_t length[], size_t size);
private:

};

#endif //MINISQL_RESULT_SET_H
