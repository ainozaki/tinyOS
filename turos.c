#include "kbc.h"
#include "pixel.h"
#include "print.h"

// @_t: SystemTable address
void start_kernel(void *_t __attribute__((unused)),
                  struct framebuffer *fb,
                  void *_fs_start __attribute__((unused))) {
  fb_init(fb);

  // paint screen
  set_screen(64, 224, 208);

  // display character
  puts("HELLO TUROS");

  while (1)
    ;
}
