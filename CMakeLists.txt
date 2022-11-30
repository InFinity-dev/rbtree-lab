cmake_minimum_required(VERSION 3.23)
project(rbtree_lab C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(rbtree_lab
        src/driver.c
        src/rbtree.c
        src/rbtree.h
        test/test-rbtree.c)
