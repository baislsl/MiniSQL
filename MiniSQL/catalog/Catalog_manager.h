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
    Catalog_manager(const std::string &_filename);

    ~Catalog_manager();;

    void read_menu_titles(std::vector<std::string> &result) const;

    void create_table(const Table &table);

    void generate_ptree(ptree &pt);


    inline std::vector<Column> get_table_columns(const std::string &table_name){
        return table_map[table_name].get_table_column();
    }

    inline std::vector<Column> get_table_columns(
            const std::string &table_name, const std::vector<std::string> &selects) {
        return table_map[table_name].get_table_column(selects);
    }

    inline void add_table_row(const std::string &table_name) {
        table_map[table_name].row_number += 1;
    }

    inline void get_table_type_infos(const std::string &table_name,
                                     std::vector<Type_info> &type_infos) {
        table_map[table_name].get_table_type_infos();
    }

    inline std::vector<Type_info> get_table_type_infos(
            const std::string &table_name, const std::vector<std::string> &selects) {
        return table_map[table_name].get_table_type_infos(selects);
    }

    inline Table get_table_handler(const std::string &table_name){
        return table_map[table_name];
    }

private:
//    ptree pt;
    const std::string filename;
    std::map<std::string, Table> table_map;


    bool table_conflict(const std::string &table_name) const;

    bool table_column_duplicate(const Table &table);

};


#endif //MINISQL_CATALOG_MANAGER_H
