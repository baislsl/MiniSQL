//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_RECORD_MANAGER_H
#define MINISQL_RECORD_MANAGER_H


#include <vector>
#include <string>
#include "../util/Type_info.h"
#include "../util/Type_value.h"
#include "../buffer/Buffer_manager.h"
#include "../Interpreter/Error.h"
#include "../util/Result_set.h"
#include "../util/Condition.h"
#include "../util/Table.h"

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

private:
    Buffer_manager& buffer_manager;
    const std::string basic_address = "../Data/baislsl/";

    std::vector<Type_value> match(const std::vector<Type_info> &type_infos, const std::vector<std::string> &values);

};



#endif //MINISQL_RECORD_MANAGER_H
