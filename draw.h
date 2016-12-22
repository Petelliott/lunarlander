/*
    Peter Elliott

    lunar lander
    draw.h
*/
#ifndef DRAW_H
#define DRAW_H

#include "lander.h"
#include "pincnf.h"

void draw_init();

void redraw_lander(lander_t* lander);
void draw_lander(lander_t* lander, uint16_t color);

void redraw_data(lander_t* lander);

void finish(lander_t* lander);
void draw_flag(lander_t* lander);
void draw_crater(lander_t* lander);

#endif
