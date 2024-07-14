cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(MyProject)

# 查找 OpenCV 包
find_package(OpenCV REQUIRED)

# 设置包含目录
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

# 添加可执行文件
add_executable(opencv_fir src/main.cpp)

# 链接 OpenCV 库
target_link_libraries(opencv_fir ${OpenCV_LIBS})
