cmake_minimum_required(VERSION 3.8)

project(09_light)

add_executable(09_light 09_light.cpp)
target_link_libraries(09_light PRIVATE engine)
