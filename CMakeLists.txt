cmake_minimum_required(VERSION 3.0)

project(flappy-bird)

add_executable(flappy-bird main.cpp)

install(TARGETS flappy-bird RUNTIME DESTINATION bin)
