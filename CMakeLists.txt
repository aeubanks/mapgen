cmake_minimum_required(VERSION 2.8.4)

set(MACOSX_BUNDLE TRUE)

set(PROJECT_NAME mapgen)
project(${PROJECT_NAME})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (UNIX)
    message(STATUS "UNIX")
endif ()
if (WIN32)
    message(STATUS "WIN32")
endif ()
if (APPLE)
    message(STATUS "APPLE")
endif ()
if (MINGW)
    message(STATUS "MINGW")
endif ()
if (MSYS)
    message(STATUS "MSYS")
endif ()
if (CYGWIN)
    message(STATUS "CYGWIN")
endif ()

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

set(BOX2D_BUILD_STATIC 1)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/Box2D/)

set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)

file(GLOB SRC_FILES src/mapgen/*.cpp src/fmt/*.cpp src/json/*.cpp src/mg_util/*.cpp src/lodepng/*.cpp src/sdl2/*.cpp src/game/*.cpp src/main.cpp)

set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)

set(INCLUDE_DIRS src/ ${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} lib/Box2D/)
include_directories(${INCLUDE_DIRS})

set(LINK_LIBS ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
target_link_libraries(${PROJECT_NAME} Box2D)

if (APPLE)
    find_library(CORE_FOUNDATION CoreFoundation)
    if (NOT CORE_FOUNDATION)
        message(FATAL_ERROR "couldn't find CoreFoundation")
    endif ()
    target_link_libraries(${PROJECT_NAME} ${CORE_FOUNDATION})
endif ()

if (WIN32 OR MINGW)
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -lmingw32 -mwindows")
endif ()

if (MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lmingw32")
endif ()

if(APPLE)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic") #-fsanitize=memory -fsanitize=address -fsanitize=undefined
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0")

