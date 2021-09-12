cmake_minimum_required(VERSION 3.7)

project(textgraphicslib VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_MODULE_PATH /home/ecb/Worlds-Best-Text-Editor-3/Text-Graphics-Lib)

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
include_directories(textgraphicslib ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})

set(SOURCE_FILES main.cpp)
set(TEXTGRAPHICSLIB_SOURCE_FILES fc_helper.cpp sdlfontmanager.cpp sdlfontmanager_fontmanagerinitsuccess.cpp sdlfonttexturemanager.cpp fc_helper.cpp sdlhelper.cpp tty_write_functions.cpp)

add_library(textgraphicslib ${TEXTGRAPHICSLIB_SOURCE_FILES})
target_link_libraries(textgraphicslib PUBLIC SDL2::SDL2)
target_include_directories(textgraphicslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(textgraphicslibexe ${SOURCE_FILES})

configure_file(version.hpp.in version.hpp)
target_include_directories(textgraphicslib PUBLIC "${PROJECT_BINARY_DIR}")

#target_link_libraries(textgraphicslibexe PRIVATE textgraphicslib)
#target_link_libraries(textgraphicslibexe PRIVATE textgraphicslib fontconfig)
target_link_libraries(textgraphicslibexe PRIVATE textgraphicslib ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY} fontconfig)
