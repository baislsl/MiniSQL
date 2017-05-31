cmake_minimum_required(VERSION 3.7)
project(MiniSQL)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Test/test.cpp Test/test.h MiniSQL/catalog/Catalog_manager.cpp MiniSQL/catalog/Catalog_manager.h MiniSQL/util/Type_value.h MiniSQL/util/Type_name.h MiniSQL/util/Table.h MiniSQL/util/Column.h MiniSQL/Interpreter/Interpreter.h MiniSQL/Interpreter/Error.h MiniSQL/API/API.cpp MiniSQL/API/API.h MiniSQL/buffer/Buffer_manager.h MiniSQL/record/Record_manager.cpp MiniSQL/record/Record_manager.h MiniSQL/util/Result_set.h MiniSQL/util/Condition.h MiniSQL/buffer/Buffer_manager.h)

add_executable(MiniSQL ${SOURCE_FILES})