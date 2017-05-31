//
// Created by baislsl on 17-5-29.
//

#ifndef MINISQL_INTERPRETER_H
#define MINISQL_INTERPRETER_H


#include <iostream>
#include <sstream>
#include <regex>
#include "../util/Type_value.h"
#include "../util/Table.h"
#include "Error.h"
#include "../API/API.h"

class Interpreter {
public:
    ~Interpreter(){
    }

    Interpreter(std::istream &_in, std::ostream &_out)
            :in(_in), out(_out), api() {
        std::string line, input(" ");
        while (in >> line) {
            input.append(line);
            while (input.find(';') != std::string::npos) {
                size_t pos = input.find(';');
                std::string command = input.substr(0, pos);
                input = input.substr(pos + 1);
                run(command);
//                try{
//                    run(command);
//                }catch (const std::exception &e){
//                    handle_error(e);
//                }
            }
            input.append(" ");      // replace '\n' with blank
        }
    }

    void handle_error(const std::exception &e){
        out << e.what() << std::endl;
    }

private:
    API api;
    std::ostream& out;
    std::istream& in;
    const std::string char_ptn = "\\s*([[:alnum:]]+)\\s+(char)\\s*\\((\\d+)\\)(\\s+unique)?\\s*";// sgender char (1) |sgender|char|1||
    const std::string float_int_ptn = "\\s*([[:alnum:]]+)\\s+(float|int)(\\s+unique)?\\s*";           // sage int|sage|int||
    const std::string create_table_ptn = "\\s*create[\\s+]table[\\s+]([[:alnum:]]+)[\\s*]\\((.*)\\)";
    const std::string primary_key_ptn = "\\s*primary[\\s+]key[\\s*]\\((.*)\\)";
    const std::string insert_ptn = "\\s*insert[\\s+]into[\\s+]([[:alnum:]]+)[\\s+]values[\\s*]\\((.*)\\)";
    const std::string select_ptn = "\\s*select\\s(.+)[\\s]from\\s([[:alnum:]]+)\\s*[where\\strue\\s*]?";

    void run(const std::string &command) {
        std::smatch m;
        std::regex e_table(create_table_ptn, std::regex::icase);
        std::regex e_insert(insert_ptn, std::regex::icase);
        std::regex e_select(select_ptn, std::regex::icase);
//        out << "cmd=" << command << std::endl;
//        out << "create_table_regex=" << create_table_ptn << std::endl;
        if (std::regex_match(command, m, e_table)) {
//            std::cout << "table name=" << m.str(1)
//                      << "\n items=" << m.str(2) << std::endl;
            create_table(m.str(1), m.str(2));
        } else if(std::regex_match(command, m, e_insert)){
//            std::cout << "insert table name=" << m.str(1)
//                      << "\n items=" << m.str(2) << std::endl;
            insert_table(m.str(1), m.str(2));
            out << "finished insert";
        }else if(std::regex_match(command, m, e_select)){
            std::cout << m.str(1) << "|" << m.str(2) << std::endl;
            std::string selections = string_trim(m.str(1));
            vector<std::string> selects;
            vector<Condition> conditoins;
            if(selections != "*"){
                string_spilt(selections, selects);
            }
            out << api.select_table(m.str(2), selects, conditoins);
        }else{
            throw Parse_error("syntax in " + command);
        }
    }

    void insert_table(const std::string &table_name, const std::string& command){
        std::vector<std::string> items;
        string_spilt(command, items);
        std::vector<Condition> conditions;
        api.insert_table(table_name, items);
    }

    void create_table(const std::string &table_name, const std::string &command) {
        std::smatch m;
        std::vector<std::string> items;
        std::regex e_char(char_ptn, std::regex::icase);
        std::regex e_float_int(float_int_ptn, std::regex::icase);
        std::regex e_primary(primary_key_ptn, std::regex::icase);
        Table table(table_name);
        string_spilt(command, items);
        for (std::string str : items) {
            std::cout << str << std::endl;
            if (std::regex_match(str, m, e_char)) {
                std::string column_name = m.str(1),
                            type_name   = m.str(2),
                            size        = m.str(3),
                            unique      = m.str(4);
                Type_info value_info(Type_name::CHAR, atoi(size.c_str()));
                Column column(column_name, value_info, unique.empty() ? 0 : Column::UNQUE);
                table.add_column(column);
            } else if (std::regex_match(str, m, e_float_int)) {
                std::string column_name = m.str(1),
                            type_name   = m.str(2),
                            unique      = m.str(3);
                Type_info value_info(type_name == "float" ? Type_name::FLOAT : Type_name::INT);
                Column column(column_name, value_info, unique.empty() ? 0 : Column::UNQUE);
                table.add_column(column);
            } else if(std::regex_match(str, m, e_primary)) { // debug
               //  std::cout << m.str(1);
                std::vector<std::string> column_names;
                string_spilt(m.str(1), column_names);
                for (std::string col : column_names) {
                    col = string_trim(col);
                    table.add_column_attribute(col, Column::PRIMARY);
                }
            } else {
                throw Parse_error("syntax error in " + command);
            }
        }
        api.create_table(table);
    }

    /**
     * @brief find a comma index that was not include in parentheses
     * @return the smallest comma index that was not included in parentheses,
     *          if no such comma, return std::string::npos instead
     *
     * */
    static size_t string_find_comma(const std::string& str){
        int count = 0;
        for(size_t i = 0;i < str.size();i++){
            if(str[i] == '('){
                count++;
            }else if(str[i] == ')'){
                if(--count < 0)
                    count = 0;
            }else if(str[i] == ',' && count == 0){
                return i;
            }
        }
        return std::string::npos;
    }

    static std::string string_trim(const std::string& str){
        std::string::size_type begin = 0, end = str.size() - 1;
        while(begin <= end && str[begin] <= ' ') begin++;
        while(begin <= end && str[end] <= ' ') end--;
        return str.substr(begin, end + 1 - begin);
    }

    // split the string by ';'
    static void string_spilt(std::string str, std::vector<std::string> &result) {
        size_t pos;
        result.clear();
        while (pos = string_find_comma(str), pos != std::string::npos) {
            result.push_back(str.substr(0, pos));
            str = str.substr(pos + 1);
        }
        result.push_back(string_trim(str));
    }

    /* remove command , not finish yet */
    static std::string string_simplify(const std::string& str){
        size_t index = 0;
        for(index = 0;index<str.length();index++){
            if(str[index] == '#')
                return str.substr(0, index);
            else if(str[index] == '/' && index < str.length() - 1 && str[index + 1] == '*'){
                for(size_t i=index + 2;i < str[index + 1];i++){}
            }
        }
        return str;
    }
};


#endif //MINISQL_INTERPRETER_H
