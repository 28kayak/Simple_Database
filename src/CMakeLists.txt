cmake_minimum_required(VERSION 3.17)
project(simple_batabase C)

set(CMAKE_C_STANDARD 99)

add_executable(simple_batabase main.c results.h statement.h input_buffer.h)