//
// Created by baislsl on 17-6-2.
//

#include "Result_set.h"

Result_set::Result_set() {

}


Result_set::Result_set(const std::vector<Column> &attributes) {
    for (const Column &column: attributes) {
        value_set.push_back(column);
    }
}

std::ostream &operator<<(std::ostream &out, const Result_set &result_set) {
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