cmake_minimum_required (VERSION 3.8)

project(03_application)


add_executable(03_application 
    "03_application.cpp"
)

target_link_libraries(03_application PRIVATE engine)
