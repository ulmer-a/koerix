cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(UlmerOS VERSION 0.1)

set(TARGET_ARCH "x86_64")
set(TARGET_PLATFORM "pc")
set(ARCH_FLAGS -mno-red-zone)

message(STATUS "UlmerOS v${CMAKE_PROJECT_VERSION}")
message(STATUS "Building for ${TARGET_ARCH}-${TARGET_PLATFORM}")

set(CMAKE_C_FLAGS "--target=${TARGET_ARCH}-none-eabi \
    -ffreestanding -fno-rtti -fno-exceptions \
    -nostdlib -nostdinc -fno-stack-protector \
    ${ARCH_FLAGS}"
)
set(CMAKE_CXX_FLAGS "--target=${TARGET_ARCH}-none-eabi \
    -ffreestanding -fno-rtti -fno-exceptions \
    -nostdlib -nostdinc -fno-stack-protector \
    ${ARCH_FLAGS}"
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/include
)

file(GLOB_RECURSE KERNEL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.cpp
)
add_library(kernel STATIC ${KERNEL_SOURCES})

file(GLOB_RECURSE ARCH_SOURCES FOLLOW_SYMLINKS
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/*.cpp
)
add_library(arch STATIC ${ARCH_SOURCES})

file(GLOB_RECURSE PLATFORM_SOURCES FOLLOW_SYMLINKS
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/*.cpp
)
add_library(platform STATIC ${PLATFORM_SOURCES})

add_custom_target(kernel.elf ALL
    COMMAND "/usr/bin/ld.lld"
        -T ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/${TARGET_ARCH}.ld
        $<TARGET_FILE:kernel> $<TARGET_FILE:arch> $<TARGET_FILE:platform>
        -o ${CMAKE_CURRENT_BINARY_DIR}/kernel.elf
    DEPENDS kernel arch platform
)

add_custom_target(rootfs ALL
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/sysroot
    COMMAND cp $<TARGET_FILE:kernel> ${CMAKE_BINARY_DIR}/sysroot/kernel.elf
    DEPENDS kernel
)

if(NOT DEFINED LIMINE_PATH)
    message(FATAL_ERROR "LIMINE_PATH is not set!")
endif()

add_custom_target(diskimage ALL
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/boot/mkimg
        ${CMAKE_BINARY_DIR}/sysroot
        ${CMAKE_CURRENT_SOURCE_DIR}/boot
        ${LIMINE_PATH}
    DEPENDS rootfs
)

add_custom_target(qemu
    COMMAND qemu-system-x86_64 -debugcon stdio -drive format=raw,file=${CMAKE_BINARY_DIR}/disk.img -m 512
)