cmake_minimum_required(VERSION 3.7)
project(MiniSQL)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES MiniSQL/catalog/Catalog_manager.cpp MiniSQL/catalog/Catalog_manager.h MiniSQL/util/Type_value.h MiniSQL/util/Type_info.h MiniSQL/util/Table.h MiniSQL/util/Column.h MiniSQL/Interpreter/Interpreter.h MiniSQL/Interpreter/Interpreter_exception.h MiniSQL/API/API.cpp MiniSQL/API/API.h MiniSQL/buffer/Buffer_manager.h MiniSQL/record/Record_manager.cpp MiniSQL/record/Record_manager.h MiniSQL/util/Result_set.h MiniSQL/util/Condition.h MiniSQL/buffer/Buffer_manager.h MiniSQL/index/Index_manager.cpp MiniSQL/index/Index_manager.h MiniSQL/util/Condition.cpp MiniSQL/Interpreter/Interpreter.cpp MiniSQL/buffer/Buffer_manager.cpp MiniSQL/util/Result_set.cpp MiniSQL/util/Type_info.cpp MiniSQL/util/Type_value.cpp MiniSQL/util/Table.cpp MiniSQL/util/Column.cpp MiniSQL/index/Index.h MiniSQL/buffer/Block.cpp MiniSQL/buffer/Block.h MiniSQL/index/BP_tree.cpp MiniSQL/index/BP_tree.h MiniSQL/index/BP_node.cpp MiniSQL/index/BP_node.h MiniSQL/buffer/Buffer_exception.h MiniSQL/catalog/Catalog_exception.h MiniSQL/index/Index_exception.h MiniSQL/record/Record_exception.h MiniSQL/util/Util_exception.h MiniSQL/util/String_handler.cpp MiniSQL/util/String_handler.h)

include_directories(MiniSQL)
include_directories(MiniSQL/API)
include_directories(MiniSQL/buffer)
include_directories(MiniSQL/catalog)
include_directories(MiniSQL/index)
include_directories(MiniSQL/Interpreter)
include_directories(MiniSQL/record)
include_directories(MiniSQL/util)

add_executable(MiniSQL ${SOURCE_FILES} Main.cpp)
add_executable(MiniSQLTest ${SOURCE_FILES} Test/test.cpp Test/test.h)