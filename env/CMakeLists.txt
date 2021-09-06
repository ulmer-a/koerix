cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_FLAGS "-ffreestanding -nostdinc -O3 -fno-pic -fno-stack-protector")
set(BIN_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

add_custom_target(env)

macro(user_prog id sources)
    add_library(env_${id}_lib STATIC
        ${sources}
    )
    target_include_directories(env_${id}_lib PRIVATE ${LIBKOERIX_INCLUDE})

    add_custom_target(env_${id}
    COMMAND mkdir -p ${BIN_PATH}
    COMMAND "/usr/bin/ld.lld" -static --gc-sections
        -T ${CMAKE_CURRENT_SOURCE_DIR}/../../platform/${TARGET_PLATFORM}/skel/usr/lib/${TARGET_ARCH}.ld
        $<TARGET_FILE:env_${id}_lib> $<TARGET_FILE:koerix> -o ${BIN_PATH}/${id}
    DEPENDS koerix env_${id}_lib
    )
    add_dependencies(env env_${id})
endmacro()

# add your programs here
add_subdirectory(hello_world)
