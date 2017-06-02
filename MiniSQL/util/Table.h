//
// Created by baislsl on 17-5-29.
//

#ifndef MINISQL_TABLE_H
#define MINISQL_TABLE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "Type_info.h"
#include "Column.h"
#include "../Interpreter/Error.h"
#include "Condition.h"


class Table {
    friend class Catalog_manager;
    friend class API;
public:
    Table();

    Table(const std::string &table_name);

    inline void add_column(const Column &column) {
        value_list.push_back(column);
        block_size += column.size();
    }

    inline size_t get_block_size() const {
        return block_size;
    }

    inline size_t get_column_number() const {
        return value_list.size();
    }

    inline std::string name() const {
        return table_name;
    }

    inline size_t get_row_number()const{
        return row_number;
    }

    void add_column_attribute(const std::string &column_name, int attr);


    std::vector<size_t> get_offset(const std::vector<Condition> &conditions) const;

    std::vector<size_t> get_offset(const std::vector<std::string> &selects) const;

    std::vector<Column> get_table_column() const;

    std::vector<Column> get_table_column(const std::vector<std::string> &selects) const;

    const Type_info get_column_info(const std::string &table_name) const;

    std::vector<Type_info> get_table_type_infos() const;

    std::vector<Type_info> get_table_type_infos(const std::vector<std::string> &selects) const;

private:
    size_t block_size = 0;
    size_t row_number = 0;
    std::string table_name;
    std::vector<Column> value_list;
};


#endif //MINISQL_TABLE_H
