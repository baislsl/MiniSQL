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

void API::drop_table(const std::string &table_name) {
    Table table = catalog.get_table_handler(table_name);
    std::vector<Index> indexes = catalog.get_indexes(table);
    catalog.drop_table(table_name);
    for(const Index &index : indexes){
        index_manager.drop_index(index);
    }
}

void API::insert_table(const std::string &table_name, const std::vector<std::string> &items) {
    Table table = catalog.get_table_handler(table_name);
    record_manager.insert_table(table, items);
    catalog.add_table_row(table.name());
    std::vector<Index> indexes = catalog.get_indexes(table);
    for (Index &index : indexes) {  // update index
        size_t cnt = table.get_column_index(index.column_name);
        Type_info type_info = table.get_column_info(index.column_name);
        std::string value = items[cnt];
        Type_value type_value(type_info, value);
        size_t offset = table.get_row_number();
        index_manager.insert_index_value(index, type_value, offset);
        catalog.update_index(index);
    }
}

Result_set API::select_table(const std::string &table_name, const std::vector<std::string> &selects,
                             std::vector<Condition> &conditions) {
    Table table = catalog.get_table_handler(table_name);
    return record_manager.select_table(table, selects, conditions);
}

bool API::create_index(const std::string &index_name, const std::string &table_name,
                       const std::string &column_name) {
    Index index;
    index.index_name = index_name;
    index.table_name = table_name;
    index.column_name = column_name;
    if(catalog.find_index(index)){
        throw Conflict_error("Index name " + index.index_name + " has existed");
    }
    const Table table = catalog.get_table_handler(index.table_name);
    index.size = table.get_row_number();
    std::vector<Type_value> values = record_manager.select_columns(table, index.column_name);
    const Column column = table.get_column_handler(index.column_name);
    index_manager.create_index(index, values);
    catalog.create_index(index);
}

bool API::drop_index(const std::string &index_name) {
    Index index = catalog.get_index(index_name);
    catalog.drop_index(index_name);
    index_manager.drop_index(index);
}

void API::delete_table(const std::string &table_name, const std::vector<Condition> &conditions) {
    if(conditions.size() == 0){
        clear_table(table_name);
        catalog.update_table_size(table_name, 0);
        return;
    }
    const Table table = catalog.get_table_handler(table_name);
    size_t size = record_manager.delete_table(table, conditions);
    catalog.update_table_size(table_name, size);

}

void API::clear_table(const std::string &table_name) {
    const Table table = catalog.get_table_handler(table_name);
    const std::vector<Index> indexes = catalog.get_indexes(table);
    for(const Index &index : indexes){
        index_manager.drop_index(index);
    }
    record_manager.clear_table(table);
}
