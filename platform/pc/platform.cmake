
add_custom_target(disk
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/platform/pc/build_diskimage
        ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}/disk.img
    BYPRODUCTS ${CMAKE_BINARY_DIR}/disk.img
    DEPENDS vmkoerix env
)

add_custom_target(qemu
    COMMAND qemu-system-${TARGET_ARCH} -serial stdio -drive format=raw,file=${CMAKE_BINARY_DIR}/disk.img -m 256 -s
)
