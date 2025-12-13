#include "StartScreen.h"

#include <drivers/Clavier.h>
#include <drivers/EcranBochs.h>
#include <sextant/sprite.h>
#include <Applications/Draw/Draw.h>

static bool any_key_pressed(Clavier &c)
{
    for (int i = 0; i < 256; i++)
        if (c.is_pressed(i))
            return true;
    return false;
}

static void wait_key_release(Clavier &c)
{
    while (any_key_pressed(c))
        asm volatile("hlt");
}

static inline void sleep_ticks(int n)
{
    for (int i = 0; i < n; ++i)
        asm volatile("hlt");
}

void start_screen()
{
    ui16_t WIDTH = 640, HEIGHT = 400;
    EcranBochs vga(WIDTH, HEIGHT, VBE_MODE::_8);
    Clavier c;

    vga.init();
    vga.clear(0);
    vga.set_palette(palette_vga);

    wait_key_release(c);

    bool blink = false;

    while (true)
    {
        vga.clear(0);

        // title Dungeon Explorer
        draw_text(vga, "DUNGEON", 152, 80, 3, 15);  // 640/2 - (7*16)*3/2 = 152
        draw_text(vga, "EXPLORER", 128, 136, 3, 15);  // 640/2 - (8*16)*3/2 = 128

        /* ===== Press Any Key (Blink) ===== */
        if (blink)
        {
            draw_text(vga, "PRESS ANY KEY", 216, 220, 1, 50);  // 640/2 - (13*16)*1/2 = 216
        }

        vga.swapBuffer();

        if (any_key_pressed(c))
            break;

        blink = !blink;
        sleep_ticks(30);
    }

    vga.clear(0);
    vga.swapBuffer();
}
