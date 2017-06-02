//
// Created by baislsl on 17-6-2.
//

#include "Column.h"

Column::Column(const std::string &name, const Type_info &value_info, const int attr)
        : name(name), Type_info(value_info), attr(attr) {}
