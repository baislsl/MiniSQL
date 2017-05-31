//
// Created by baislsl on 17-5-30.
//

#ifndef MINISQL_BUFFER_MANAGER_H
#define MINISQL_BUFFER_MANAGER_H


#include <vector>
#include <fstream>
#include "../util/Type_value.h"
#include "../util/Result_set.h"

class Buffer_manager {
public:
    Buffer_manager(){

    }

    void select(const std::string &table_name,
                const size_t block_size,
                const std::vector<size_t> &offsets,
                Result_set &result_set){
        std::string address = basic_address + table_name + ".db";
        std::fstream in(address , std::fstream::in | std::fstream::ate);
        if(!in){
            throw File_construction_error("Fail to open file " + address);
        }
        auto end_mark = in.tellg();
        size_t cur = 0;
        in.seekg(cur, std::fstream::beg);
        while(in && in.tellg() != end_mark){
            std::vector<Type_value> block_data;
            for(size_t i =0;i< offsets.size();i++){
                Column column = result_set.value_set[i];
                size_t length = column.size();
                char data[length];
                in.read(data, length);
                Type_info type_info = column.value_type();
                block_data.push_back(Type_value(type_info, data));
            }
            cur += block_size;
            in.seekg(cur, std::fstream::beg);

            result_set.data.push_back(block_data);
        }
        in.close();

    }

    void insert(const std::string &table_name, const std::vector<Type_value> &values){
        std::string address = basic_address + table_name + ".db";
        std::ofstream out(address , std::ios::out | std::ios::binary);
        for(const Type_value &value : values){
            out.write(value.data(), value.size());
        }
        out.close();
    }

private:
    const std::string basic_address = "../Data/baislsl/";

};


#endif //MINISQL_BUFFER_MANAGER_H
