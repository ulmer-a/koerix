cmake_minimum_required(VERSION 3.14)

add_custom_target(qemu
    COMMAND qemu-system-arm -S -M raspi2 -serial stdio -kernel ${CMAKE_BINARY_DIR}/vmkoerix -s
)
