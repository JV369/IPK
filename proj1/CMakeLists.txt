cmake_minimum_required(VERSION 3.6)
project(proj1)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES client.c server.c)
add_executable(proj1 ${SOURCE_FILES} client.c server.c)