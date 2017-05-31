//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_ERROR_H
#define MINISQL_ERROR_H

#include <stdexcept>

class Parse_error : public std::logic_error {
public:
    Parse_error(const std::string &info)
            : std::logic_error("Parse error : " + info) {};
};

class Data_error : public std::runtime_error {
public:
    Data_error(const std::string &info)
            : std::runtime_error("Data error : " + info) {};
};

class Conflict_error : public std::runtime_error{
public:
    Conflict_error(const std::string &info)
            :std::runtime_error("Conflict error: " + info){};
};

class File_construction_error : public std::runtime_error{
public:
    File_construction_error(const std::string &info)
            :std::runtime_error("File construct error" + info){};
};

#endif //MINISQL_ERROR_H
