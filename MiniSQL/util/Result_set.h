//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_RESULT_SET_H
#define MINISQL_RESULT_SET_H

#include <iostream>
#include <vector>
#include "Column.h"
#include "Type_value.h"

class Result_set {
    friend class Record_manager;
public:
    Result_set();

    Result_set(const std::vector<Column> &attributes);

    friend std::ostream &operator<<(std::ostream &out, const Result_set &result_set);

private:
    std::vector<std::vector<Type_value>> data;
    std::vector<Column> value_set;
};

#endif //MINISQL_RESULT_SET_H
