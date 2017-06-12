//
// Created by baislsl on 17-6-2.
//

#include <iomanip>
#include "Result_set.h"
#include "Type_value.h"
#include "Column.h"

Result_set::Result_set() {

}


Result_set::Result_set(const std::vector<Column> &attributes) {
    for (const Column &column: attributes) {
        value_set.push_back(column);
    }
}

std::ostream &operator<<(std::ostream &out, const Result_set &result_set) {
    if(result_set.value_set.size() == 0){
        out << "No value selected.\n";
        return out;
    }
    const std::vector<Column> &values = result_set.value_set;
    const int gap = 2;
    size_t length[values.size()]{0,};
    for (size_t i = 0; i < values.size(); i++) {
        length[i] = std::max(length[i], values[i].name.size());
    }
    for (const std::vector<Type_value> &type_values : result_set.data) {
        for (size_t i = 0; i < values.size(); i++) {
            length[i] = std::max(length[i], type_values[i].print_length());
        }
    }
    for (size_t i = 0; i < values.size(); i++) {
        length[i] += gap * 2;
    }

    out.flags(std::ios::left);
    Result_set::line(out, length, values.size());
    for (size_t i = 0; i < values.size(); i++) {
        out << "|" << std::string(gap, ' ') << std::setw(length[i] - gap) << values[i].name;
    }
    out << "|\n";
    Result_set::line(out, length, values.size());
    for (const std::vector<Type_value> &type_values : result_set.data) {
        for (size_t i = 0; i < values.size(); i++) {
            out << "|" << std::string(gap, ' ') << std::setw(length[i] - gap) << type_values[i];
        }
        out << "|\n";
    }
    Result_set::line(out, length, values.size());
    out << std::flush;
    return out;
}

std::ostream &Result_set::line(std::ostream &out, size_t length[], size_t size) {
    out << "+";
    while (size--) {
        out << std::string(*length, '-') << "+";
        ++length;
    }
    out << "\n";
    return out;
}
