//
// Created by baislsl on 17-6-2.
//

#ifndef MINISQL_BNODE_H
#define MINISQL_BNODE_H


#include <cstddef>
#include "../util/Type_value.h"

/**
 * the form store in memory
 *
 * non leaf node :
 *   leaf, size, parent, node[_size], next[_size+1],
 *
 * leaf node :
 *   leaf, size, parent, node[_size], next[_size]
 *
 *      next[_size] is the offset in the storage table
 *
 *
 * for leaf node, next[] store the index of the value in table
 * for non-leaf node, next[] store the index of the next node
 *
 * */

// template <class T = Type_value, class N = BNext>
class BNode {

public:

    BNode():size(0){}

    inline bool full() const {
        return size == 2 * t - 1;
    }


    inline bool small() const {
        return size < t - 1;
    }

    void insert(const Type_value &value, const size_t index){
        size_t i = size - 1;
        while(node[i] > value){
            node[i] = node[i - 1];
            next[i + 1] = next[i];
            i--;
        }
        node[i] = value;
        next[i + 1] = index;
    }

    void remove(const Type_value &value){
        for(size_t i = 0;i<size;i++){
            if(node[i] == value){
                for(size_t j = i; j < size;j++) {
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
    Type_value node[2 * t - 1];
    size_t next[2 * t];
    size_t parent;


};


#endif //MINISQL_BNODE_H
