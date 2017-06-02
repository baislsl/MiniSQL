//
// Created by baislsl on 17-5-30.
//

#include "API.h"

API::API() : catalog("../Test/cata.xml"),
             buffer_manager(),
             index_manager(buffer_manager),
             record_manager(buffer_manager) {
}

void API::create_table(const Table &table) {
    catalog.create_table(table);
}

void API::insert_table(const std::string &table_name, const std::vector<std::string> &items) {
    Table table = std::move(catalog.get_table_handler(table_name));
    record_manager.insert_table(table, items);
}

Result_set API::select_table(const std::string &table_name, const std::vector<std::string> &selects,
                             std::vector<Condition> &conditions) {
    Table table = catalog.get_table_handler(table_name);
    return record_manager.select_table(table, selects, conditions);
}
