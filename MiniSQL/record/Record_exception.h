//
// Created by baislsl on 17-6-5.
//

#ifndef MINISQL_RECORD_EXCEPTION_H
#define MINISQL_RECORD_EXCEPTION_H

#include <string>
#include <exception>

class Table_value_not_found_error : public std::exception {
private:
    std::string errlog;
public:
    Table_value_not_found_error(const std::string &info) :
            errlog("Table value not found error : " + info) {};

    virtual const char *what() const throw () { return errlog.c_str(); }
};

#endif //MINISQL_RECORD_EXCEPTION_H
