//
// Created by baislsl on 17-5-28.
//

#include "Catalog_manager.h"
#include "Catalog_exception.h"
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "../util/Type_info.h"
#include "../util/Column.h"


Catalog_manager::Catalog_manager(const std::string &_filename) : filename(_filename) {
    ptree pt;
    read_xml(filename, pt);
    BOOST_FOREACH(const ptree::value_type &list, pt.get_child("minisql.table.list")) {
                    Table table;
                    table.table_name = list.second.get<std::string>("name");
                    table.row_number = list.second.get<int>("size");
                    size_t block_size = list.second.get<size_t>("block");
                    BOOST_FOREACH(const ptree::value_type &item, list.second.get_child("attribute")) {
                                    std::string column_name = std::move(item.second.data());
                                    std::string type_str = item.second.get<std::string>("<xmlattr>.type");
                                    size_t size = item.second.get<size_t>("<xmlattr>.size");
                                    int attr = item.second.get<int>("<xmlattr>.attr");
                                    Type_info type_info(type_str, size);
                                    Column column(column_name, type_info, attr);
                                    table.add_column(column);
                                }
                    table_map[table.table_name] = table;
                }
    BOOST_FOREACH(const ptree::value_type &list, pt.get_child("minisql.index.list")) {
                    Index index;
                    index.table_name = list.second.get<std::string>("table");
                    index.column_name = list.second.get<std::string>("column");
                    index.index_name = list.second.get<std::string>("name");
                    index.size = list.second.get<size_t>("size");
                    indexes.push_back(index);
                }
}

Catalog_manager::~Catalog_manager() {
    ptree pt;
    generate_ptree(pt);
    write_xml(filename, pt);
}

void Catalog_manager::read_menu_titles(std::vector<std::string> &result) const {
    // std::cout << pt.get<std::string>("minisql.table.list.tb.name");
    result.clear();
    for (auto &value : table_map) {
        result.push_back(value.first);
    }
}

void Catalog_manager::create_table(const Table &table) {
    if (table_conflict(table.name()))
        throw Conflict_error("duplicate table name for " + table.name());
    if (table_column_duplicate(table))
        throw Conflict_error("duplicate column name in table " + table.name());
    table_map[table.table_name] = table;
}

void Catalog_manager::drop_table(const std::string &table_name) {
    table_map.erase(table_name);
}

void Catalog_manager::generate_ptree(ptree &pt) {
    for (auto &value : table_map) {
        const Table &table = value.second;
        if (table.name().size() == 0) continue;
        ptree p_table;
        p_table.put("name", table.name());
        p_table.put("block", table.get_block_size());
        p_table.put("size", table.get_row_number());
        ptree col_list;
        for (const Column &column : table.value_list) {
            ptree col;
            col.put("", column.name);
            col.put("<xmlattr>.type", column.get_type_name());
            col.put("<xmlattr>.attr", column.attr);
            col.put("<xmlattr>.size", column.type_size());
            p_table.add_child("attribute.col", col);
        }
        pt.add_child("minisql.table.list.tb", p_table);
    }
    for (const Index &index : indexes) {
        ptree p_index;
        p_index.put("table", index.table_name);
        p_index.put("column", index.column_name);
        p_index.put("name", index.index_name);
        p_index.put("size", index.size);
        pt.add_child("minisql.index.list.tb", p_index);
    }
}

bool Catalog_manager::table_conflict(const std::string &table_name) const {
    std::vector<std::string> tables;
    read_menu_titles(tables);
    for (const std::string &name : tables) {
        if (table_name == name) {
            return true;
        }
    }
    return false;
}

bool Catalog_manager::table_column_duplicate(const Table &table) {
    size_t size = table.value_list.size();
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = i + 1; j < size; j++) {
            if (table.value_list[i].name == table.value_list[j].name)
                return true;
        }
    }
    return false;
}

Index Catalog_manager::get_index(const std::string &table_name, const std::string &column_name) {
    for (const Index &index : indexes) {
        if (index.table_name == table_name && index.column_name == index.column_name)
            return index;
    }
    throw Name_not_found_error("No column name " + column_name + " in table name " + table_name);
}

void Catalog_manager::drop_index(const std::string &index_name) {
    for (auto cur = indexes.begin(); cur != indexes.end(); ++cur) {
        if (cur->index_name == index_name) {
            indexes.erase(cur);
            return;
        }
    }
    throw Name_not_found_error("No index name " + index_name);
}

bool Catalog_manager::find_index(const std::string &index_name) {
    for (const Index &index : indexes) {
        if (index.index_name == index_name) {
            return true;
        }
    }
    return false;
}

void Catalog_manager::update_index(const Index &new_index) {
    for (Index &index : indexes) {
        if (index.index_name == new_index.index_name) {
            index = new_index;
        }
    }
}

std::vector<Index> Catalog_manager::get_indexes(const std::string &table_name) const{
    std::vector<Index> result;
    for (const Index &index : indexes) {
        if (index.table_name == table_name)
            result.push_back(index);
    }
    return result;
}

std::vector<Index> Catalog_manager::get_indexes(const Table &table) const {
    return get_indexes(table.name());
}

void Catalog_manager::update_table_size(const std::string table_name, size_t size) {
    table_map[table_name].row_number = size;
}

Index Catalog_manager::get_index(const std::string &index_name) {
    for (const Index &index : indexes) {
        if (index.index_name == index_name) {
            return index;
        }
    }
    throw Name_not_found_error("No index name " + index_name);
}

void Catalog_manager::create_index(const Index &index) {
    if (find_index(index))
        throw Conflict_error("Index name " + index.index_name + " has existed!");
    table_map[index.table_name].add_column_attribute(index.column_name, Column::INDEX);
    indexes.push_back(index);
}

std::vector<Column> Catalog_manager::get_table_columns(const std::string &table_name) {
    return table_map[table_name].get_table_column();
}

std::vector<Column>
Catalog_manager::get_table_columns(const std::string &table_name, const std::vector<std::string> &selects) {
    return table_map[table_name].get_table_column(selects);
}

void Catalog_manager::add_table_row(const std::string &table_name) {
    table_map[table_name].row_number += 1;
}

void Catalog_manager::get_table_type_infos(const std::string &table_name, std::vector<Type_info> &type_infos) {
    table_map[table_name].get_table_type_infos();
}

std::vector<Type_info>
Catalog_manager::get_table_type_infos(const std::string &table_name, const std::vector<std::string> &selects) {
    return table_map[table_name].get_table_type_infos(selects);
}

Table Catalog_manager::get_table_handler(const std::string &table_name) {
    return table_map[table_name];
}

