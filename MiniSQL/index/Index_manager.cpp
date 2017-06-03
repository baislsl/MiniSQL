//
// Created by baislsl on 17-6-1.
//

#include "Index_manager.h"

Index_manager::Index_manager(Buffer_manager &_buffer_manager)
        : buffer_manager(_buffer_manager) {
}

void Index_manager::create_index(Index &index, const std::vector<Type_value> &values) {
    std::string path = get_path(index);
    BP_tree bp_tree(path, buffer_manager, index.size);
    for(auto cur = values.begin(); cur != values.end(); cur++){
        bp_tree.insert(*cur, cur - values.begin());
    }
    index.size = bp_tree.size();
}

void Index_manager::drop_index(const Index &index) {
    std::remove(static_cast<std::string>(basic_index_address + index.index_name + ".ind").c_str());
}

size_t Index_manager::select(const Index &index, const Type_value &select_value) {
    std::string path = get_path(index);
    BP_tree bp_tree(path, buffer_manager, index.size);
    return bp_tree.select(select_value).index;
}

void Index_manager::insert_index_value(Index &index, const Type_value &value, const size_t offset) {
    std::string path = get_path(index);
    BP_tree bp_tree(path, buffer_manager, index.size);
    bp_tree.insert(value, offset);
    index.size = bp_tree.size();
}

