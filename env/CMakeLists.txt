cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_FLAGS "-ffreestanding -nostdinc -O0 -g -fno-pic -fno-stack-protector")
set(BIN_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

add_custom_target(env)

function(user_prog id sources)
    add_library(env_${id}_lib STATIC
        ${sources}
    )
    target_include_directories(env_${id}_lib PRIVATE ${LIBKOERIX_INCLUDE})

    add_custom_target(env_${id}
    COMMAND mkdir -p ${BIN_PATH}
    COMMAND "/usr/bin/ld.lld" -static --gc-sections
        -T ${CMAKE_CURRENT_SOURCE_DIR}/../platform/${TARGET_PLATFORM}/skel/usr/lib/${TARGET_ARCH}.ld
        $<TARGET_FILE:env_${id}_lib> $<TARGET_FILE:koerix> -o ${BIN_PATH}/${id}
    DEPENDS koerix env_${id}_lib
    )
    add_dependencies(env env_${id})
endfunction()

# add your programs here
user_prog(textcon ${CMAKE_CURRENT_SOURCE_DIR}/textcon.c)
user_prog(thread_test ${CMAKE_CURRENT_SOURCE_DIR}/thread_test.c)
user_prog(fork_test ${CMAKE_CURRENT_SOURCE_DIR}/fork_test.c)
user_prog(htop ${CMAKE_CURRENT_SOURCE_DIR}/htop.c)
