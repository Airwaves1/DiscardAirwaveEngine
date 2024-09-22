cmake_minimum_required(VERSION 3.8)

project(12_model)

add_executable(12_model 12_model.cpp)
target_link_libraries(12_model PRIVATE engine)
