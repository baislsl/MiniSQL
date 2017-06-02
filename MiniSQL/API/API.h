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



private:
    Catalog_manager catalog;
    Buffer_manager  buffer_manager;
    Record_manager record_manager;

};


#endif //MINISQL_API_H
