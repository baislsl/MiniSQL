//
// Created by baislsl on 17-5-30.
//

#include "Record_manager.h"
#include "Record_exception.h"
#include "../util/Type_info.h"
#include "../util/Type_value.h"
#include "../util/Condition.h"
#include "../util/Table.h"
#include "../util/Column.h"
#include "../catalog/Catalog_exception.h"
#include "../util/String_handler.h"

Record_manager::Record_manager(Buffer_manager &_buffer_manager)
        : buffer_manager(_buffer_manager){
}

std::vector<Type_value>
Record_manager::match(const std::vector<Type_info> &type_infos, const std::vector<std::string> &values) {
    std::vector<Type_value> result;
    if (type_infos.size() != values.size())
        throw Table_value_not_found_error("value does not match to table");
    for (size_t i = 0; i < type_infos.size(); i++) {
        Type_value type_value(type_infos[i], values[i]);
        result.push_back(type_value);
    }
    return result;
}

Result_set Record_manager::select_table(const Table &table, const std::vector<std::string> &selects,
                                        std::vector<Condition> &conditions) {
    for(Condition &condition : conditions){
        Type_info type_info = table.get_column_info(String_handler::trim(condition.name()));
        condition.build(type_info);
    }

    std::vector<size_t> select_offset = table.get_offset(selects),
            condition_offset = table.get_offset(conditions),
            select_index = table.get_index(selects);
    std::vector<Column> columns = table.get_table_column();
    std::vector<Column> select_columns;
    for(size_t i : select_index){
        select_columns.push_back(columns[i]);
    }
    Result_set result_set(select_columns);
    size_t block_size = table.get_block_size(), row_number = table.get_row_number();
    std::string path = basic_address + table.name() + ".db";
    for (size_t i = 0; i < row_number; i++) {
        char *cache = buffer_manager.read(path, i * block_size, block_size);
        std::vector<Type_value> block_data;
        bool flag = true;
        if(!conditions.empty()){
            for(size_t cnt = 0;cnt < condition_offset.size();cnt++){
                size_t length =conditions[cnt].size();
                char data[length];
                memcpy(data, cache + condition_offset[cnt], length);
                Type_info type_info = conditions[cnt].type_info();
                Type_value type_value(type_info, data);
                if(!conditions[cnt].match(type_value)){
                    flag = false;
                    break;
                }
            }
        }

        if(flag){
            for (size_t cnt = 0; cnt < select_offset.size(); cnt++) {
                Column column = result_set.value_set[cnt];
                size_t length = column.size();
                char data[length];
                memcpy(data, cache + select_offset[cnt], length);
                Type_info type_info = column.value_type();
                block_data.push_back(Type_value(type_info, data));
            }
            result_set.data.push_back(block_data);
        }
    }
    return result_set;
}

void Record_manager::insert_table(const Table &table, const std::vector<std::string> &values) {
    std::vector<Type_info> type_infos = table.get_table_type_infos();
    std::vector<Type_value> result(match(type_infos, values));
    Result_set result_set = select_table(table);
    for(const std::vector<Type_value>& datas : result_set.data){
        auto i_result = result.begin();
        auto i_value = datas.begin();
        auto i_column = result_set.value_set.begin();
        for( ;
            i_result != result.end() && i_value != datas.end();
            ++i_result, ++i_value, ++i_column){
            if(i_column->find_attr(Column::UNIQUE)){
                if(*i_result == *i_value)
                    throw Conflict_error(
                            "Duplicated unique value for " + i_column->name + " in table " + table.name()
                    );
            }else if(i_column->find_attr(Column::PRIMARY)){
                if(*i_result == *i_value)
                    throw Conflict_error(
                            "Duplicated primary value for " + i_column->name + " in table " + table.name()
                    );
            }
        }
    }
    insert_table(table.name(), result);
}

void Record_manager::insert_table(const std::string &table_name, const std::vector<Type_value> &values) {
    size_t size = 0;
    for(const Type_value& value : values){
        size += value.size();
    }
    char data[size + 1];
    size_t offset = 0;
    std::string path = basic_address + table_name + ".db";
    for (const Type_value &value : values) {
        memcpy(data + offset, value.data(), value.size());
        offset += value.size();
    }
    buffer_manager.app_write(path, data, size);
}

void Record_manager::clear_table(const Table &table) {
    std::string path = basic_address + table.name() + ".db";
    buffer_manager.remove_file(path);
    buffer_manager.unset_block(path);
}

size_t Record_manager::delete_table(const Table &table, const std::vector<Condition> conditions) {
    std::vector<Condition> op_conditions;
    for(const Condition &condition : conditions){
        op_conditions.push_back(condition.opposite());
    }
    std::vector<std::string> selects;
    Result_set result_set = select_table(table, selects, op_conditions);
    // std::cout << result_set << std::endl;
    clear_table(table);
    auto &data = result_set.data;
    for(const std::vector<Type_value> values : data){
        insert_table(table.name(), values);
    }
    return data.size();
}

std::vector<Type_value> Record_manager::select_columns(const Table &table, std::string column) {
    std::vector<std::string> selects;
    selects.push_back(column);
    std::vector<Condition> conditions;
    Result_set result_set = select_table(table, selects, conditions);
    std::vector<Type_value> result;
    for(const std::vector<Type_value> &value : result_set.data){
        if(!value.empty())
            result.push_back(*(value.begin()));
    }
    return result;
}

Result_set Record_manager::select_table(const Table &table) {
    std::vector<Condition> tmp; std::vector<std::string> select_tmp;
    return select_table(table, select_tmp, tmp);
}



