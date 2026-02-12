
if(NOT "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/e3x4m392/arm64-v8a/_deps/sdl3-subbuild/sdl3-populate-prefix/src/sdl3-populate-stamp/sdl3-populate-gitinfo.txt" IS_NEWER_THAN "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/e3x4m392/arm64-v8a/_deps/sdl3-subbuild/sdl3-populate-prefix/src/sdl3-populate-stamp/sdl3-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/e3x4m392/arm64-v8a/_deps/sdl3-subbuild/sdl3-populate-prefix/src/sdl3-populate-stamp/sdl3-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL3"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL3'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout --config "advice.detachedHead=false" "https://github.com/libsdl-org/SDL.git" "SDL3"
    WORKING_DIRECTORY "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/libsdl-org/SDL.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout main --
  WORKING_DIRECTORY "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL3"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'main'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL3"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/media/vodapc/Hdd/projects/MetroidGame/Game2d/dependencies/git/SDL3'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/e3x4m392/arm64-v8a/_deps/sdl3-subbuild/sdl3-populate-prefix/src/sdl3-populate-stamp/sdl3-populate-gitinfo.txt"
    "/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/e3x4m392/arm64-v8a/_deps/sdl3-subbuild/sdl3-populate-prefix/src/sdl3-populate-stamp/sdl3-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/media/vodapc/Hdd/projects/MetroidGame/Game2d/app/.cxx/Debug/e3x4m392/arm64-v8a/_deps/sdl3-subbuild/sdl3-populate-prefix/src/sdl3-populate-stamp/sdl3-populate-gitclone-lastrun.txt'")
endif()

