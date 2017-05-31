//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_API_H
#define MINISQL_API_H


#include "../catalog/Catalog_manager.h"
#include "../record/Record_manager.h"
#include "../buffer/Buffer_manager.h"
#include "../util/Result_set.h"

class API {
public:
    API():buffer_manager(),
          catalog("../Test/cata.xml"),
          record_manager(buffer_manager){
    }

    void create_table(const Table& table){
        catalog.create_table(table);
    }

    void insert_table(const std::string &table_name, const std::vector<std::string> &items){
        std::vector<Type_info> type_infos;
        catalog.get_table_type_infos(table_name, type_infos);
        record_manager.insert_table(table_name, type_infos ,items);
    }

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
                            const std::vector<Condition> &conditions){
        Table table = catalog.get_table_handler(table_name);
        return record_manager.select_table(table, selects,  conditions);
    }

private:
    Catalog_manager catalog;
    Record_manager record_manager;
    Buffer_manager buffer_manager;

};


#endif //MINISQL_API_H
