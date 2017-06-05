//
// Created by baislsl on 17-6-2.
//

#include "Buffer_manager.h"

Buffer_manager::Buffer_manager() {
    for (size_t i = 0; i < 1024; i++) {
        pool[i] = new char[0x3ff << 2];
    }
}

Buffer_manager::~Buffer_manager() {
    for (size_t i = 0; i < 1024; i++) {
        delete pool[i];;
    }
}

char *Buffer_manager::read(std::string path, size_t offset, size_t length) {
    if (length > (0x3ff << 2))
        throw "Too big memory required to read once";
    size_t index = hash(path);
    while (blocks[index].isLock()) {
        index = (index + 1) & 0x3ff;
    }

    Block &block = blocks[index];
    if (block.inUse() && path == block.path && cover(block.offset, block.length, offset, length)) {
        block.power = ++power;
        if (power == 0) {
            // reset_power();
        }
    } else {    /// no memory in the cache
        fill_block(block, path, offset, index);
    }
    return pool[block.address] + offset - block.offset;
}

void Buffer_manager::app_write(const std::string &path, char *data, size_t length) {
    std::ofstream out(path, std::fstream::binary | std::fstream::app | std::ios::out);
    out.write(data, length);
    unset_block(path);
    out.close();
}

void Buffer_manager::write(const std::string &path, char *data, size_t offset, size_t length) {
    std::ofstream out(path, std::fstream::binary);
    out.seekp(offset);
    out.write(data, length);
    unset_block(path);
    out.close();
}

void Buffer_manager::fill_block(Block &block, const std::string &path, const size_t offset, const size_t index) {
    std::string address = path;
    block.set_attr(Block::USE);
    block.path = path;
    block.address = index;
    block.length = 0x3ff << 2;
    block.offset = offset;
    std::fstream in(address, std::fstream::in);
    if (!in) {
        throw Fail_open_file_error("Fail to open file " + address);
    }
    in.seekg(offset, std::fstream::beg);
    in.read(pool[block.address], 0x3ff << 2);
    std::cout << pool[block.address] << std::endl;
    in.close();
}

bool Buffer_manager::cover(size_t dis_offset, size_t dis_length, size_t src_offset, size_t src_length) {
    return src_offset >= dis_offset && dis_offset + dis_length >= src_offset + src_length;
}

size_t Buffer_manager::hash(const std::string &str) {
    size_t ans = 0;
    for (size_t i = 0; i < str.size(); i++) {
        ans ^= (str[i] * str[i] - i) & 0x3ff;
    };
    return (ans * ans) & 0x3ff;
}

void Buffer_manager::unset_block(const std::string path) {
    size_t index = hash(path);
    Block &block = blocks[index];
    if (block.inUse() && path == block.path) {
        block.unset_attr(Block::USE);   // 数据被修改过, 不再有效
    }

    while (blocks[index].isLock()) {
        index = (index + 1) & 0x3ff;
    }
    if (blocks[index].inUse() && path == block.path) {
        blocks[index].unset_attr(Block::USE);   // 数据被修改过, 不再有效
    }
}


