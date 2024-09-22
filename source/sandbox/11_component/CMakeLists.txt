cmake_minimum_required(VERSION 3.8)

project(11_component)

add_executable(11_component 11_component.cpp)
target_link_libraries(11_component PRIVATE engine)
