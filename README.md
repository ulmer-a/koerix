# Koerix

## What is Koerix?
Koerix is a Unix-like hobby OS written in C++. I am mainly targeting the
`x86_64` (on PC) and `AArch64` (on raspi4) architectures, but I'm also
interested in getting the OS to work on `i386` and `arm` 32bit architectures.

## Building
Building the kernel and userspace binaries:
```
$ git clone --recursive https://github.com/ulmer-a/koerix
$ mkdir -p koerix/build && cd koerix/build
$ cmake ..
$ make -j
```

Assembling a diskimage:
```
$ make disk    # this will build the limine bootloader
               # from source and generate a disk image
$ make qemu    # this will start your diskimage in QEMU
```
