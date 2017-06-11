//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_RESULT_SET_H
#define MINISQL_RESULT_SET_H

#include "Type_value.h"
#include "Column.h"
#include <iostream>
#include <vector>

class Result_set {
public:
    std::vector<std::vector<Type_value>> data;
    std::vector<Column> value_set;

    Result_set();

    Result_set(const std::vector<Column> &attributes);

    friend std::ostream &operator<<(std::ostream &out, const Result_set &result_set);
private:
    static std::ostream &line(std::ostream &out, size_t length[], size_t size);
};

#endif //MINISQL_RESULT_SET_H
