cmake_minimum_required(VERSION 3.6)
project(kaleidoscope)

set(CMAKE_CXX_STANDARD 11)

# 使用llvm来设置路径
find_package(LLVM REQUIRED CONFIG)

# 打印信息
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(SOURCE_FILES top.cpp)

# 添加头文件包含路径
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(kaleidoscope ${SOURCE_FILES})