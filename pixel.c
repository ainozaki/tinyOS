#include "pixel.h"

struct framebuffer *fb;

void fb_init(struct framebuffer *_fb) {
    fb->base = _fb->base;
    fb->size = _fb->size;
    fb->hr = _fb->hr;
    fb->vr = _fb->vr;
}

void draw_px(unsigned int x,
             unsigned int y,
             unsigned char red,
             unsigned char green,
             unsigned char blue) {
    struct pixelformat *p = fb->base;
    p += y * fb->hr + x;
    p->r = red;
    p->g = green;
    p->b = blue;
}

void set_screen(unsigned char red, unsigned char green, unsigned char blue) {
    unsigned int x, y;
    for (y = 0; y < fb->vr; y++) {
        for (x = 0; x < fb->hr; x++) {
            draw_px(x, y, red, green, blue);
        }
    }
}

void set_default_screen() {
    set_screen(64, 224, 208);
}
