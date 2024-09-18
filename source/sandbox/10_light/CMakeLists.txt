cmake_minimum_required(VERSION 3.8)

project(10_light)

add_executable(10_light 10_light.cpp)
target_link_libraries(10_light PRIVATE engine)
