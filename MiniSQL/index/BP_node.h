//
// Created by baislsl on 17-6-3.
//

#ifndef MINISQL_BP_NODE_H
#define MINISQL_BP_NODE_H

#include "../util/Type_value.h"
struct Key {
    Type_value value;
    size_t index;

    Key() {}

    Key(const Type_value &_value) : value(_value) {}

    Key(const Type_value &_value, const size_t _index) : value(_value), index(_index) {}

    friend bool operator<(const Key &a, const Key &b) {
        return a < b;
    }

    friend bool operator==(const Key &a, const Key &b) {
        return a == b;
    }

    friend bool operator!=(const Key &a, const Key &b) {
        return !(a == b);
    }

    friend bool operator>(const Key &a, const Key &b) {
        return b < a;
    }

    friend bool operator<=(const Key &a, const Key &b) {
        return !(b < a);
    }

    friend bool operator>=(const Key &a, const Key &b) {
        return !(a < b);
    }

};

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
 * the node is the root if offset is 0 (the root of the BP tree is always stored in the first of the file
 * nonleaf node : [t - 1, 2t - 1]
 * leaf node : [t, 2t - 1]
 * */
class BP_node {
friend class BP_tree;
public:

    BP_node() : size(0) {}

    inline bool full() const {
        return size == 2 * t;
    }

    inline bool root() const {
        return offset == 0;
    }

    inline bool small() const {
        return leaf ? size < t : size < t - 1;
    }

    inline Key begin() const {
        return node[0];
    }

    void combine(const BP_node& com, const Key &key);

    /// when the node is full, it will be divide to two parts
    void split(BP_node &left, BP_node &right, const size_t left_offset, const size_t right_offset, Key &value);

    Key remove_last(size_t &offset);

    Key neibo(const size_t a, const size_t b);

    /// @brief return the slide child of the node next to ob,
    /// @param is_left  true if the return child is left to the given object child
    ///         we set default value is is_leaf = true if it is possible
    size_t get_slide(const size_t &ob, bool &is_left) const;

    void insert(const Key &value, const size_t &offset);

    void insert(const Key &value) { // only leaf node is available
        node[size++] = value;
    }


    void remove(const Key &value, bool);

    void remove(const Key &value);

    Key find_value(const Key &value) const;;

    size_t find_next(const Key &value) const;

private:
    static const size_t t = ((0x3ff << 2) - 4 * sizeof(size_t ) - sizeof(bool))/(2 * sizeof(Key) + 2*sizeof(size_t)) ;
    size_t size;
    size_t offset; // offset do not need to store in the file, offset * 4K is the begin address in the file
    int leaf;
    Key node[2 * t];
    size_t next[2 * t + 1];
    size_t parent;
};


#endif //MINISQL_BP_NODE_H
