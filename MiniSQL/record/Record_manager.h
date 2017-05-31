//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_RECORD_MANAGER_H
#define MINISQL_RECORD_MANAGER_H


#include <vector>
#include <string>
#include "../util/Type_name.h"
#include "../util/Type_value.h"
#include "../buffer/Buffer_manager.h"
#include "../Interpreter/Error.h"
#include "../util/Result_set.h"
#include "../util/Condition.h"
#include "../util/Table.h"

class Record_manager {
public:
    Record_manager(Buffer_manager &_buffer_manager)
            : buffer_manager(_buffer_manager){
    }

    void insert_table(const std::string &table_name,
                      const std::vector<Type_info> &type_infos,
                      const std::vector<std::string> &values) {
        std::vector<Type_value> result;
        legal(type_infos, values, result);
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

    Result_set select_table(const Table &table,
                            const std::vector<std::string> &selects,
                            const std::vector<Condition> &conditions){
        std::vector<size_t> offsets;
        table.get_offset(selects, offsets);
        std::vector<Column> columns;
        table.get_table_column(columns, selects);
        Result_set result_set(columns);
        size_t block_size = table.get_block_size(), row_number = table.get_row_number();
        std::string path = basic_address + table.name() + ".db";
        for (size_t i = 0; i < row_number; i++) {
            char *cache = buffer_manager.read(path, i * block_size, block_size);
            std::vector<Type_value> block_data;
            for (size_t cnt = 0; cnt < offsets.size(); cnt++) {
                Column column = result_set.value_set[cnt];
                size_t length = column.size();
                char data[length];
                strncpy(data, cache + offsets[cnt], length);
                Type_info type_info = column.value_type();
                block_data.push_back(Type_value(type_info, data));
            }
            result_set.data.push_back(block_data);
        }
        return result_set;
    }

private:
    Buffer_manager& buffer_manager;
    const std::string basic_address = "../Data/baislsl/";

    void legal(const std::vector<Type_info> &type_infos,
               const std::vector<std::string> &values,
               std::vector<Type_value> &result) {
        result.clear();
        if (type_infos.size() != values.size())
            throw Data_error("value does not match to table");
        for (size_t i = 0; i < type_infos.size(); i++) {
            Type_value match(type_infos[i], values[i]);
            result.push_back(match);
        }
    }

};


#endif //MINISQL_RECORD_MANAGER_H
