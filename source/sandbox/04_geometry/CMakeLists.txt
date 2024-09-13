cmake_minimum_required (VERSION 3.8)

project(04_geometry)


add_executable(04_geometry 
    "04_geometry.cpp"
)

target_link_libraries(04_geometry PRIVATE engine)
