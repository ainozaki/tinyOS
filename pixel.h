#ifndef PIXEL_H_
#define PIXEL_H_

struct pixelformat {
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char _reserved;
};

struct framebuffer {
    struct pixelformat *base;// top address of framebuffer
    unsigned long long size;
    unsigned int hr;// horizontal resolution
    unsigned int vr;// vertical resolution
};

// Register framebuffer pointer.
void fb_init(struct framebuffer *fb);

// Paint out screen.
void set_screen(unsigned char red, unsigned char green, unsigned char blue);

#endif// PIXEL_H_
