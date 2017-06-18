//
// Created by baislsl on 17-6-19.
//

#ifndef MINISQL_STRING_HANDLER_H
#define MINISQL_STRING_HANDLER_H

#include <string>
#include <vector>

class String_handler {
public:
    /**
     * @brief find a comma index that was not include in parentheses
     * @return the smallest comma index that was not included in parentheses,
     *          if no such comma, return std::string::npos instead
     * */
    static size_t find_comma(const std::string &str);

    static std::string trim(const std::string &str);

    // split the string by ','
    static void string_spilt(std::string str, std::vector<std::string> &result);

    /* remove command , not finish yet */
    static std::string simplify(const std::string &str);

    static std::vector<std::string> cmp_split(const std::string &str);

    static std::string rid_comment(const std::string command);

    /**
     * rid the string of quotation marks
     * */
    static std::string fix(std::string str);
};


#endif //MINISQL_STRING_HANDLER_H
