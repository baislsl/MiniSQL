//
// Created by baislsl on 17-6-2.
//

#ifndef MINISQL_BNODE_H
#define MINISQL_BNODE_H


#include <cstddef>
#include "../util/Type_value.h"



// template <class T = Type_value, class N = BNext>
class BNode {

public:

    BNode() : size(0) {}

    inline bool full() const {
        return size == 2 * t;
    }

    inline bool root() const {
        return offset == 0;
    }

    inline bool small() const {
        return leaf ? size < t : size < t - 1;
    }

    inline Type_value begin() const {
        return node[0];
    }

    /// when the node is full, it will be divide to two parts
    void split(BNode &left, BNode &right, size_t left_offset, size_t right_offset, Type_value &value) {
        if(leaf){
            for (size_t i = 0; i < t; i++) {
                left.node[i] = std::move(node[i]);
                right.node[i] = std::move(node[i + t]);
                left.next[i] = next[i + 1];
                right.next[i] = next[i + 1 + t];
            }
            left.size = right.size = t;
            left.parent = right.parent = parent;
            left.leaf = right.leaf = true;
            left.offset = left_offset;
            right.offset = right_offset;
            value = right.begin();
        }else{
            for (size_t i = 0; i < t - 1; i++) {
                left.node[i] = std::move(node[i]);
                right.node[i] = std::move(node[i + t]);
                left.next[i] = next[i + 1];
                right.next[i] = next[i + 1 + t];
            }
            left.size = right.size = t - 1;
            left.parent = right.parent = parent;
            left.leaf = right.leaf = false;
            left.offset = left_offset;
            right.offset = right_offset;
            value = std::move(node[t - 1]);
        }
    }

    void insert(const Type_value &value, const size_t index) {
        size_t i = size - 1;
        while (node[i] > value) {
            node[i] = node[i - 1];
            next[i + 1] = next[i];
            i--;
        }
        node[i] = value;
        next[i + 1] = index;
    }

    void remove(const Type_value &value) {
        for (size_t i = 0; i < size; i++) {
            if (node[i] == value) {
                for (size_t j = i; j < size; j++) {
                    node[j] = node[j + 1];
                    next[j + 1] = next[j + 2];
                }
                --size;
                return;
            }
        }
        throw Data_error("No value in the index as ...");
    }

    size_t find_next(const Type_value &type_value) const;

    static const size_t t = 10;
    size_t size, offset; // offset do not need to store in the file, offset * 4K is the begin address in the file
    bool leaf;
    Type_value node[2 * t];
    size_t next[2 * t + 1];
    size_t parent;


};


#endif //MINISQL_BNODE_H
