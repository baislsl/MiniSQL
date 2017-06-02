//
// Created by baislsl on 17-5-28.
//

#include "Catalog_manager.h"


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
                                    std::cout << "type = " << type_info.get_type_name() << size << std::endl;
                                }
                    table_map[table.table_name] = table;
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
    for(auto &value : table_map){
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

void Catalog_manager::generate_ptree(ptree &pt) {
    for (auto &value : table_map) {
        const Table &table = value.second;
        ptree p_table;
        size_t block_size = 0;
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
