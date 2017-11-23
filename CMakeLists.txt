cmake_minimum_required(VERSION 2.8)
project(project2_akin_ilerle_2017400252)

set(CMAKE_CXX_FLAGS "-std=c++0x")

set(SOURCE_FILES Main.cpp Order.cpp Starbucks.cpp Employee.cpp StarbucksModel2.cpp)
add_executable(project2 ${SOURCE_FILES})