cmake_minimum_required(VERSION 3.8)

project(09_model)

add_executable(09_model 09_model.cpp)
target_link_libraries(09_model PRIVATE engine)
