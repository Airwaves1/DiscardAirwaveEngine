cmake_minimum_required(VERSION 3.8)

add_library(external external.cpp)

#glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(glfw)

#glad
add_subdirectory(glad)

#glm
add_subdirectory(glm)
option(GLM_ENABLE_CXX_20 "Enable C++ 20" ON)

#imgui
add_subdirectory(imgui)

#stb_image
add_subdirectory(stb_image)

#nlohmann_json
add_subdirectory(nlohmann_json)


target_link_libraries(external PUBLIC glfw)
target_link_libraries(external PUBLIC glad)
target_link_libraries(external PUBLIC glm)
target_link_libraries(external PUBLIC ImGui)
target_link_libraries(external PUBLIC stb_image)
target_link_libraries(external PUBLIC nlohmann_json)