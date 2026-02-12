# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf")
  file(MAKE_DIRECTORY "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL_ttf")
endif()
file(MAKE_DIRECTORY
  "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/30196d2b/arm64-v8a/_deps/sdl3_ttf-build"
  "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/30196d2b/arm64-v8a/_deps/sdl3_ttf-subbuild/sdl3_ttf-populate-prefix"
  "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/30196d2b/arm64-v8a/_deps/sdl3_ttf-subbuild/sdl3_ttf-populate-prefix/tmp"
  "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/30196d2b/arm64-v8a/_deps/sdl3_ttf-subbuild/sdl3_ttf-populate-prefix/src/sdl3_ttf-populate-stamp"
  "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/30196d2b/arm64-v8a/_deps/sdl3_ttf-subbuild/sdl3_ttf-populate-prefix/src"
  "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/30196d2b/arm64-v8a/_deps/sdl3_ttf-subbuild/sdl3_ttf-populate-prefix/src/sdl3_ttf-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/30196d2b/arm64-v8a/_deps/sdl3_ttf-subbuild/sdl3_ttf-populate-prefix/src/sdl3_ttf-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/30196d2b/arm64-v8a/_deps/sdl3_ttf-subbuild/sdl3_ttf-populate-prefix/src/sdl3_ttf-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
