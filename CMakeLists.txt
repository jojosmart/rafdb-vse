cmake_minimum_required(VERSION 3.0)
project(rafdb)

# 设置 C++98 标准
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB BASE_SOURCES "base/*.cc")

include_directories(third_party/thrift/include)
include_directories(third_party/boost)

# 添加 base 文件夹为静态库
add_library(base STATIC ${BASE_SOURCES})


#将项目的根目录添加到base的包含路径中
target_include_directories(base PUBLIC ${PROJECT_SOURCE_DIR})

set_target_properties(base PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/base)

file(GLOB MAIN_SOURCES "src/*.cc")
file(GLOB THRIFT_GEN_SOURCES "proto/gen-cpp/*.cpp")
#删除thrift示例文件
list(REMOVE_ITEM THRIFT_GEN_SOURCES "${PROJECT_SOURCE_DIR}/proto/gen-cpp/RafdbService_server.skeleton.cpp")

# 添加 src 文件夹的可执行文件
add_executable(rafdb ${MAIN_SOURCES} ${THRIFT_GEN_SOURCES})

#将项目的根目录添加到rafdb的包含路径中
target_include_directories(rafdb PUBLIC ${PROJECT_SOURCE_DIR})


# 链接 base 静态库和 libs 文件夹中的静态库
target_link_libraries(rafdb 
    ${PROJECT_SOURCE_DIR}/libs/base/libbase.a
    ${PROJECT_SOURCE_DIR}/libs/third_party/thrift/libthrift.a
    ${PROJECT_SOURCE_DIR}/libs/third_party/thrift/libthriftnb.a
    ${PROJECT_SOURCE_DIR}/libs/third_party/thrift/libthriftz.a
    ${PROJECT_SOURCE_DIR}/libs/vse/libvse_n_cli_c.a
)

# 设置可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/script)
