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
    friend class Record_manager;
    friend class api;
public:
    Result_set();

    Result_set(const std::vector<Column> &attributes);

    friend std::ostream &operator<<(std::ostream &out, const Result_set &result_set);

private:
    std::vector<std::vector<Type_value>> data;
    std::vector<Column> value_set;
};

#endif //MINISQL_RESULT_SET_H
