#include "pixel.h"

// @_t: SystemTable address
void start_kernel(void *_t __attribute__((unused)), struct framebuffer *fb,
                  void *_fs_start __attribute__((unused))) {
    fb_init(fb);
    set_screen(64, 224, 208);

    while (1)
        ;
}