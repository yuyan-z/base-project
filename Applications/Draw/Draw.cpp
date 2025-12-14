#include "Draw.h"

#include <drivers/EcranBochs.h>
#include <Applications/Draw/sprite_char.h>

void draw_rect(
    EcranBochs &vga,
    int x0, int y0,
    int w, int h,
    ui8_t color)
{
    for (int y = y0; y < y0 + h; y++)
        for (int x = x0; x < x0 + w; x++)
            vga.paint(x, y, color);
}

void draw_sprite_scaled(
    EcranBochs &vga,
    const unsigned char *sprite,
    int sprite_w,
    int sprite_h,
    int x0,
    int y0,
    int scale,
    ui8_t light,
    bool mirror_x
)
{
    int screen_w = vga.getWidth();
    int screen_h = vga.getHeight();

    for (int y = 0; y < sprite_h; y++)
    {
        for (int x = 0; x < sprite_w; x++)
        {
            int sx = mirror_x ? (sprite_w - 1 - x) : x;
            ui8_t color = sprite[y * sprite_w + sx];

            if (color == 0)
                continue; // skip transparent pixels

            if (light != 15)
                color = 15 - color + light;
  
            for (int dy = 0; dy < scale; dy++)
            {
                for (int dx = 0; dx < scale; dx++)
                {
                    int px = x0 + x * scale + dx;
                    int py = y0 + y * scale + dy;

                    if (px < 0 || px >= screen_w || py < 0 || py >= screen_h)
                        continue;

                    vga.paint(px, py, color);
                }
            }
        }
    }
}

static const unsigned char *get_char_sprite(char c)
{
    switch (c)
    {
    case 'A':
        return sprite_A;
    case 'D':
        return sprite_D;
    case 'E':
        return sprite_E;
    case 'G':
        return sprite_G;
    case 'K':
        return sprite_K; 
    case 'L':
        return sprite_L;
    case 'N':
        return sprite_N;
    case 'O':
        return sprite_O;
    case 'P':
        return sprite_P;
    case 'R':
        return sprite_R;
    case 'S':
        return sprite_S;
    case 'U':
        return sprite_U;
    case 'X':
        return sprite_X;
    case 'Y':
        return sprite_Y;
    default:
        return nullptr;
    }
}

void draw_text(
    EcranBochs &vga,
    const char *text,
    int x,
    int y,
    int scale,
    ui8_t color  // default color is white 15
)
{
    int cursor_x = x;

    for (int i = 0; text[i] != '\0'; i++)
    {
        char c = text[i];

        if (c == ' ')
        {
            cursor_x += SPRITE_C_W * scale;
            continue;
        }

        const unsigned char *sprite = get_char_sprite(c);
        if (!sprite)
            continue;

        draw_sprite_scaled(
            vga,
            sprite,
            SPRITE_C_W,
            SPRITE_C_H,
            cursor_x,
            y,
            scale,
            color
        );

        cursor_x += SPRITE_C_W * scale;
    }
}

void draw_tile(
    EcranBochs &vga,
    const unsigned char *sprite,
    int x,
    int y,
    int w,
    int h,
    int sprite_w,
    int sprite_h,
    int scale
) {
    int tile_w = sprite_w * scale;
    int tile_h = sprite_h * scale;

    int tiles_x = (w + tile_w - 1) / tile_w;
    int tiles_y = (h + tile_h - 1) / tile_h;

    for (int ty = 0; ty < tiles_y; ty++) {
        for (int tx = 0; tx < tiles_x; tx++) {

            int draw_x = x + tx * tile_w;
            int draw_y = y + ty * tile_h;

            draw_sprite_scaled(
                vga,
                sprite,
                sprite_w,
                sprite_h,
                draw_x,
                draw_y,
                scale
            );
        }
    }
}
