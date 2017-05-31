//
// Created by baislsl on 17-5-29.
//

#ifndef MINISQL_TABLE_H
#define MINISQL_TABLE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "Type_name.h"
#include "Column.h"
#include "../Interpreter/Error.h"


class Table {
    friend class Catalog_manager;
    friend class API;
public:
    Table() {}

    Table(const std::string &table_name) : table_name(table_name) {}

    ~Table() {};

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

    void add_column_attribute(const std::string &column_name, int attr) {
        for (Column column : value_list) {
            if (column.name == column_name) {
                column.add_attribute(attr);
                return;
            }
        }
        throw Data_error("No column name as " + column_name);
    }


    void get_offset(const std::vector<std::string> &selects,
                    std::vector<size_t> &offsets) const {
        size_t  offset = 0;
        for (const Column &column : value_list) {
            std::string name = column.name;
            if (selects.empty() || std::find(selects.begin(), selects.end(), name) != selects.end()) {
                offsets.push_back(offset);
            }
            offset += column.size();
        }
    }

    inline void get_table_column(std::vector<Column> &columns) const {
        columns.clear();
        columns = value_list;
    }

    inline void get_table_column(std::vector<Column> &columns,
                                 const std::vector<std::string> &selects) const {
        if (selects.empty()) {
            get_table_column(columns);
        } else {
            columns.clear();
            for (const Column &column : value_list) {
                std::string name = column.name;
                if (std::find(selects.begin(), selects.end(), name) != selects.end()) {
                    columns.push_back(column);
                }
            }
        }
    }

    void get_table_type_infos(std::vector<Type_info> &type_infos) const {
        type_infos.clear();
        for (const Column &column : value_list) {
            type_infos.push_back(column.value_type());
        }
    }

    void get_table_type_infos(std::vector<Type_info> &type_infos,
                              const std::vector<std::string> &selects) const {
        if (selects.empty()) {
            get_table_type_infos(type_infos);
        } else {
            type_infos.clear();
            for (const Column &column : value_list) {
                std::string name = column.name;
                if (std::find(selects.begin(), selects.end(), name) != selects.end()) {
                    type_infos.push_back(column.value_type());
                }
            }
        }
    }

private:
    size_t block_size = 0;
    size_t row_number = 0;
    std::string table_name;
    std::vector<Column> value_list;
};


#endif //MINISQL_TABLE_H
