//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_API_H
#define MINISQL_API_H


#include "../catalog/Catalog_manager.h"
#include "../record/Record_manager.h"
#include "../buffer/Buffer_manager.h"
#include "../util/Result_set.h"
#include "../index/Index_manager.h"
#include "../util/Index.h"

class API {
public:
    API();

    void create_table(const Table& table);

    void insert_table(const std::string &table_name, const std::vector<std::string> &items);

    /**
     * @brief conduction selection on table
     * @param table_name table name
     * @param selects the result of the select columns,
     *          if this vector is empty, that supposes to select all
     * @param conditions the requirement of the selections,
     *          if this vector is empty, that supposes the condition is always true
     * */
    Result_set select_table(const std::string &table_name,
                            const std::vector<std::string> &selects,
                            std::vector<Condition> &conditions );

    bool create_index(const Index &index){
        if(catalog.find_index(index)){
            throw Conflict_error("Index name " + index.index_name + " has existed");
        }
        const Table table = catalog.get_table_handler(index.table_name);
        std::vector<Type_value> values = record_manager.select_columns(table, index.column_name);
        const Column column = table.get_column_handler(index.column_name);
        index_manager.create_index(index, values);
        catalog.create_index(index);
    }

    bool drop_index(const std::string &index_name){
        Index index = catalog.get_index(index_name);
        catalog.drop_index(index_name);
        index_manager.drop_index(index);
    }

private:
    Catalog_manager catalog;
    Buffer_manager  buffer_manager;
    Record_manager record_manager;
    Index_manager index_manager;
};


#endif //MINISQL_API_H
