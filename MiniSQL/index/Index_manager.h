//
// Created by baislsl on 17-6-1.
//

#ifndef MINISQL_INDEX_MANAGER_H
#define MINISQL_INDEX_MANAGER_H

#include <vector>
#include <cstddef>
#include <string>
class Buffer_manager;
class Index;
class BP_tree;
class Type_value;

class Index_manager {

public:
    Index_manager(Buffer_manager &_buffer_manager);

    void remove_value(const Index &index, const Type_value &value);

    void create_index(Index &index, const std::vector<Type_value> &values);

    void drop_index(const Index &index);

    void insert_index_value(const Index &index, const Type_value &value, const size_t offset);

    size_t select(const Index &index, const Type_value &select_value);

private:
    const std::string basic_index_address = "../Data/baislsl/";
    Buffer_manager &buffer_manager;

    std::string get_path(const Index &index);

    inline std::string get_path(const std::string &index_name){
        return basic_index_address + index_name + ".ind";
    }
};



#endif //MINISQL_INDEX_MANAGER_H
