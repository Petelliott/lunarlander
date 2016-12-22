/*
    Peter Elliott

    lunar lander
    lander.cpp
*/
#include "lander.h"


/*
    gets a randomish seed from RAND_PIN
    used only by init_lander, and not globaly accesable
*/
int seed(int n) {
    int out = 0;
    for (int i = 0; i < n; ++i) {
        delay(10);
        out |= analogRead(RAND_PIN) & (1<<i);
    }
    return out;
}


/*
    produce a lander random x and vel_x
*/
lander_t init_lander() {
    randomSeed(seed(16));

    int r = random(-50, 50); // r is the value from which x and vel_x are calculated

    Serial.print("playing start: ");
    Serial.println(r);

    int x = TFT_WIDTH/2 + r;
    float vel_x = -r / (float)1500;

    return (lander_t) {
        x, 20,    // x, y
        vel_x, 0, // vel_x, vel_y
        0,        // angle
        0.0,      // thrust
        999,      //fuel
        false     // on
    };
}


/*
    set the control values according to their input devices
*/
void get_inputs(lander_t* lander) {
    lander->angle = (int) analogRead(ANGLE_PIN) / ((float)1024/9);
    lander->thrust = analogRead(THROTTLE_PIN) / (float)1024;
    lander->on = !digitalRead(BUTTON_PIN);
}


/*
    update position and speed of lander
    G: acceleration of gravity on the lander
    A: maximum acceleration of thrust
    FUEL_USE: self explanitory
*/
void update_lander(lander_t* lander, float G, float A, float FUEL_USE) {
    lander->x += lander->vel_x * TIMESTEP;
    lander->y += lander->vel_y * TIMESTEP;

    lander->x = fmod(lander->x+TFT_WIDTH, TFT_WIDTH); // allow lander wraparound

    if (lander->on && lander->fuel > 0) {
        lander->vel_x += lander->thrust * TIMESTEP * -COS_TABLE[lander->angle] * A;
        lander->vel_y += lander->thrust * TIMESTEP * -SIN_TABLE[lander->angle] * A;
        lander->fuel -= lander->thrust * TIMESTEP * FUEL_USE;
    }

    lander->vel_y += G * TIMESTEP;
}


/*
    returns true if angle and manhattan velocity are safe for landing
*/
bool safe_to_land(lander_t* lander) {
    return lander->angle == 4 && abs(lander->vel_x) + abs(lander->vel_y) < 0.0055;
}
