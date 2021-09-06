cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_FLAGS "-ffreestanding -nostdinc -O3")

macro(user_prog id sources)
    add_library(env_${id}_lib STATIC
        ${sources}
    )
    target_include_directories(env_${id}_lib PRIVATE ${LIBKOERIX_INCLUDE})

    add_custom_target(env_${id}
    COMMAND "/usr/bin/ld.lld"
        -T ${CMAKE_CURRENT_SOURCE_DIR}/../../platform/${TARGET_PLATFORM}/skel/usr/lib/${TARGET_ARCH}.ld
        $<TARGET_FILE:env_${id}_lib> $<TARGET_FILE:koerix> -o ${CMAKE_CURRENT_BINARY_DIR}/${id}
        --gc-sections
    DEPENDS koerix env_${id}_lib
    )
    set(ENV_BINARIES ${ENV_BINARIES} ${CMAKE_CURRENT_BINARY_DIR}/${id} PARENT_SCOPE)
endmacro()

# add your programs here
add_subdirectory(hello_world)