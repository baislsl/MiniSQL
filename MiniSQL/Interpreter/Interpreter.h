//
// Created by baislsl on 17-5-29.
//

#ifndef MINISQL_INTERPRETER_H
#define MINISQL_INTERPRETER_H



#include "Interpreter_exception.h"
#include "API.h"
#include <iostream>
#include <sstream>
#include <regex>

class Interpreter {
public:
    ~Interpreter();

    Interpreter(std::istream &_in, std::ostream &_out);

    void exec_file(std::istream &in);

private:
    API api;
    std::ostream &out;
    std::istream &in;
    const std::string char_ptn = "\\s*(.+)\\s+(char)\\s*\\((\\d+)\\)(\\s+unique)?\\s*";// sgender char (1) |sgender|char|1||
    const std::string float_int_ptn = "\\s*(.+)\\s+(float|int)(\\s+unique)?\\s*";           // sage int|sage|int||
    const std::string create_table_ptn = "\\s*create[\\s+]table[\\s+]([^(]+)[\\s*]\\((.*)\\)";
    const std::string drop_table_ptn = "\\s*drop\\s+table\\s(.*)";
    const std::string primary_key_ptn = "\\s*primary\\s+key\\s*\\((.*)\\)";
    const std::string insert_ptn = "\\s*insert[\\s+]into[\\s+](.+)[\\s+]values[\\s*]\\((.*)\\)";
    const std::string select_condition_ptn = "\\s*select\\s(.+)[\\s+]from\\s+(.+)\\s+where[\\s+](.*)";
    const std::string select_ptn = "\\s*select\\s+(.+)[\\s+]from\\s+(.+)\\s*";
    const std::string create_index_ptn = "\\s*create\\s+index\\s+(.+)\\s+on(.+)\\s*\\((.+)\\)";
    const std::string drop_index_ptn = "\\s*drop\\s+index\\s+(.+)";
    const std::string quit_ptn = "\\s*quit\\s*";
    const std::string execfile_ptn = "\\s*execfile\\s+(.*)";
    const std::string delete_condition_ptn = "\\s*delete[\\s+]from\\s+(.+)\\s+where[\\s+](.*)";
    const std::string delete_ptn = "\\s*delete[\\s+]from\\s+(.+)\\s*";
    const std::string describe_ptn = "\\s*describe[\\s+](.+)";

    std::regex e_create_table, e_drop_table, e_insert, e_select, e_delete_condition, e_delete,
            e_select_condition, e_create_index, e_drop_index, e_quit, e_execfile, e_describe;

    void describe_table(const std::string &table_name);

    std::vector<Condition> analysis_condition(const std::string condition_lists);

    void delete_table(const std::string &table_name);

    void delete_table(const std::string &table_name, const std::string cond);

    bool run(const std::string &command);

    void on_exec_file(const std::string path);

    void select_table(const std::string &value_lists, const std::string &table_name, const std::string condition_lists= "");

    void create_index(const std::string &index_name, const std::string &table_name,
                      const std::string &column_name);

    void drop_index(const std::string &index_name);

    void insert_table(const std::string &table_name, const std::string &command);

    void create_table(const std::string &table_name, const std::string &command);

    void drop_table(const std::string &table_name);

    void handle_error(const std::exception &e);

    Condition get_condition(const std::string &condition_query);

};


#endif //MINISQL_INTERPRETER_H
