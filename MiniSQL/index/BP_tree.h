//
// Created by baislsl on 17-6-3.
//

#ifndef MINISQL_BP_TREE_H
#define MINISQL_BP_TREE_H


#include "BNode.h"
#include "../buffer/Buffer_manager.h"
#include<unistd.h>

class BP_tree {
public:
    BP_tree(const std::string _path, Buffer_manager &_buffer_manager)
            : path(_path), buffer_manager(_buffer_manager) {
        if( access( path.c_str(), F_OK ) != 0 ){ // file no exist
            /// wirte a empty root index
            BNode root;
            root.size = 0;
            root.leaf = true;
            root.parent = 0;//
            root.offset = 0;
            writein_node(root);
        }
    }

    void insert(const Type_value &value, const size_t index) {
        BNode node = find_node(value);
        if (node.full()) {
            ///
        } else {
            node.insert(value, index);
            writein_node(node);
        }
    }

    size_t select(const Type_value &value) {
        BNode node = find_node(value);
        return node.find_next(value);
    }

    void remove(const Type_value &value) {
        BNode node = find_node(value);
        node.remove(value);
        if (node.small()) {
            ///
        }

    }

private:
    std::string path;
    Buffer_manager &buffer_manager;

    BNode read_node(char *data, size_t offset) {
        BNode node;
        char flag;
        read(&node.leaf, data, sizeof(node.leaf));
        read(&node.size, data, sizeof(node.size));
        read(&node.parent, data, sizeof(node.parent));
        read(&node.node, data, sizeof(node.node));
        read(&node.next, data, sizeof(node.next));
        node.offset = offset;
        return node;
    }

    inline void read(void *p, char *&data, size_t size) {
        memcpy(p, data, size);
        data += size;
    }

    inline void b_wirte(char *&p, const void *data, size_t size) {
        memcpy(p, data, size);
        p += size;
    }

    BNode find_node(const Type_value &value) {
        char *data = buffer_manager.read(path, 0, 0x3ff << 2);
        BNode node = read_node(data, 0);
        while (!node.leaf) {
            size_t next = node.find_next(value);
            data = buffer_manager.read(path, next * (0x3ff << 2), 0x3ff << 2);
            node = read_node(data, next);
        }
        return node;
    }

    void writein_node(const BNode &node) {
        char *data = new char[0x3ff << 2];
        char *cur = data;
        b_wirte(cur, &node.leaf, sizeof(node.leaf));
        b_wirte(cur, &node.size, sizeof(node.size));
        b_wirte(cur, &node.parent, sizeof(node.parent));
        b_wirte(cur, &node.node, sizeof(node.node));
        b_wirte(cur, &node.next, sizeof(node.next));
        buffer_manager.write(path, data, node.offset * (0x3ff << 2), 0x3ff << 2);
    }

};


#endif //MINISQL_BP_TREE_H
