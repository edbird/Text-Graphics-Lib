cmake_minimum_required(VERSION 3.0)

project(textgraphicslib VERSION 1.0)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(textgraphicslib main.cpp fc_helper.cpp sdlfontmanager.cpp sdlfonttexturemanager.cpp)
target_link_libraries(textgraphicslib ${SDL2_LIBRARIES})

configure_file(version.hpp.in version.hpp)
target_include_directories(textgraphicslib PUBLIC "${PROJECT_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

