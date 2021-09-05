cmake_minimum_required(VERSION 3.14)

set(LIBKOERIX_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${TARGET_ARCH}.cmake)
set(CMAKE_C_FLAGS "--target=${TARGET_ARCH}-none-eabi -O3 \
    -ffreestanding -nostdlib -nostdinc ${ARCH_USER_FLAGS}")

file(GLOB LIBKOERIX_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.S
)
add_library(koerix STATIC
    ${LIBKOERIX_SRC}
)

target_include_directories(koerix PRIVATE
    ${LIBKOERIX_INCLUDE}
)
