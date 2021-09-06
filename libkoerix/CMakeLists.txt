cmake_minimum_required(VERSION 3.14)

set(LIBKOERIX_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_ARCH}/include
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${TARGET_ARCH}.cmake)
set(CMAKE_C_FLAGS "--target=${TARGET_ARCH}-none-eabi -O3 \
    -ffreestanding -nostdlib -nostdinc ${ARCH_USER_FLAGS}\
    -Wno-shift-op-parentheses -fPIC -ffunction-sections ")

file(GLOB_RECURSE LIBKOERIX_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/generic/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/generic/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generic/*.S
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_ARCH}/*.S
    ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_ARCH}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_ARCH}/*.h
)
add_library(koerix STATIC
    ${LIBKOERIX_SRC}
)

target_include_directories(koerix PRIVATE
    ${LIBKOERIX_INCLUDE}
)
set(LIBKOERIX_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_ARCH}/include
PARENT_SCOPE)
