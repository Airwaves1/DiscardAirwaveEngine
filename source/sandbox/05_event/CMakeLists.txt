cmake_minimum_required(VERSION 3.8)

project(05_event)

add_executable(05_event 05_event.cpp)
target_link_libraries(05_event PRIVATE engine)
