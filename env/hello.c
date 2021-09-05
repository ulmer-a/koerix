
/* 
   stdlib / print not yet available 
   program is started by kernel in user mode
   env$ gcc -ffreestanding -nostdlib -Wl,-T ../platform/pc/skel/usr/lib/x86_64.ld hello.c -o hello_world
   env$ mkdir ../skel/bin
   cp hello_world ../skel/bin/hello_world
*/

void exit(int status) {
    __asm__ volatile (
        "mov $0, %%rax;" // system call exit
        "mov %0, %%rbx;" // parameter status
        "int $0x80;"     // do system call
        :                // no output parameters
        : "g"(status)    // input parameter status
    );
}

void _start()
{
    exit(0xbadbeef); /* system call to kernel to avoid crash */
}