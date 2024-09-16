cmake_minimum_required(VERSION 3.8)

project(08_mesh)

add_executable(08_mesh 08_mesh.cpp)
target_link_libraries(08_mesh PRIVATE engine)
