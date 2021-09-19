# Koerix

## What is Koerix?
Koerix is a Unix-like hobby OS written in C++. I am mainly targeting the
`x86_64` (on PC) and the `armv6a` and `AArch64` (on raspi/raspi4) architectures.
This repo is the patched-together result of ~4 years of on/off work on my
OS projects (turns out OS development is real hard, so I started over again
a few times). It's not the most stable, yet, (especially on real hardware) but I'm
getting there.

Version 1.0 will be shipped as soon as bash and busybox are up and running :)

![Screenshot](screenshot.png)

In the image, you can see the userspace program `textcon` in action. the terminal
to the left shows the serial output. `textcon` will print everything on the screen
it reads from it's stdin (in this case, the serial console). 

## Licensing
While the kernel (directories `arch/`, `platform/`, `kernel/`) and userspace environment
programs (`env/`) is licensed under the terms of the GNU Lesser General Public License,
the directory `libc/` contains large parts of the *Musl* libc, which is provided under
the terms of the MIT license. Additionally, all files should have a SPDX comment at the top
stating the exact license and copyright holder. If they don't: please tell me or open
a pull request.

## Roadmap
Currently, my goal is to make the codebase more stable before implementing lots of
new features, because it still doesn't run stable on my real-hardware testbeds. Things
I want to do/implement in the near future:
* Get it to run on my test hardware (some x86\_64 PCs)
* Add APIC and SMP support (supported by limine bootloader): I want to add it as soon
as possible in order to be able to detect synchronization issues early.
* Writing a driver for SATA drives and implementing ext2 filesystem support: pretty much
a must-have for any serious OS. After that, I can start thinking about porting some
actually useful software, like bash.
* raspi1 port (BCM2835/armv6), since I have some old 32bit pis that are too slow
for anything else and I think that's a nice re-use application.

Recently, I completely dropped support for 32bit x86 (i386 architecture), since I think
supporting it would be too much added complexity that I don't want (cannot map the whole
physical RAM in the upper half, jumping to higher half requires some extra magic), given
that I don't even own corresponding hardware anymore. However, I will eventually support
executing 32bit code in userspace (compatibility mode).

## Current Kernel Features
* x86\_64 kernel (64bit) written in C++, C as well as some GNU Assembler
* preemptive multitasking (single core), Mutexes and Spinlocks
* full paged Virtual Memory support
* POSIX `fork()` with multi-level copy-on-write
* `read()`, `write()`, `open()`, `close()` via Virtual file system (WIP)
* system calls via software interrupt 0x80
* `limine` bootloader
* POSIX pipes (create by calling pipe `pipe()`, use by `fork()`ing)
* Processes and Threads (but no `exec()` so far)
* OS support for FPU/SSE

## Drivers
* PC Serial port driver (for getting a shell and some debug output)
* Rudimentary support for PS/2 keyboard

## Userspace features
* Standard C-library (by copying and modifying sources from musl libc as needed)
* Access to full resolution framebuffer, font rendering and draw functions via
`libc` in userspace.
* Create userspace threads with functions from `koerix/threads.h`

## Available Software
* `textcon`: A userspace program that prints everything it reads from stdin to the
framebuffer providing a text console. It also opens the keyboard device and prints
everything it reads from there to stdout.

## Runtime configuration
The kernel can be configured at boot time (via the kernel command line) 
to do a few things. For example:
* `console=/dev/uart0` will open a serial console on `uart0` (first serial port)
* `init=/bin/init` will load the specified binary from disk and run it after booting
* `init=@some_mod` will load the specified module passed via the bootloader and run it after booting

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
