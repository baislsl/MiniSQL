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
        buffer_manager.insert(table_name, result);
    }

    Result_set select_table(const Table &table,
                            const std::vector<std::string> &selects,
                            const std::vector<Condition> &conditions){
        std::vector<size_t> offsets;
        table.get_offset(selects, offsets);
        std::vector<Column> columns;
        table.get_table_column(columns, selects);
        Result_set result_set(columns);
        buffer_manager.select(table.name(), table.get_block_size(), offsets, result_set);
        return result_set;
    }

private:
    Buffer_manager& buffer_manager;

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
