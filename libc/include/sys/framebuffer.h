#pragma once

#include <sys/types.h>

typedef struct
{
    unsigned int width;
    unsigned int height;
} fbinfo_t;

/* fb_info() will return information on framebuffers
 * that are present on the system. */
int fb_info(size_t id, fbinfo_t* fbinfo);

/* getfb() will return a pointer to the framebuffer
 * specified by id. */
void* getfb(size_t id);
