//
// Created by baislsl on 17-5-28.
//

#ifndef MINISQL_CATALOG_MANAGER_H
#define MINISQL_CATALOG_MANAGER_H

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <map>
#include "../util/Table.h"

using boost::property_tree::ptree;

class Catalog_manager {
public:
    Catalog_manager(const std::string &_filename) : filename(_filename) {
        read_xml(filename, pt);
        BOOST_FOREACH(const ptree::value_type &list, pt.get_child("minisql.table.list")) {
                        Table table;
                        table.table_name = list.second.get<std::string>("name");
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

    ~Catalog_manager() {
        write_xml(filename, pt);
    };

    void read_menu_titles(std::vector<std::string> &result) const {
        // std::cout << pt.get<std::string>("minisql.table.list.tb.name");
        BOOST_FOREACH(const ptree::value_type &list, pt.get_child("minisql.table.list")) {
                        result.push_back(list.second.get<std::string>("name"));
                    }
    }

    void read_menu_items(const std::string &menu_title, std::vector<std::string> &result) const {
        BOOST_FOREACH(const ptree::value_type &list, pt.get_child("minisql.table.list")) {
                        if (list.second.get<std::string>("name") == menu_title) {
                            BOOST_FOREACH(const ptree::value_type &item, list.second.get_child("attribute")) {
                                            result.push_back(item.second.data());
                                        }
                        }
                    }
    }

    void create_table(const Table &table) {
        if (table_conflict(table.name()))
            throw Conflict_error("duplicate table name for " + table.name());
        if (table_column_duplicate(table))
            throw Conflict_error("duplicate column name in table " + table.name());
        ptree p_table;
        size_t block_size = 0;
        p_table.put("name", table.name());
        p_table.put("block", table.get_block_size());
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
        table_map[table.table_name] = table;
    }


    inline void get_table_columns(const std::string &table_name,
                                  std::vector<Column> &columns){
        return table_map[table_name].get_table_column(columns);
    }

    inline void get_table_columns(const std::string &table_name,
                                  std::vector<Column> &columns,
                                  const std::vector<std::string> &selects){
        return table_map[table_name].get_table_column(columns, selects);
    }


    inline void get_table_type_infos(const std::string &table_name,
                                     std::vector<Type_info> &type_infos) {
        table_map[table_name].get_table_type_infos(type_infos);
    }

    inline void get_table_type_infos(const std::string &table_name,
                                     std::vector<Type_info> &type_infos,
                                     const std::vector<std::string> &selects){
        table_map[table_name].get_table_type_infos(type_infos, selects);
    }
    inline Table get_table_handler(const std::string &table_name){
        return table_map[table_name];
    }

private:
    ptree pt;
    const std::string filename;
    std::map<std::string, Table> table_map;


    bool table_conflict(const std::string &table_name) const {
        std::vector<std::string> tables;
        read_menu_titles(tables);
        for (const std::string &name : tables) {
            if (table_name == name) {
                return true;
            }
        }
        return false;
    }

    bool table_column_duplicate(const Table &table) {
        size_t size = table.value_list.size();
        for (size_t i = 0; i < size - 1; i++) {
            for (size_t j = i + 1; j < size; j++) {
                if (table.value_list[i].name == table.value_list[j].name)
                    return true;
            }
        }
        return false;
    }

};


#endif //MINISQL_CATALOG_MANAGER_H
