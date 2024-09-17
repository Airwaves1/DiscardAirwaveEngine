cmake_minimum_required(VERSION 3.8)

project(09_ecs)

add_executable(09_ecs 09_ecs.cpp)
target_link_libraries(09_ecs PRIVATE engine)
