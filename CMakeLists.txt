cmake_minimum_required(VERSION 3.13)
set(EXECUTABLE_NAME "SFML_particleSystem")
project(${EXECUTABLE_NAME})


set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17 -fopenmp)


add_executable(${EXECUTABLE_NAME} main.cpp Particle.cpp Particle.h StatDisplay/FPS.cpp StatDisplay/FPS.h StatDisplay/StatDisplay.cpp StatDisplay/StatDisplay.h StatDisplay/IDisplayable.h Quadtree.h Quadtree.cpp)

FIND_PACKAGE(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O0")

find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} sfml-graphics sfml-window sfml-system pthread)
endif()