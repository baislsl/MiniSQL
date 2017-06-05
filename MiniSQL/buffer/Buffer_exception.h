//
// Created by baislsl on 17-6-5.
//

#ifndef MINISQL_BUFFER_EXCEPTION_H
#define MINISQL_BUFFER_EXCEPTION_H

#include <exception>
#include <string>

class Block_all_lock_error : public std::exception {
private:
    std::string errlog;
public:
    Block_all_lock_error(const std::string info)
            : errlog("Block all lock error :" + info) {}

    virtual const char *what() const throw () { return errlog.c_str(); }
};


class Fail_open_file_error : public std::exception {
private:
    std::string errlog;
public:
    Fail_open_file_error(const std::string &info)
            : errlog("Fail to open file error : " + info) {};

    virtual const char *what() const throw () { return errlog.c_str(); }
};


#endif //MINISQL_EXCEPTION_H
