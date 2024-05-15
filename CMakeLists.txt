cmake_minimum_required(VERSION 2.8)
project(mprpc)

#设置项目可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#设置项目库文件输入路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#设置项目编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)

#设置项目库文件搜索路径 -L
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src)   #框架源代码
add_subdirectory(example)   #使用框架的业务代码