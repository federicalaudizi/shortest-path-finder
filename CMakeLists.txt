cmake_minimum_required(VERSION 3.22)
project(shortest_path_finder)

set(CMAKE_C_STANDARD 11)

add_executable(shortest_path_finder shortest_path_finder.C)

add_compile_options(-Wall -Werror -std=gnu11 -O2 -pipe -static -s -o -lm)