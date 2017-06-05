//
// Created by baislsl on 17-6-3.
//

#include "BP_tree.h"

BP_tree::BP_tree(const std::string _path, Buffer_manager &_buffer_manager, size_t size)
        : path(_path), buffer_manager(_buffer_manager), _size(size) {
    if (access(path.c_str(), F_OK) != 0) { // file not exist
        /// write an empty root index
        BP_node root;
        root.size = 0;
        root.leaf = true;
        root.parent = 0;//
        root.offset = 0;
        writein_node(root);
    }
}

void BP_tree::insert(const Type_value &value, const size_t index) {
    Key o(value, index);
    insert(o);
}

void BP_tree::insert(const Key &value) {
    BP_node node = find_leaf(value);
    node.insert(value);
    while (node.full()) {
        if (node.root()) {
            BP_node left, right;
            Key p;
            node.split(left, right, _size + 1, _size + 2, p);
            _size += 2;
            left.parent = right.parent = 0;
            writein_node(left);
            writein_node(right);

            // set node as the root instead
            node.leaf = true;
            node.offset = 0;
            node.node[0] = p;
            node.next[0] = left.offset;
            node.next[1] = right.offset;
            node.size = 1;
            node.parent = 0; // offset is 0, we do not care the parent
            break;
        } else {
            size_t offset = node.offset;
            size_t parent = node.parent;
            BP_node left, right;
            Key p;
            node.split(left, right, offset, ++_size, p);
            writein_node(left);
            writein_node(right);
            node = read_node(parent);
            node.insert(p, right.offset);
        }
    }
    writein_node(node);
}

Key BP_tree::select(const Type_value &value) {
    Key o(value);
    return select(o);
}

Key BP_tree::select(const Key &value) {
    BP_node node = find_leaf(value);
    return node.find_value(value);
}

void BP_tree::remove(const Type_value &value) {
    Key o(value);
    remove(o);
}

void BP_tree::remove(const Key &value) {
    BP_node node = find_leaf(value);
    node.remove(value);
    if (node.small()) {
        while (!node.root()) {
            BP_node parent = read_node(node.parent);
            bool is_left;
            BP_node slide = read_node(parent.get_slide(node.offset, is_left));
            size_t offset;
            Key remove = slide.remove_last(offset);
            if (slide.small()) {
                Key add = is_left ? parent.neibo(slide.offset, node.offset)
                                  : parent.neibo(node.offset, slide.offset);
                parent.remove(add);
                slide.insert(value, offset);
                if (is_left) {
                    combine(slide, node, add);
                } else {
                    combine(node, slide, add);
                }
            } else {
                node.insert(remove);
            }
        }
    }
}

void BP_tree::combine(BP_node &left, const BP_node &right, const Key &key) {
    left.combine(right, key);
    writein_node(left);
    drop_bp_node(right);
}

void BP_tree::drop_bp_node(const BP_node &node) {
    size_t offset = node.offset + 1;
    while (offset < _size) {
        BP_node move_node = read_node(offset);
        --move_node.offset;
        writein_node(move_node);
        offset++;
    }

    size_t begin = 0;
    while (begin < node.offset) {
        BP_node dec_node = read_node(offset);
        for (size_t &i : dec_node.next) {
            if (i > node.offset) {
                i--;
            }
        }
        ++begin;
    }
    --_size;
}

BP_node BP_tree::read_node(size_t offset) {
    char *data = buffer_manager.read(path, offset * (0x3ff << 2), 0x3ff << 2);
    BP_node node;
    read(&node.leaf, data, sizeof(node.leaf));
    read(&node.size, data, sizeof(node.size));
    read(&node.parent, data, sizeof(node.parent));
    read(&node.node, data, sizeof(node.node));
    read(&node.next, data, sizeof(node.next));
    node.offset = offset;
    return node;
}

BP_node BP_tree::find_leaf(const Key &value) {
    BP_node node = read_node(0);
    while (!node.leaf) {
        size_t next = node.find_next(value);
        node = read_node(next);
    }
    return node;
}

void BP_tree::writein_node(const BP_node &node) {
    char *data = new char[0x3ff << 2];
    char *cur = data;
    b_wirte(cur, &node.leaf, sizeof(node.leaf));
    b_wirte(cur, &node.size, sizeof(node.size));
    b_wirte(cur, &node.parent, sizeof(node.parent));
    b_wirte(cur, &node.node, sizeof(node.node));
    b_wirte(cur, &node.next, sizeof(node.next));
    buffer_manager.write(path, data, node.offset * (0x3ff << 2), 0x3ff << 2);
}
