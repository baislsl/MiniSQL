//
// Created by baislsl on 17-6-5.
//

#ifndef MINISQL_INDEX_EXCEPTION_H
#define MINISQL_INDEX_EXCEPTION_H

#include <string>
#include <exception>

class Data_not_found : public std::exception {
private:
    std::string errlog;
public:
    Data_not_found(const std::string &info) :
            errlog("Name not found error : " + info) {};

    virtual const char *what() const noexcept { return errlog.c_str(); }
};


#endif //MINISQL_EXCEPTION_H
