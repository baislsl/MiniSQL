//
// Created by baislsl on 17-6-2.
//

#include "BNode.h"

size_t BNode::find_next(const Type_value &type_value) const {
    size_t begin = 0, end = size, mid;
    while (begin < end - 1) {
        mid = (begin + end) / 2;
        if (type_value >= node[mid]) {
            begin = mid;
        } else{
            end = mid;
        }
    }
    if (leaf) {
        return next[begin];
    } else {
        return next[begin + 1];
    }
}
