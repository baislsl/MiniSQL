//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_RECORD_MANAGER_H
#define MINISQL_RECORD_MANAGER_H


#include "../util/Type_info.h"
#include "../util/Type_value.h"
#include "../buffer/Buffer_manager.h"
#include "../Interpreter/Error.h"
#include "../util/Result_set.h"
#include "../util/Condition.h"
#include "../util/Table.h"
#include <vector>
#include <string>

class Record_manager {
public:
    Record_manager(Buffer_manager &_buffer_manager);

    void insert_table(const Table &table, const std::vector<std::string> &values);

    void insert_table(const std::string &table_name,
                      const std::vector<Type_info> &type_infos,
                      const std::vector<std::string> &values);

    Result_set select_table(const Table &table,
                            const std::vector<std::string> &selects,
                            std::vector<Condition> &conditions);

    /**
     * only select the value lists of a given column
     *
     * */
    std::vector<Type_value> select_columns(const Table &table, std::string column){
        std::vector<std::string> selects;
        selects.push_back(column);
        std::vector<Condition> conditions;
        Result_set result_set = select_table(table, selects, conditions);
        std::vector<Type_value> result;
        for(const std::vector<Type_value> &value : result_set.data){
            result.push_back(*(value.begin()));
        }
        return result;
    }

private:
    Buffer_manager &buffer_manager;
    const std::string basic_address = "../Data/baislsl/";

    std::vector<Type_value> match(const std::vector<Type_info> &type_infos, const std::vector<std::string> &values);

};



#endif //MINISQL_RECORD_MANAGER_H
