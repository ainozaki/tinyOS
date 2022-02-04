#include "gui.h"

#include "efi.h"
#include "graphics.h"

void gui() {
    ST->ConOut->ClearScreen(ST->ConOut);

    struct RECT r = {10, 10, 100, 200};
    draw_rect(r, white);
    while (1)
        ;
}