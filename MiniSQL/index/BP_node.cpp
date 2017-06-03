//
// Created by baislsl on 17-6-3.
//

#include "BP_node.h"

void BP_node::combine(const BP_node &com, const Key &key) {
    node[size] = key;
    for(size_t i =0;i<com.size;i++){
        node[size + 1 + i] = com.node[i];
        next[size + 1 + i] = com.next[i];
    }
    next[size + 1 + com.size ] = com.next[size];
    size += com.size;
}

void BP_node::split(BP_node &left, BP_node &right, const size_t left_offset, const size_t right_offset, Key &value) {
    if (leaf) {
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
    } else {
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

Key BP_node::remove_last(size_t &offset) {
    offset = next[size];
    --size;
    return node[size];
}

Key BP_node::neibo(const size_t a, const size_t b) {
    for (int i = 0; i < size; i++) {
        if (next[i] == a && next[i + 1] == b) {
            return node[i];
        }
    }
    throw Data_error("error when search in the BP_node");
}

size_t BP_node::get_slide(const size_t &ob, bool &is_left) const {
    for (size_t i = 0; i <= size; i++) {
        if (next[i] == ob) {
            if (i == 0) {
                is_left = false;
                return next[i + 1];
            } else {
                is_left = true;
                return next[i - 1];
            }
        }
    }
}

void BP_node::insert(const Key &value, const size_t &offset) { // only for nonleaf
    node[size] = value;
    next[size + 1] = offset;
    ++size;
}

void BP_node::remove(const Key &value, bool) { // only for nonleaf node
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

void BP_node::remove(const Key &value) {   // only leaf node is available
    for (size_t i = 0; i < size; i++) {
        if (node[i] == value) {
            for (size_t j = i; j < size; j++) {
                node[j] = node[j + 1];
            }
            --size;
            return;
        }
    }
    throw Data_error("No value in the index as ...");
}

Key BP_node::find_value(const Key &value) const {
    for (size_t i = 0; i < size; i++) {
        if (node[i] == value)
            return node[i];
    }
}

size_t BP_node::find_next(const Key &value) const {  // only for non leaf node
    size_t i = 0;
    while (i < size && node[i] <= value) i++;
    return next[i];
}