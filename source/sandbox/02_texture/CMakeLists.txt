cmake_minimum_required (VERSION 3.8)

project(02_texture)


add_executable(02_texture 
    "02_texture.cpp"
)

target_link_libraries(02_texture PRIVATE engine)
