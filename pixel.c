#include "pixel.h"

struct framebuffer *fb;

void fb_init(struct framebuffer *_fb) {
    fb->base = _fb->base;
    fb->size = _fb->size;
    fb->hr = _fb->hr;
    fb->vr = _fb->vr;
}

void set_screen(unsigned char red, unsigned char green, unsigned char blue) {
    unsigned int x, y;
    struct pixelformat *p;
    for (y = 0; y < fb->vr; y++) {
        for (x = 0; x < fb->hr; x++) {
            p = fb->base + (fb->hr * y) + x;
            p->r = red;
            p->g = green;
            p->b = blue;
        }
    }
}
