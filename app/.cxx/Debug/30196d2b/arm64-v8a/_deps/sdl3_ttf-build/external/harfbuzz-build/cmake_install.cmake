# Install script for directory: /media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/vodapc/Android/Sdk/ndk/28.2.13676358/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/harfbuzz" TYPE FILE FILES
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-aat-layout.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-aat.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-blob.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-buffer.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-common.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-cplusplus.hh"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-deprecated.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-draw.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-face.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-font.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-map.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-ot-color.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-ot-deprecated.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-ot-font.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-ot-layout.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-ot-math.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-ot-meta.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-ot-metrics.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-ot-name.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-ot-shape.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-ot-var.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-ot.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-paint.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-set.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-shape-plan.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-shape.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-style.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-unicode.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-version.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb.h"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf/external/harfbuzz/src/hb-ft.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/30196d2b/arm64-v8a/_deps/sdl3_ttf-build/external/harfbuzz-build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
