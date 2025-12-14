#include "StartScreen.h"

#include <drivers/Clavier.h>
#include <drivers/EcranBochs.h>
#include <sextant/sprite.h>
#include <Applications/Draw/Draw.h>
#include <Applications/Draw/sprite_tile.h>

static bool any_key_pressed(Clavier &c)
{
    for (int i = 0; i < 256; i++)
        if (c.is_pressed(i))
            return true;
    return false;
}

// static inline void sleep_ticks(int n)
// {
//     for (int i = 0; i < n; ++i)
//         asm volatile("hlt");
// }

static inline void sleep_ticks(int n)
{
    for (volatile int i = 0; i < n * 100000; ++i) { }
}

void start_screen()
{
    ui16_t WIDTH = 640, HEIGHT = 400;
    EcranBochs vga(WIDTH, HEIGHT, VBE_MODE::_8);
    Clavier c;

    vga.init();
    vga.clear(0);
    vga.set_palette(palette_vga);

    bool blink = false;

    while (true)
    {
        vga.clear(0);

        // bool pressed = any_key_pressed(c);
        // draw_text(vga, pressed ? "P" : "N", 10, 10, 1, 15);

        // title Dungeon Explorer
        draw_text(vga, "DUNGEON", 152, 60, 3, 15);  // 640/2 - (7*16)*3/2 = 152
        draw_text(vga, "EXPLORER", 128, 126, 3, 15);  // 640/2 - (8*16)*3/2 = 128

        // Press Any Key
        if (blink)
        {
            draw_text(vga, "PRESS ANY KEY", 216, 200, 1, 50);  // 640/2 - (13*16)*1/2 = 216
        }

        draw_tile(vga, sprite_wall, 0, 340, 640, 60, SPRITE_WALL_W, SPRITE_WALL_H, 5);

        draw_sprite_scaled(vga, sprite_data, SPRITE_WIDTH, SPRITE_HEIGHT, 210, 250, 3);
        draw_sprite_scaled(vga, sprite_data, SPRITE_WIDTH, SPRITE_HEIGHT, 160, 250, 3);
        draw_sprite_scaled(vga, sprite_data, SPRITE_WIDTH, SPRITE_HEIGHT, 320, 250, 3, 15, true);

        vga.swapBuffer();

        if (any_key_pressed(c))
            break;

        blink = !blink;
        sleep_ticks(30);
    }

    vga.clear(0);
    vga.swapBuffer();
}
