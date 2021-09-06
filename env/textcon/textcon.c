#include <stdio.h>
#include <koerix/framebuffer.h>

int main() {
    printf("hello, world!\n");

    // initialize framebuffer
    if (fb_init() < 0) {
      fprintf(stderr, "error: framebuffer not supported\n");
      return 1;
    }

    printf("framebuffer: %zu x %zu, pitch=%zu\n",
           FB_WIDTH, FB_HEIGHT, FB_PITCH);

    return 0;
}
