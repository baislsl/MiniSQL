//
// Created by baislsl on 17-5-26.
//
#include "test.h"
#include "../MiniSQL/catalog/Catalog_manager.h"
#include "../MiniSQL/util/Type_value.h"
#include "../MiniSQL/Interpreter/Interpreter.h"
#include "../MiniSQL/buffer/Buffer_manager.h"
#include <fstream>

void test(){
    try{
        Catalog_manager cata("../Test/cata.xml");
        vector<string> input;
        cata.read_menu_titles(input);
        for(std::string pp : input){
            cout << pp << std::endl;
        }
    }catch (const std::exception& e){
        cout << "test\n";
        cerr << e.what();
    }
//    cout << k.back().value();

}

void file_IO(){
    ofstream out("../Test/io_test", ios::binary | ios::out);
    double a = 0.234;
    for(int i=0;i<100;i++){
        cout << sizeof(double) << " : " << sizeof(char) << endl;
        out.write((char*)(&a), 8);
        a += 1;
    }

    out.close();
}

void file_IO2() {
    ifstream in("../Test/io_test");
    union {
        int a;
        char cc[100];
    }value;
    ofstream out("../Test/io_test", ios::binary | ios::out);
    value.cc[0] = '3';
    value.cc[1] = '4';
    value.cc[2] = 0;
    out.write((char*)(&value), 1);
    out.close();
//    double g;
//    for (size_t i = 0; i < 100; i++) {
//        in.read((char*)(&g), 8);
//        cout << g << std::endl;
//    }
//    in.close();
}

void intpre_test(){
    ifstream in("../Test/sql");
    Interpreter interpreter(in, std::cout);
}

void hash_test(){
    cout << sizeof(double) << endl;
     file_IO();
     file_IO2();
     intpre_test();
//    cout << Buffer_manager::hash("gg");
//    cout << Buffer_manager::hash("bai");
//    cout << Buffer_manager::hash("baislsl");
}

void type_value_test(){
    Type_info info(Type_name::CHAR, 10);
    Type_value value(info , "gsdfg");
    Type_value value2;
    value2 = value;




}

int main(){
   // type_value_test();
     intpre_test();
//    std::string path = "gg.ind";
//    ofstream out(path);
//    int g = 10;
//    out.write((char*)(&g), 3);
//    out.close();
}
