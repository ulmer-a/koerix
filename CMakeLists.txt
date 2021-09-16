cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(UlmerOS VERSION 0.1)
enable_language(ASM)

if("${TARGET_ARCH}" STREQUAL "")
    set(TARGET_ARCH "x86_64")
endif()
if("${TARGET_PLATFORM}" STREQUAL "")
    set(TARGET_PLATFORM "pc")
endif()

add_compile_options("--target=${TARGET_ARCH}-none-eabi")

include(${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/arch.cmake)

set(CMAKE_BUILD_TYPE Debug)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

message(STATUS "UlmerOS v${CMAKE_PROJECT_VERSION}")
message(STATUS "Building for ${TARGET_ARCH}-${TARGET_PLATFORM}")

######################################################################
# BUILD LIBC, USERSPACE                                              #
######################################################################
add_subdirectory(libc)
add_subdirectory(env)

# todo userspace

######################################################################
# BUILD KERNEL                                                       #
######################################################################

set(CMAKE_C_FLAGS "\
    -ffreestanding -fno-rtti -fno-exceptions -fPIC \
    -nostdlib -nostdinc -fno-stack-protector \
    ${ARCH_FLAGS}"
)
set(CMAKE_CXX_FLAGS "\
    -ffreestanding -fno-rtti -fno-exceptions -fPIC \
    -nostdlib -nostdinc -fno-stack-protector \
    ${ARCH_FLAGS}"
)

set(KERNEL_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/${TARGET_ARCH}/include
)

file(GLOB_RECURSE KERNEL_SOURCES FOLLOW_SYMLINKS
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/*.S
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/drivers/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/drivers/*.cpp
)
add_library(kernel STATIC ${KERNEL_SOURCES})
target_include_directories(kernel PRIVATE ${KERNEL_INCLUDE})

add_custom_target(vmkoerix ALL
    COMMAND "/usr/bin/ld.lld" -nostdlib -static
        -T ${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/${TARGET_ARCH}.ld
        $<TARGET_FILE:kernel> -o ${CMAKE_CURRENT_BINARY_DIR}/vmkoerix
    DEPENDS kernel
)

######################################################################
# BUILD DISK IMAGE, QEMU                                             #
#####################################################################

include(${CMAKE_CURRENT_SOURCE_DIR}/platform/${TARGET_PLATFORM}/platform.cmake)
