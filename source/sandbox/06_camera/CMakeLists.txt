cmake_minimum_required(VERSION 3.8)

project(06_camera)

add_executable(06_camera 06_camera.cpp)
target_link_libraries(06_camera PRIVATE engine)
