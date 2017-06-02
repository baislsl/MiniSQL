//
// Created by baislsl on 17-5-31.
//

#ifndef MINISQL_POOL_H
#define MINISQL_POOL_H

#include <string>
#include <iostream>
#include <fstream>
#include "../Interpreter/Error.h"

/**
 * file : database/table_name.db
 * data : [offset, offset + length), may be small that 8*1024
 * the data was stored in pool[address]
 * */
class Block {
public:
    std::string path;
    std::size_t power;
    size_t offset;
    size_t length;
    size_t address;
    int var;

    Block();

    inline void set_attr(int attr);

    inline void unset_attr(int attr);

    inline bool isLock() const;

    inline bool inUse() const;

    enum {
        USE = 1 << 0,
        LOCK = 1 << 1
    };
};

/// if memory pool was full, the program will drop the block that was most pre used
/// the most recently the pool was accessed, the less possible the pool will be remove
class Buffer_manager {
public:
    Buffer_manager();

    ~Buffer_manager();

    /// length in 4(int), 8(float/double), 1-255(char[])
    /// note that the length can not be over the max size of a block
    char *read(std::string path, size_t offset, size_t length);

    void app_write(const std::string &path, char *data, size_t length);

private:
    size_t power = 0;
    Block blocks[1024];
    char* pool[1024];

    void fill_block(Block &block, const std::string &path,const size_t offset, const size_t index);

    static inline bool cover(size_t dis_offset, size_t dis_length,
                             size_t src_offset, size_t src_length);

    static size_t hash(const std::string &str);


};

#endif //MINISQL_POOL_H
