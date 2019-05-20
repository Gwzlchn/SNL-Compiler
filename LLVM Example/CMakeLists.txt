cmake_minimum_required(VERSION 3.6)
project(kaleidoscope)

set(CMAKE_CXX_STANDARD 11)
# 定义需要的库
set(LLVM_LINK_COMPONENTS
        Core
        Support

        )

# 使用llvm
find_package(LLVM REQUIRED CONFIG)

set(SOURCE_FILES toy.cpp)

# 添加头文件路径
message(STATUS "include path: ${LLVM_INCLUDE_DIRS}")
include_directories(${LLVM_INCLUDE_DIRS})

add_executable(kaleidoscope ${SOURCE_FILES})
message(STATUS "src path: ${SOURCE_FILES}")
# 寻找对应的库
llvm_map_components_to_libnames(llvm_libs ${LLVM_LINK_COMPONENTS})
message(STATUS "link path: ${LLVM_LINK_COMPONENTS}")
# 将库与目标代码一起链接
target_link_libraries(kaleidoscope ${llvm_libs})
