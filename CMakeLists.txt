cmake_minimum_required(VERSION 3.16)

project(SDLCppWrapper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.x
)

FetchContent_MakeAvailable(SDL3)

#add_library(SDLCppWrapper 
#    "src/SDLCppWrapper.cpp")

# Swap comment with add_library to run the example project
add_executable(SDLCppWrapper
    "src/SDLCppWrapper.cpp" 
    "examples/main.cpp"
)

target_include_directories(SDLCppWrapper
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(SDLCppWrapper PUBLIC SDL3::SDL3)