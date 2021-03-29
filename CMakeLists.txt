SET(CMAKE_SYSTEM_NAME Linux)
cmake_minimum_required(VERSION 3.7)

# specify cross compilers and tools
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_ASM_COMPILER  arm-linux-gnueabihf-gcc)
set(CMAKE_AR arm-linux-gnueabihf-ar)
set(CMAKE_OBJCOPY arm-linux-gnueabihf-objcopy)
set(CMAKE_OBJDUMP arm-linux-gnueabihf-objdump)
set(SIZE arm-linux-gnueabihf-size)

project(arfid_reader)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

include_directories("../liba7139/inc")
set(LIB_A7139_FILE "../liba7139/src/hal.c" "../liba7139/src/mac.c")
ADD_LIBRARY(a7139 STATIC ${LIB_A7139_FILE})

file(GLOB SRC_FILE "./main/*.c")

add_executable(${PROJECT_NAME} ${SRC_FILE} ${LIB_A7139_FILE})
