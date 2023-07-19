cmake_minimum_required(VERSION 3.22)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled cercaPercorso.c)

add_compile_options(-Wall -Werror -std=gnu11 -O2 -pipe -static -s -o -lm)