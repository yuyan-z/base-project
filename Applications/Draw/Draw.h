#ifndef DRAW_H
#define DRAW_H

#include <sextant/types.h> // ui8_t

class EcranBochs; // Forward declaration

void draw_rect(
    EcranBochs &vga,
    int x0, int y0,
    int w, int h,
    ui8_t color);

void draw_sprite_scaled(
    EcranBochs &vga,
    const unsigned char *sprite,
    int sprite_w,
    int sprite_h,
    int x0,
    int y0,
    int scale,
    ui8_t light
);

void draw_text(
    EcranBochs &vga,
    const char *text,
    int x,
    int y,
    int scale,
    ui8_t color);

#endif
