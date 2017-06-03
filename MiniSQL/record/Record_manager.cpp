//
// Created by baislsl on 17-5-30.
//

#include "Record_manager.h"

Record_manager::Record_manager(Buffer_manager &_buffer_manager)
        : buffer_manager(_buffer_manager){
}

std::vector<Type_value>
Record_manager::match(const std::vector<Type_info> &type_infos, const std::vector<std::string> &values) {
    std::vector<Type_value> result;
    if (type_infos.size() != values.size())
        throw Data_error("value does not match to table");
    for (size_t i = 0; i < type_infos.size(); i++) {
        Type_value type_value(type_infos[i], values[i]);
        result.push_back(type_value);
    }
    return result;
}

Result_set Record_manager::select_table(const Table &table, const std::vector<std::string> &selects,
                                        std::vector<Condition> &conditions) {
    for(Condition &condition : conditions){
        Type_info type_info = table.get_column_info(condition.name());
        condition.build(type_info);
    }

    std::vector<size_t> select_offset = table.get_offset(selects),
            condition_offset = table.get_offset(conditions);
    std::vector<Column> columns = table.get_table_column();
    Result_set result_set(columns);
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
                strncpy(data, cache + condition_offset[cnt], length);
                Type_info type_info = conditions[cnt].type_info();
                if(!conditions[cnt].match(Type_value(type_info, data))){
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
                strncpy(data, cache + select_offset[cnt], length);
                Type_info type_info = column.value_type();
                block_data.push_back(Type_value(type_info, data));
            }
            result_set.data.push_back(block_data);
        }
    }
    return result_set;
}

void Record_manager::insert_table(const std::string &table_name, const std::vector<Type_info> &type_infos,
                                  const std::vector<std::string> &values) {
    std::vector<Type_value> result(match(type_infos, values));
    size_t size = 0;
    for(const Type_value& value : result){
        size += value.size();
    }
    char data[size + 1];
    size_t offset = 0;
    std::string path = basic_address + table_name + ".db";
    for (const Type_value &value : result) {
        strncpy(data + offset, value.data(), value.size());
        offset += value.size();
    }
    buffer_manager.app_write(path, data, size);
}

void Record_manager::insert_table(const Table &table, const std::vector<std::string> &values) {
    std::vector<Type_info> type_infos = table.get_table_type_infos();
    insert_table(table.name(), type_infos, values);
}
