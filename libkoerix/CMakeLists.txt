cmake_minimum_required(VERSION 3.14)

set(LIBKOERIX_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB LIBKOERIX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
add_library(koerix STATIC
    ${LIBKOERIX_SRC}
    ${LIBKOERIX_INCLUDE}
)

target_include_directories(koerix PRIVATE
    ${LIBKOERIX_INCLUDE}
    ${LIBC_INCLUDE}
)
