//
// Created by baislsl on 17-6-3.
//

#ifndef MINISQL_BP_TREE_H
#define MINISQL_BP_TREE_H


#include "../buffer/Buffer_manager.h"
#include<unistd.h>
#include <cstring>

class BP_node;
class Type_value;
class Key;

class BP_tree {
public:
    BP_tree(const std::string _path, Buffer_manager &_buffer_manager, size_t size);

    void insert(const Type_value &value, const size_t index);

    void insert(const Key &value);

    Key select(const Type_value &value);

    Key select(const Key &value);

    void remove(const Type_value &value);

    void remove(const Key &value);

    inline size_t size() {
        return _size;
    }

private:
    const std::string path;
    Buffer_manager &buffer_manager;
    size_t _size;

    const size_t _4K = 4096;

    inline void read(void *p, char *&data, size_t size) {
        memcpy(p, data, size);
        data += size;
    }

    inline void b_wirte(char *&p, const void *data, size_t size) {
        memcpy(p, data, size);
        p += size;
    }

    void combine(BP_node &left, const BP_node &right, const Key &key);

    void drop_bp_node(const BP_node &node);

    BP_node read_node(size_t offset);

    BP_node find_leaf(const Key &value);

    void writein_node(const BP_node &node);

};



#endif //MINISQL_BP_TREE_H
