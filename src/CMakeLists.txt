cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(controller_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(controller
  main.cpp
)
pico_enable_stdio_usb(controller 1)

pico_add_extra_outputs(controller)
target_link_libraries(controller pico_stdlib)
