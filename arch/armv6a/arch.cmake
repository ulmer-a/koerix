cmake_minimum_required(VERSION 3.14)

# probably also give it -mcpu=arm1176jzf for raspi1
set(ARCH_FLAGS "-march=armv6k")

add_definitions(-Darmv6a -DARCH=armv6a -DBITS32)
