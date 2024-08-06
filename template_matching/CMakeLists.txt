cmake_minimum_required(VERSION 3.10)

# 项目名称
project(template_matching)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找OpenCV库
find_package(OpenCV REQUIRED)

# 包含头文件目录
include_directories(${OpenCV_INCLUDE_DIRS})

# 添加可执行文件
add_executable(num_match src/num_match.cpp)

# 链接OpenCV库
target_link_libraries(num_match ${OpenCV_LIBS})
