//
// Created by baislsl on 17-6-2.
//

#ifndef MINISQL_BLOCK_H
#define MINISQL_BLOCK_H


#include <cstddef>
#include <string>
#include "Buffer_exception.h"

#define MAX_PIN 1024

class Block {
public:
    std::string path;
    std::size_t power;
    size_t offset;
    size_t length;
    size_t address;
    int var;

    Block();

    inline void set_attr(int attr) {
        var |= attr;
    }

    inline void unset_attr(int attr){
        var &= ~attr;
    }

    inline bool isLock() const{
        return (var & (int) LOCK) != 0;
    }

    inline bool inUse() const{
        return (var & (int) USE) != 0;
    }

    enum {
        USE = 1 << 0,
        LOCK = 1 << 1
    };
private:
};


#endif //MINISQL_BLOCK_H
