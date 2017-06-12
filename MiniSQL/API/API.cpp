//
// Created by baislsl on 17-5-30.
//
#include "../catalog/Catalog_manager.h"
#include "../record/Record_manager.h"
#include "../util/Result_set.h"
#include "../index/Index_manager.h"
#include "../index/Index.h"
#include "API.h"
#include "../catalog/Catalog_exception.h"
#include "../util/Column.h"
#include "../util/Type_value.h"
#include "../util/Condition.h"


API::API() : catalog("../Test/cata.xml"),
             buffer_manager(),
             index_manager(buffer_manager),
             record_manager(buffer_manager) {
}

void API::create_table(const Table &table) {
    catalog.create_table(table);
    std::vector<Column> columns = table.get_table_column();
    for (const Column &column : columns) {
        if (column.find_attr(Column::PRIMARY)) {
            create_index(default_index_name(table.name(), column.name), table.name(), column.name);
        }
    }
}

void API::drop_table(const std::string &table_name) {
    Table table = catalog.get_table_handler(table_name);
    std::vector<Index> indexes = catalog.get_indexes(table);
    catalog.drop_table(table_name);
    for (const Index &index : indexes) {
        index_manager.drop_index(index);
        catalog.drop_index(index.index_name);
    }
}

void API::insert_table(const std::string &table_name, const std::vector<std::string> &items) {
    Table table = catalog.get_table_handler(table_name);
    record_manager.insert_table(table, items);  // solve conflict on unique, primary
    catalog.add_table_row(table.name());
    std::vector<Index> indexes = catalog.get_indexes(table);
    for (Index &index : indexes) {  // update index
        size_t cnt = table.get_column_index(index.column_name);
        Type_info type_info = table.get_column_info(index.column_name);
        std::string value = items[cnt];
        Type_value type_value(type_info, value);
        size_t offset = table.get_row_number();
        index_manager.insert_index_value(index, type_value, offset);
        index.size += 1;
        catalog.update_index(index);
    }
}

Result_set API::select_table(const std::string &table_name, const std::vector<std::string> &selects,
                             std::vector<Condition> &conditions) {
    Table table = catalog.get_table_handler(table_name);
    Index index;
    if (conditions.size() == 1 && false) {
        try {
            index = catalog.get_index(table_name, conditions[0].name());
            throw Name_not_found_error("");
        } catch (const Name_not_found_error &) {   // can not find index
            return record_manager.select_table(table, selects, conditions);
        }
        // index_manager.select(index, conditions[0]);

    } else {
        return record_manager.select_table(table, selects, conditions);
    }
}

bool API::create_index(const std::string &index_name, const std::string &table_name,
                       const std::string &column_name) {
    Index index;
    index.index_name = index_name;
    index.table_name = table_name;
    index.column_name = column_name;
    if (catalog.find_index(index)) {
        throw Conflict_error("Index name " + index.index_name + " has existed");
    }
    const Table table = catalog.get_table_handler(index.table_name);
    index.size = table.get_row_number();
    std::vector<Type_value> values = record_manager.select_columns(table, index.column_name);
    const Column column = table.get_column_handler(index.column_name);
    if (!column.find_attr(Column::UNIQUE) && !column.find_attr(Column::PRIMARY)) {
        throw std::logic_error("Can not create index on a non unique column!");
    }
    index_manager.create_index(index, values);
    catalog.create_index(index);
}

bool API::drop_index(const std::string &index_name) {
    Index index = catalog.get_index(index_name);
    catalog.drop_index(index_name);
    index_manager.drop_index(index);
}

void API::delete_table(const std::string &table_name, const std::vector<Condition> &conditions) {
    if (conditions.size() == 0) {
        clear_table(table_name);
        catalog.update_table_size(table_name, 0);
        return;
    } else {
        std::vector<Condition> index_condi(conditions);
        const std::vector<std::string> selects;
        Result_set deletes = record_manager.select_table(table_name, selects, index_condi);
        const Table table = catalog.get_table_handler(table_name);
        size_t size = record_manager.delete_table(table, conditions);
        catalog.update_table_size(table_name, size);
        std::vector<Index> indexes = catalog.get_indexes(table);
        for (const std::vector<Type_value> &values : deletes.data) {
            for (const Index &index : indexes) {
                auto value = values.begin();
                auto column = deletes.value_set.begin();
                while (column != deletes.value_set.end() && column->name != index.column_name) {
                    ++value;
                    ++column;
                }
                index_manager.remove_value(index, *value);
            }
        }
        for (Index &index : indexes) {
            index.size -= size;
            catalog.update_index(index);
        }

    }

}

void API::clear_table(const std::string &table_name) {
    const Table table = catalog.get_table_handler(table_name);
    std::vector<Index> indexes = catalog.get_indexes(table);
    for (Index &index : indexes) {
        index_manager.drop_index(index);
        index.size = 0;
        catalog.update_index(index);
    }
    record_manager.clear_table(table);
}

std::string API::default_index_name(const std::string table_name, const std::string column_name) const {
    return "_sys_" + table_name + "_" + column_name;
}

Table API::describe_table(const std::string &table_name) {
    return catalog.get_table_handler(table_name);

}
