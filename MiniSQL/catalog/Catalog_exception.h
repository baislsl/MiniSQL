//
// Created by baislsl on 17-6-5.
//

#ifndef MINISQL_CATALOG_EXCEPTION_H
#define MINISQL_CATALOG_EXCEPTION_H


#include <exception>
#include <string>

class Name_not_found_error : public std::exception {
private:
    std::string errlog;
public:
    Name_not_found_error(const std::string &info) :
            errlog("Name not found error : " + info) {};

    virtual const char *what() const noexcept { return errlog.c_str(); }
};

class Conflict_error : public std::exception {
private:
    std::string errlog;
public:
    Conflict_error(const std::string &info) :
            errlog("Conflict error : " + info) {};

    virtual const char *what() const noexcept { return errlog.c_str(); }
};

#endif //MINISQL_EXCEPTION_H
