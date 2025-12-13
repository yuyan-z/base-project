#include "StartScreen.h"

#include <drivers/EcranBochs.h>
#include <drivers/Clavier.h>
#include <sextant/sprite.h>
#include <sextant/types.h>

/* ---------- Utils ---------- */

static bool any_key_pressed(Clavier& c) {
    for (int i = 0; i < 256; i++)
        if (c.is_pressed(i))
            return true;
    return false;
}

static void wait_key_release(Clavier& c) {
    while (any_key_pressed(c))
        asm volatile("hlt");
}

static inline void sleep_ticks(int n) {
    for (int i = 0; i < n; ++i)
        asm volatile("hlt");
}

static void draw_block(
    EcranBochs& vga,
    int x0, int y0,
    int w, int h,
    ui8_t color
) {
    for (int y = y0; y < y0 + h; y++)
        for (int x = x0; x < x0 + w; x++)
            vga.paint(x, y, color);
}

/* ---------- Start Screen ---------- */

void start_screen() {
	ui16_t WIDTH = 640, HEIGHT = 400;
    EcranBochs vga(WIDTH, HEIGHT, VBE_MODE::_8);
    Clavier c;

    vga.init();
    vga.clear(0);

    vga.set_palette(palette_vga);

    wait_key_release(c);

    bool blink = false;

    while (true) {
        vga.clear(0);

        /* ===== DungeonExplorer (Title) ===== */
        draw_block(vga, 120, 90, 400, 50, 30);

        /* ===== Press Any Key (Blink) ===== */
        if (blink) {
            draw_block(vga, 200, 240, 240, 30, 45);
        }

        vga.swapBuffer();

        if (any_key_pressed(c))
            break;

        blink = !blink;
        sleep_ticks(30);
    }

    /* ===== Exit start screen ===== */
    vga.clear(0);
    vga.swapBuffer();
}
