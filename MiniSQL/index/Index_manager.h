//
// Created by baislsl on 17-6-1.
//

#ifndef MINISQL_INDEX_MANAGER_H
#define MINISQL_INDEX_MANAGER_H

#include "../util/Table.h"
#include "../buffer/Buffer_manager.h"
#include "../util/Index.h"
#include "BNode.h"
#include "BP_tree.h"

class Index_manager {

public:
    Index_manager(Buffer_manager &_buffer_manager);

    void create_index(const Index &index, const std::vector<Type_value> &values){
        std::string path = basic_index_address + index.index_name + ".ind";
        BP_tree bp_tree(path, buffer_manager);
        for(auto cur = values.begin(); cur != values.end(); cur++){
            bp_tree.insert(*cur, cur - values.begin());
        }
    }

    void drop_index(const Index &index){
        std::remove(static_cast<std::string>(basic_index_address + index.index_name + ".ind").c_str());
    }

    void update_index(const Table& table, const Column& column){

    }

    size_t select(const Index &index, const Type_value &select_value){
        std::string path = basic_index_address + index.index_name + ".ind";
        BP_tree bp_tree(path, buffer_manager);
        return bp_tree.select(select_value);
    }

private:
    const std::string basic_index_address = "../../Data/baislsl/";
    Buffer_manager &buffer_manager;
};


#endif //MINISQL_INDEX_MANAGER_H
