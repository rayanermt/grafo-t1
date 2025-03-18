cmake_minimum_required(VERSION 3.27)
project(grafos C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(grafos
        grafo.c
        grafo.h
        grafoTeste.c)
