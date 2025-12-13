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
    ui8_t light
)
{
    for (int y = 0; y < sprite_h; y++)
    {
        for (int x = 0; x < sprite_w; x++)
        {
            ui8_t color = sprite[y * sprite_w + x];

            if (color == 0)
                continue; // skip transparent pixels

            if (light != 15)
                color = 15 - color + light;
  
            for (int dy = 0; dy < scale; dy++)
            {
                for (int dx = 0; dx < scale; dx++)
                {
                    vga.paint(
                        x0 + x * scale + dx,
                        y0 + y * scale + dy,
                        color);
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
