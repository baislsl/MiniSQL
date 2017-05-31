//
// Created by baislsl on 17-5-31.
//

#ifndef MINISQL_POOL_H
#define MINISQL_POOL_H

#include <string>
#include <iostream>
#include <fstream>
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

    Block() : var(0) {

    }

    inline void set_attr(int attr) {
        var |= attr;
    }

    inline void unset_attr(int attr) {
        var &= ~attr;
    }

    inline bool isLock() const {
        return (var & (int) LOCK) != 0;
    }

    inline bool inUse() const {
        return (var & (int) USE) != 0;
    }

    enum {
        USE = 1 << 0,
        LOCK = 1 << 1
    };
};

/// if memory pool was full, the program will drop the block that was most pre used
/// the most recently the pool was accessed, the less possible the pool will be remove
class Buffer_manager {
public:
    Buffer_manager() {
        for(int i=0;i<1024;i++){
            pool[i] = new char[4*1024];
        }
    }

    ~Buffer_manager(){
        for(int i=0;i<1024;i++){
            delete pool[i];
        }
    }

    /// length in 4(int), 8(float/double), 1-255(char[])
    /// note that the length can not be over the max size of a block
    char *read(std::string path, size_t offset, size_t length) {
        if(length > 0x3ff)
            throw Data_error("Too big memory required to read once");
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

    void app_write(const std::string &path, char *data, size_t length){
        std::ofstream out(path, std::fstream::binary | std::fstream::app | std::ios::out);
        out.write(data, length);
        size_t index = hash(path);
        while (blocks[index].isLock()) {
            index = (index + 1) & 0x3ff;
        }
        Block &block = blocks[index];
        if (block.inUse() && path == block.path){
            block.unset_attr(Block::USE);   // 数据被修改过, 不再有效
        }
    }

private:
    size_t power = 0;
    Block blocks[1024];
    char* pool[1024];

    void fill_block(Block &block, const std::string &path,const size_t offset, const size_t index) {
        std::string address = path;
        block.set_attr(Block::USE);
        block.path = path;
        block.address = index;
        block.length = 0x3ff << 2;
        block.offset = offset;
        std::fstream in(address , std::fstream::in);
        if(!in){
            throw File_construction_error("Fail to open file " + address);
        }
        in.seekg(offset, std::fstream::beg);
        in.read(pool[block.address], 0x3ff << 2);
        std::cout << pool[block.address] << std::endl;
        in.close();
    }

    static inline bool cover(size_t dis_offset, size_t dis_length,
                             size_t src_offset, size_t src_length){
        return src_offset >= dis_offset && dis_offset + dis_length >= src_offset + src_length;
    }

    static size_t hash(const std::string &str) {
        size_t ans = 0;
        for (size_t i = 0; i < str.size(); i++) {
            ans ^= (str[i] * str[i] - i) & 0x3ff;
        };
        return (ans * ans) & 0x3ff;
    }


};

#endif //MINISQL_POOL_H
