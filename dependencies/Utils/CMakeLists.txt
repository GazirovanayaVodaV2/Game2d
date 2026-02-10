cmake_minimum_required(VERSION 3.31.6)
project(UtilsLib VERSION 1.0.0)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC 
    SDL3::SDL3
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${SDL3_INCLUDE_DIRS}
)

target_precompile_headers(${PROJECT_NAME} 
    PRIVATE 
        "pch.h"
)