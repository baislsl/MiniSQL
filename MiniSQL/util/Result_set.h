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
    friend class Buffer_manager;
public:
    Result_set() {

    }

    Result_set(const std::vector<Column> &attributes) {
        for (const Column &column: attributes) {
            value_set.push_back(column);
        }
    }

    friend std::ostream &operator<<(std::ostream &out, const Result_set &result_set) {
        out << "output result set";
        for(const Column& value_set : result_set.value_set){
            out << value_set.name << " : ";
        }
        out << std::endl;
        for(const std::vector<Type_value>& type_values : result_set.data){
            for(const Type_value& item : type_values){
                out << item << " -- ";
            }
            out << std::endl;
        }
        return out;
    }

private:
    std::vector<std::vector<Type_value>> data;
    std::vector<Column> value_set;
};

#endif //MINISQL_RESULT_SET_H
