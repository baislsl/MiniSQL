//
// Created by baislsl on 17-5-31.
//

#ifndef MINISQL_POOL_H
#define MINISQL_POOL_H

#include "Block.h"
#include <string>
#include <iostream>
#include <fstream>
class Block;

/**
 * file : database/table_name.db
 * data : [offset, offset + length), may be small that 8*1024
 * the data was stored in pool[address]
 * */


/// if memory pool was full, the program will drop the block that was most pre used
/// the most recently the pool was accessed, the less possible the pool will be remove
class Buffer_manager {
public:
    Buffer_manager();

    ~Buffer_manager();

    Buffer_manager(const Buffer_manager &) = delete;

    Buffer_manager &operator=(const Buffer_manager &) = delete;

    /// length in 4(int), 8(float/double), 1-255(char[])
    /// note that the length can not be over the max size of a block
    char *read(std::string path, size_t offset, size_t length);

    void app_write(const std::string &path, char *data, size_t length);

    void write(const std::string &path, char *data, size_t offset, size_t length);

    void unset_block(const std::string path);

    void remove_file(const std::string &path);

private:
    size_t power = 0;
    Block blocks[1024];
    char *pool[1024];
    const size_t _4K = 4096;

    void fill_block(Block &block, const std::string &path, const size_t offset, const size_t index);

    static inline bool cover(size_t dis_offset, size_t dis_length,
                             size_t src_offset, size_t src_length);

    static size_t hash(const std::string &str);


};

#endif //MINISQL_POOL_H
