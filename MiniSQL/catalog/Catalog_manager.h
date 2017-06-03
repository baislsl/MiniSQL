//
// Created by baislsl on 17-5-28.
//

#ifndef MINISQL_CATALOG_MANAGER_H
#define MINISQL_CATALOG_MANAGER_H

#include "../util/Table.h"
#include "../index/Index.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <map>

using boost::property_tree::ptree;

class Catalog_manager {
public:
    Catalog_manager(const std::string &_filename);

    ~Catalog_manager();;

    void read_menu_titles(std::vector<std::string> &result) const;

    void create_table(const Table &table);

    void drop_table(const std::string &table_name);

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

    inline void create_index(const Index &index){
        if(find_index(index))
            throw Conflict_error("Index name " + index.index_name + " has existed!");
        indexes.push_back(index);
    }

    void drop_index(const std::string &index_name);

    inline bool find_index(const Index &index){
        return find_index(index.index_name);
    }

    bool find_index(const std::string &index_name);

    std::vector<Index> get_indexes(const Table &table) const;

    void update_index(const Index& new_index);

    Index get_index(const std::string &table_name, const std::string &column_name);

    Index get_index(const std::string &index_name){
        for(const Index &index : indexes){
            if(index.index_name == index_name){
                return index;
            }
        }
        throw Data_error("No index name " + index_name);
    }

private:
    const std::string filename;
    std::map<std::string, Table> table_map;
    std::vector<Index> indexes;


    bool table_conflict(const std::string &table_name) const;

    bool table_column_duplicate(const Table &table);

    void generate_ptree(ptree &pt);
};


#endif //MINISQL_CATALOG_MANAGER_H
