cmake_minimum_required(VERSION 3.16)
project(jnp1_zad3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(jnp1_zad3
        fuzzy.cc
        fuzzy.h
        fuzzy_example.cc
        main.cpp
        rational.cc)
