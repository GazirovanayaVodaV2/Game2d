cmake_minimum_required(VERSION 3.20)
project(Game2d VERSION 0.1.0)

# Поддержка C++20 требует минимум CMake 3.20 для правильной компиляции с Android NDK
if(ANDROID)
    # Минимальный API level для Android
    if(NOT ANDROID_PLATFORM)
        set(ANDROID_PLATFORM android-24)
    endif()
    
    # STL для Android
    if(NOT ANDROID_STL)
        set(ANDROID_STL c++_shared)
    endif()
endif()

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Определить директории для сборки в зависимости от типа
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Установить выходные директории
if(ANDROID)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs/${ANDROID_ABI}")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/git")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/git")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Android-специфичные опции
if(ANDROID)
    set(SDL_X11_XTEST OFF CACHE BOOL "" FORCE)
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_FORCE_STATIC_VCRT OFF CACHE BOOL "" FORCE)
    set(SDL_LIBC ON CACHE BOOL "" FORCE)
else()
    set(SDL_X11_XTEST OFF CACHE BOOL "" FORCE) 
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
endif()


FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/git/SDL3"
    UPDATE_DISCONNECTED ON
)

FetchContent_Declare(SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG main
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/git/SDL_image"
    UPDATE_DISCONNECTED ON
)

FetchContent_Declare(SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG main
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/git/SDL_ttf"
    UPDATE_DISCONNECTED ON
)
FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/git/nlohmann_json"
    UPDATE_DISCONNECTED ON
)
FetchContent_MakeAvailable(SDL3 SDL3_image SDL3_ttf nlohmann_json)

add_subdirectory(dependencies/Utils)
add_subdirectory(dependencies/GuiLib)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/headers/*.h")

# Для Android создаём shared library, для остального - executable
if(ANDROID)
    add_library(${PROJECT_NAME} SHARED "Main.cpp" ${SOURCES} ${HEADERS})
else()
    add_executable(${PROJECT_NAME} "Main.cpp" ${SOURCES} ${HEADERS})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/headers"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GuiLib"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Utils"
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    UtilsLib 
    GuiLib
    nlohmann_json::nlohmann_json
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

# Для Android добавляем системные библиотеки
if(ANDROID)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        android
        log
    )
endif()

# Копирование ассетов только для настольных платформ
# ВАЖНО: На Android ассеты должны быть в папке app/src/main/assets/
# и автоматически распаковываются SDL3 в internal storage.
# Путь будет доступен через SDL_GetBasePath()
if(NOT ANDROID)
    set(SOURCE_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/content")
    set(DEST_ASSETS_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SOURCE_ASSETS_DIR}
            ${DEST_ASSETS_DIR}
        COMMENT "Copying assets to build directory..."
    )
endif()

