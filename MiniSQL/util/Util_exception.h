//
// Created by baislsl on 17-6-5.
//

#ifndef MINISQL_UTIL_EXCEPTION_H
#define MINISQL_UTIL_EXCEPTION_H

#include <exception>
#include <string>

class Column_not_found_error : public std::exception {
private:
    std::string errlog;
public:
    Column_not_found_error(const std::string &info) :
            errlog("Column not found error : " + info) {};

    virtual const char *what() const noexcept { return errlog.c_str(); }
};

class Data_compare_error : public std::exception {
private:
    std::string errlog;
public:
    Data_compare_error(const std::string &info) :
            errlog("Data compare error : " + info) {};

    virtual const char *what() const noexcept { return errlog.c_str(); }
};

#endif //MINISQL_UTIL_EXCEPTION_H
