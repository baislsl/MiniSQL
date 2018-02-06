//
// Created by baislsl on 17-6-19.
//

#include "String_handler.h"
#include "Interpreter_exception.h"

size_t String_handler::find_comma(const std::string &str) {
    int count = 0;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == '(') {
            count++;
        } else if (str[i] == ')') {
            if (--count < 0)
                count = 0;
        } else if (str[i] == ',' && count == 0) {
            return i;
        }
    }
    return std::string::npos;
}

std::string String_handler::trim(const std::string &str) {
    if(str.empty()) return str;
    std::string::size_type begin = 0, end = str.size() - 1;
    while (begin <= end && str[begin] <= ' ') begin++;
    while (begin <= end && str[end] <= ' ') end--;
    return str.substr(begin, end + 1 - begin);
}

void String_handler::string_spilt(std::string str, std::vector<std::string> &result) {
    size_t pos;
    result.clear();
    while (pos = find_comma(str), pos != std::string::npos) {
        result.push_back(trim(str.substr(0, pos)));
        str = str.substr(pos + 1);
    }
    result.push_back(trim(str));
}

std::vector<std::string> String_handler::cmp_split(const std::string &str) {
    static char a[] = {'<', '>', '='};
    size_t op_position = str.find('<');
    if (op_position == std::string::npos) {
        op_position = str.find('>');
        if (op_position == std::string::npos) {
            op_position = str.find('=');
            if (op_position == std::string::npos) {
                throw Parse_error("Illegal comparision sets as " + str);
            }
        }
    }
    std::string op;
    std::vector<std::string> result;
    if (str[op_position + 1] <= ' ') {
        op = str.substr(op_position, 1);
        result.push_back(str.substr(0, op_position - 1));
        result.push_back(str.substr(op_position, 1));
        result.push_back(str.substr(op_position  + 1));
    } else {        // <=, >=
        result.push_back(str.substr(0, op_position - 1));
        result.push_back(str.substr(op_position, 2));
        result.push_back(str.substr(op_position  + 2));
    }
    return result;
}

std::string String_handler::fix(std::string str) {
    str = trim(str);
    size_t end = str.size() - 1;
    if (str[0] == str[end] && (str[0] == '\'' || str[0] == '\"'))
        return str.substr(1, end - 1);
    else return str.substr(0, end + 1);
}

std::string String_handler::simplify(const std::string &str) {
    size_t index = 0;
    for (index = 0; index < str.length(); index++) {
        if (str[index] == '#')
            return str.substr(0, index);
        else if (str[index] == '/' && index < str.length() - 1 && str[index + 1] == '*') {
            for (size_t i = index + 2; i < str[index + 1]; i++) {}
        }
    }
    return str;
}


std::string String_handler::rid_comment(const std::string command) {
    bool cnt_1 = false, cnt_2 = false;
    bool advance = false;          //advance : true if " appear before '
    size_t index = 0;
    while(index < command.length()){
        if(command[index] == '\''){
            if(!cnt_1){
                cnt_1 = true;
                if(cnt_2){
                    advance = true;
                }
            }else{
                cnt_1 = false;
                if(!advance){
                    cnt_2 = false;
                }
            }
        }else if(command[index] == '\"'){
            if(!cnt_2){
                cnt_2 = true;
                if(cnt_1){
                    advance = false;
                }
            }else{
                cnt_2 = false;
                if(advance){
                    cnt_1 = false;
                }
            }
        }else if(command[index] == '#'){
            if(!cnt_1 && !cnt_2){
                return command.substr(0, index);
            }
        }
        index++;
    }
    return command;

}
