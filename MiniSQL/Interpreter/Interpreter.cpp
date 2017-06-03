//
// Created by baislsl on 17-6-2.
//
#include "Interpreter.h"

Interpreter::~Interpreter() {
}

Interpreter::Interpreter(std::istream &_in, std::ostream &_out)
        :in(_in), out(_out), api(),
         e_create_table(create_table_ptn, std::regex::icase),
         e_drop_table(drop_table_ptn, std::regex::icase),
         e_insert(insert_ptn, std::regex::icase),
         e_select(select_ptn, std::regex::icase),
         e_select_condition(select_condition_ptn, std::regex::icase),
         e_create_index(create_index_ptn, std::regex::icase),
         e_drop_index(drop_index_ptn, std::regex::icase),
         e_qiut(quit_ptn, std::regex::icase),
         e_execfile(execfile_ptn, std::regex::icase){
    exec_file(in);
}

void Interpreter::insert_table(const std::string &table_name, const std::string &command) {
    std::vector<std::string> items;
    string_spilt(command, items);
    for (std::string &str : items) {
        str = fix(str);
    }
    std::vector<Condition> conditions;
    api.insert_table(table_name, items);
}

void Interpreter::create_table(const std::string &table_name, const std::string &command) {
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
            Column column(column_name, value_info, unique.empty() ? 0 : Column::UNIQUE);
            table.add_column(column);
        } else if (std::regex_match(str, m, e_float_int)) {
            std::string column_name = m.str(1),
                        type_name   = m.str(2),
                        unique      = m.str(3);
            Type_info value_info(type_name == "float" ? Type_name::FLOAT : Type_name::INT);
            Column column(column_name, value_info, unique.empty() ? 0 : Column::UNIQUE);
            table.add_column(column);
        } else if (std::regex_match(str, m, e_primary)) { // debug
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

bool Interpreter::run(const std::string &command) {
    std::smatch m;
    if (std::regex_match(command, m, e_create_table)) {
        create_table(m.str(1), m.str(2));
        out << "succeed to create table " + m.str(1) << std::endl;
    } else if (std::regex_match(command, m, e_insert)) {
        insert_table(m.str(1), m.str(2));
        out << "finished insert on table " + m.str(1) << std::endl;
    } else if (std::regex_match(command, m, e_select)) {
        std::cout << m.str(1) << "|" << m.str(2) << std::endl;
        std::string selections = string_trim(m.str(1));
        std::vector<std::string> selects;
        std::vector<Condition> conditions; // no fill, always true condition
        if (selections != "*") {
            string_spilt(selections, selects);
        }
        out << api.select_table(m.str(2), selects, conditions);
    } else if (std::regex_match(command, m, e_select_condition)) {
        std::cout << m.str(1) << "|" << m.str(2) << "|" << m.str(3) << std::endl;
        std::string selections = string_trim(m.str(1));
        std::vector<std::string> selects;
        if (selections != "*") {
            string_spilt(selections, selects);
        }
        std::vector<Condition> conditions; // no fill, always true condition
        std::string condition_query = m.str(3);
        size_t pos;
        while (pos = condition_query.find("and"), pos != std::string::npos) {
            if (condition_query[pos + 3] > ' ') {
                throw Parse_error("Syntax error in condition sets " + m.str(3));
            }
            std::string str = condition_query.substr(0, pos);
            conditions.push_back(get_condition(str));
            condition_query = condition_query.substr(pos + 4);
        }
        conditions.push_back(get_condition(condition_query));
        out << api.select_table(m.str(2), selects, conditions);
    } else if (std::regex_match(command, m, e_create_index)) {
        std::string index_name = string_trim(m.str(1)), table_name = string_trim(m.str(2));
        std::string column_name = string_trim(m.str(3));
        create_index(index_name, table_name, column_name);
        out << "succeed to create index " + index_name + " on "
               + column_name + " of table " + table_name << std::endl;
    } else if (std::regex_match(command, m, e_drop_index)) {
        std::string index_name = m.str(1);
        drop_index(index_name);
        out << "succeed to drop index " + index_name;
    } else if (std::regex_match(command, m, e_drop_table)) {
        std::string table_name = string_trim(m.str(1));
        drop_table(table_name);
        out << "succeed to drop table " + table_name << std::endl;
    } else if (std::regex_match(command, m, e_qiut)) {
        out << "exit the minisql system" << std::endl;
        return false;
    } else if (std::regex_match(command, m, e_execfile)) {;
        std::string path = fix(string_trim(m.str(1)));
        out << "run sql file... " + path << std::endl;
        on_exec_file(path);
    } else {
        throw Parse_error("Syntax in " + command);
    }
    return true;
}

void Interpreter::exec_file(std::istream &in) {
    std::string line, input;
    while (in >> line) {
        input.append(line);
        while (input.find(';') != std::string::npos) {
            size_t pos = input.find(';');
            std::string command = input.substr(0, pos);
            input = input.substr(pos + 1);
            bool result = run(command);
//                try{
//                    bool result = run(command);
//                }catch (const std::exception &e){
//                    handle_error(e);
//                }
            if(!result) return;

        }
        input.append(" ");      // replace '\n' with blank
    }
}

void Interpreter::handle_error(const std::exception &e) {
    out << e.what() << std::endl;
}

size_t Interpreter::string_find_comma(const std::string &str) {
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

std::string Interpreter::string_trim(const std::string &str) {
    std::string::size_type begin = 0, end = str.size() - 1;
    while (begin <= end && str[begin] <= ' ') begin++;
    while (begin <= end && str[end] <= ' ') end--;
    return str.substr(begin, end + 1 - begin);
}

void Interpreter::string_spilt(std::string str, std::vector<std::string> &result) {
    size_t pos;
    result.clear();
    while (pos = string_find_comma(str), pos != std::string::npos) {
        result.push_back(str.substr(0, pos));
        str = str.substr(pos + 1);
    }
    result.push_back(string_trim(str));
}

Condition Interpreter::get_condition(const std::string &condition_query) {
    std::vector<std::string> split = string_cmp_split(condition_query);
    if (split.size() != 3)
        throw Parse_error("Illegal comparision form as" + condition_query);
    return Condition(split[0], split[1], fix(split[2]));
}

std::vector<std::string> Interpreter::string_cmp_split(const std::string &str) {
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
    if (str[op_position + 1] <= ' ') {
        op = str.substr(op_position, 1);
    } else {
        op = str.substr(op_position, 2);
    }

    std::string condition_split = "\\s*([[:alnum:]]+)\\s*[<|>|<=|>=|=|==]\\s*(.*)\\s*";
    std::regex e_split(condition_split, std::regex::icase);
    std::smatch m;
    if (!std::regex_match(str, m, e_split)) {
        throw Parse_error("");
    }
    std::vector<std::string> result;
    result.push_back(m.str(1));
    result.push_back(op);
    result.push_back(m.str(2));
    return result;
}

std::string Interpreter::fix(std::string str) {
    str = string_trim(str);
    size_t end = str.size() - 1;
    if (str[0] == str[end] && (str[0] == '\'' || str[0] == '\"'))
        return str.substr(1, end - 1);
    else return str.substr(0, end + 1);
}

std::string Interpreter::string_simplify(const std::string &str) {
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

void
Interpreter::create_index(const std::string &index_name, const std::string &table_name,
                          const std::string &column_name) {
    api.create_index(index_name, table_name, column_name);
}

void Interpreter::drop_index(const std::string &index_name) {
    api.drop_index(index_name);
}

void Interpreter::drop_table(const std::string &table_name) {
    api.drop_table(table_name);
}

void Interpreter::on_exec_file(const std::string path) {
    std::ifstream file(path);
    exec_file(file);
    file.close();
}

