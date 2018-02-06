//
// Created by baislsl on 17-5-28.
//

#ifndef MINISQL_CATALOG_MANAGER_H
#define MINISQL_CATALOG_MANAGER_H

#include "Table.h"
#include "Index.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <map>
class ptree;
class Column;
class Type_info;
using boost::property_tree::ptree;

class Catalog_manager {
public:
    Catalog_manager(const std::string &_filename);

    ~Catalog_manager();

    void update_table_size(const std::string table_name, size_t size);

    void read_menu_titles(std::vector<std::string> &result) const;

    void create_table(const Table &table);

    void drop_table(const std::string &table_name);

    std::vector<Column> get_table_columns(const std::string &table_name);

    std::vector<Column> get_table_columns(
            const std::string &table_name, const std::vector<std::string> &selects);

    void add_table_row(const std::string &table_name);

    void get_table_type_infos(const std::string &table_name,
                                     std::vector<Type_info> &type_infos);

    std::vector<Type_info> get_table_type_infos(
            const std::string &table_name, const std::vector<std::string> &selects);

    Table get_table_handler(const std::string &table_name);

    void create_index(const Index &index);

    void drop_index(const std::string &index_name);

    inline bool find_index(const Index &index){
        return find_index(index.index_name);
    }

    bool find_index(const std::string &index_name);

    std::vector<Index> get_indexes(const std::string &table_name) const;

    std::vector<Index> get_indexes(const Table &table) const;

    void update_index(const Index& new_index);

    Index get_index(const std::string &table_name, const std::string &column_name);

    Index get_index(const std::string &index_name);

private:
    const std::string filename;
    std::map<std::string, Table> table_map;
    std::vector<Index> indexes;


    bool table_conflict(const std::string &table_name) const;

    bool table_column_duplicate(const Table &table);

    void generate_ptree(ptree &pt);
};


#endif //MINISQL_CATALOG_MANAGER_H
