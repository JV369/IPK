cmake_minimum_required(VERSION 3.6)
project(proj2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES meter.c meter.h ipk-mtrip.c reflect.c reflect.h queue.c queue.h)
add_executable(proj2 ${SOURCE_FILES} meter.c meter.h ipk-mtrip.c reflect.c reflect.h queue.c queue.h)