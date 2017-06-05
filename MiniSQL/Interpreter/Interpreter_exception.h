//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_INTERPRETER_EXCEPTION_H
#define MINISQL_INTERPRETER_EXCEPTION_H

#include <stdexcept>

class Parse_error : public std::exception {
private:
    std::string errlog;
public:
    Parse_error(const std::string &info) :
            errlog("Parse error : " + info) {};

    virtual const char *what() const throw () { return errlog.c_str(); }
};

#endif //MINISQL_ERROR_H
