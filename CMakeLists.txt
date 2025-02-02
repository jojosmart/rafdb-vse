cmake_minimum_required(VERSION 3.0)
project(rafdb)

# 设置 C++98 标准
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB BASE_SOURCES "base/*.cc")

# 添加 base 文件夹为静态库
add_library(base STATIC ${BASE_SOURCES})

# 设置 base 的头文件路径
target_include_directories(base PUBLIC ${PROJECT_SOURCE_DIR}/base)

set_target_properties(base PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/base)

# 添加 src 文件夹的可执行文件
add_executable(rafdb src/main.cc)

#将项目的根目录添加到MyExecutable的包含路径中
target_include_directories(rafdb PUBLIC ${PROJECT_SOURCE_DIR})

# 链接 base 静态库和 libs 文件夹中的静态库
target_link_libraries(rafdb 
    ${PROJECT_SOURCE_DIR}/libs/base/base 
    ${PROJECT_SOURCE_DIR}/libs/third_party/thrift/thrift
)

# 设置可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/script)
