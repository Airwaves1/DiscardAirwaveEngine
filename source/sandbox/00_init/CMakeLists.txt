cmake_minimum_required (VERSION 3.8)

project(00_init)


add_executable(00_init 
    "00_init.cpp"
)

target_link_libraries(00_init PRIVATE engine)
