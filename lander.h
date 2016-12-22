/*
    Peter Elliott

    lunar lander
    lander.h
*/
#ifndef LANDER_H
#define LANDER_H

#include <Arduino.h>
#include "precalc.h"
#include "pincnf.h"

#define TIMESTEP 35 //milliseconds
#define FLOOR 25    //pixels


typedef struct {  // 27 bytes total
    float x;      // pixels
    float y;      // pixels

    float vel_x;  // pixels per millisecond
    float vel_y;  // pixels per millisecond

    int angle;    // 0-8, π/8 rad

    float thrust; // 0-1*A
    float fuel;   // Arbitrary fuel units

    bool on;
} lander_t;


lander_t init_lander();

void get_inputs(lander_t* lander);
void update_lander(lander_t* lander, float G, float A, float FUEL_USE);

bool safe_to_land(lander_t* lander);

#endif
