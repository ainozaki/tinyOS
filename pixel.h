#ifndef PIXEL_H_
#define PIXEL_H_

extern struct framebuffer *fb;

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

// Paint one pixel.
void draw_px(unsigned int x,
             unsigned int y,
             unsigned char red,
             unsigned char green,
             unsigned char blue);

// Paint whole screen.
void set_screen(unsigned char red, unsigned char green, unsigned char blue);
void set_default_screen();

#endif// PIXEL_H_
