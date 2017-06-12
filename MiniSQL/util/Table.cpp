//
// Created by baislsl on 17-6-2.
//

#include "Table.h"
#include "Type_info.h"
#include "Column.h"
#include "Condition.h"

Table::Table() {}

Table::Table(const std::string &table_name) : table_name(table_name) {}

void Table::add_column_attribute(const std::string &column_name, int attr) {
    for (Column &column : value_list) {
        if (column.name == column_name) {
            column.add_attribute(attr);
            return;
        }
    }
    throw Column_not_found_error("No column name as " + column_name);
}

std::vector<size_t> Table::get_offset(const std::vector<Condition> &conditions) const {
    std::vector<std::string> selects;
    for(const Condition &condition : conditions){
        selects.push_back(condition.name());
    }
    return get_offset(selects);
}

std::vector<size_t> Table::get_offset(const std::vector<std::string> &selects) const {
    std::vector<size_t> offsets;
    size_t  offset = 0;
    for (const Column &column : value_list) {
        std::string name = column.name;
        if (selects.empty() || std::find(selects.begin(), selects.end(), name) != selects.end()) {
            offsets.push_back(offset);
        }
        offset += column.size();
    }
    return offsets;
}

std::vector<size_t> Table::get_index(const std::vector<std::string> &selects) const {
    std::vector<size_t> indexes;
    for(auto column = value_list.begin(); column != value_list.end(); ++column){
        std::string name = column->name;
        if (selects.empty() || std::find(selects.begin(), selects.end(), name) != selects.end()) {
            indexes.push_back(size_t(column - value_list.begin()));
        }
    }
    return indexes;
}

std::vector<Column> Table::get_table_column() const {
    std::vector<Column> columns;
    columns = value_list;
    return columns;
}

std::vector<Column> Table::get_table_column(const std::vector<std::string> &selects) const {
    if (selects.empty()) {
        return get_table_column();
    } else {
        std::vector<Column> columns;
        for (const Column &column : value_list) {
            std::string name = column.name;
            if (std::find(selects.begin(), selects.end(), name) != selects.end()) {
                columns.push_back(column);
            }
        }
        return columns;
    }
}

const Type_info Table::get_column_info(const std::string &column_name) const {
    for(const Column &column : value_list){
        if(column.name == column_name){
            return column.value_type();
        }
    }
}

std::vector<Type_info> Table::get_table_type_infos() const {
    std::vector<Type_info> type_infos;
    for (const Column &column : value_list) {
        type_infos.push_back(column.value_type());
    }
    return type_infos;
}

std::vector<Type_info> Table::get_table_type_infos(const std::vector<std::string> &selects) const {
    if (selects.empty()) {
        return get_table_type_infos();
    } else {
        std::vector<Type_info> type_infos;
        for (const Column &column : value_list) {
            std::string name = column.name;
            if (std::find(selects.begin(), selects.end(), name) != selects.end()) {
                type_infos.push_back(column.value_type());
            }
        }
        return type_infos;
    }
}

size_t Table::get_column_offset(const std::string &column_name) const {
    size_t result = 0;
    for(const Column& column : value_list){
        if(column.name == column_name){
            return result;
        }else{
            result += column.size();
        }
    }
    throw Column_not_found_error("No column name " + column_name + " in " + table_name);
}

size_t Table::get_column_index(const std::string &column_name) const {
    for(auto column = value_list.begin(); column != value_list.end(); ++column){
        if(column->name == column_name)
            return (size_t)(column - value_list.begin());
    }
    throw Column_not_found_error("No column name " + column_name + " in " + table_name);
}

Column Table::get_column_handler(const std::string &column_name) const {
    for(const Column &column : value_list){
        if(column.name == column_name){
            return column;
        }
    }
    throw Column_not_found_error("No column name " + column_name + " in " + table_name);
}

void Table::add_column(const Column &column) {
    value_list.push_back(column);
    block_size += column.size();
}

size_t Table::get_column_number() const {
    return value_list.size();
}



