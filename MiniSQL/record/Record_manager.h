//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_RECORD_MANAGER_H
#define MINISQL_RECORD_MANAGER_H


#include <vector>
#include <string>
#include "../buffer/Buffer_manager.h"
#include "../util/Result_set.h"

class Condition;
class Table;
class Type_info;
class Type_value;

class Record_manager {
public:
    Record_manager(Buffer_manager &_buffer_manager);

    void insert_table(const std::string &table_name, const std::vector<Type_value> &values);

    void clear_table(const Table &table);

    /// return the size of table ater deletion
    size_t delete_table(const Table &table, const std::vector<Condition> conditions);

    void insert_table(const Table &table, const std::vector<std::string> &values);

    Result_set select_table(const Table &table,
                            const std::vector<std::string> &selects,
                            std::vector<Condition> &conditions);

    Result_set select_table(const Table &table);

    /**
     * only select the value lists of a given column
     *
     * */
    std::vector<Type_value> select_columns(const Table &table, std::string column);

private:
    Buffer_manager &buffer_manager;
    const std::string basic_address = "../Data/baislsl/";

    std::vector<Type_value> match(const std::vector<Type_info> &type_infos, const std::vector<std::string> &values);

};



#endif //MINISQL_RECORD_MANAGER_H
