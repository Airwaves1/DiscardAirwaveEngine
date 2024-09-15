cmake_minimum_required(VERSION 3.8)

project(07_framebuffer)

add_executable(07_framebuffer 07_framebuffer.cpp)
target_link_libraries(07_framebuffer PRIVATE engine)
