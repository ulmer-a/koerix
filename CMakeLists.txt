cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(UlmerOS VERSION 0.1)
enable_language(ASM)

set(TARGET_ARCH "x86_64")
set(TARGET_PLATFORM "pc")
set(ARCH_FLAGS -mno-red-zone)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

message(STATUS "UlmerOS v${CMAKE_PROJECT_VERSION}")
message(STATUS "Building for ${TARGET_ARCH}-${TARGET_PLATFORM}")

set(CMAKE_C_FLAGS "--target=${TARGET_ARCH}-none-eabi \
    -ffreestanding -fno-rtti -fno-exceptions -fPIC \
    -nostdlib -nostdinc -fno-stack-protector \
    ${ARCH_FLAGS}"
)
set(CMAKE_CXX_FLAGS "--target=${TARGET_ARCH}-none-eabi \
    -ffreestanding -fno-rtti -fno-exceptions -fPIC \
    -nostdlib -nostdinc -fno-stack-protector \
    ${ARCH_FLAGS}"
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/baselib/include
)

file(GLOB_RECURSE KERNEL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.cpp
)
add_library(kernel STATIC ${KERNEL_SOURCES})

file(GLOB_RECURSE BASELIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/baselib/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/baselib/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/baselib/*.cpp
)
add_library(baselib STATIC ${BASELIB_SOURCES})

file(GLOB_RECURSE ARCH_SOURCES FOLLOW_SYMLINKS
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/*.S
)
add_library(arch STATIC ${ARCH_SOURCES})

file(GLOB_RECURSE PLATFORM_SOURCES FOLLOW_SYMLINKS
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/drivers/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/drivers/*.cpp
)
add_library(platform STATIC ${PLATFORM_SOURCES})

add_custom_target(vmkoerix ALL
    COMMAND "/usr/bin/ld.lld"
        -T ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/${TARGET_ARCH}.ld
        $<TARGET_FILE:baselib> $<TARGET_FILE:kernel> $<TARGET_FILE:arch> $<TARGET_FILE:platform>
        -o ${CMAKE_CURRENT_BINARY_DIR}/vmkoerix
    DEPENDS kernel arch platform baselib
)

add_custom_target(disk
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/platform/pc/build_diskimage
        ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}/disk.img
    BYPRODUCTS ${CMAKE_BINARY_DIR}/disk.img
    DEPENDS vmkoerix
)

add_custom_target(qemu
    COMMAND qemu-system-x86_64 -serial stdio -drive format=raw,file=${CMAKE_BINARY_DIR}/disk.img -m 256 -s
)
