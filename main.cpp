/*
    Peter Elliott

    lunar lander
    main.cpp
*/
#include <Arduino.h>

#include "draw.h"
#include "lander.h"


const float G = 0.000005;   // pixels per millisecond^2
const float A = 0.00005;    // pixels per millisecond^2
const float FUEL_USE = 0.4; // fuel units per millisecond


void looptime(int step);


int main() {
    init();
    Serial.begin(9600);

    pinMode(BUTTON_PIN, INPUT);
    digitalWrite(BUTTON_PIN, HIGH);

    draw_init();

    lander_t lander = init_lander();

    while(true) {
        looptime(TIMESTEP);

        get_inputs(&lander);
        update_lander(&lander, G, A, FUEL_USE);

        redraw_lander(&lander);
        redraw_data(&lander);

        if (lander.y+11 >= TFT_HEIGHT-FLOOR) { // collision with the surface
            break;
        }
    }

    finish(&lander);

    Serial.end();
    return 0;
}


/*
    enforce strict loop times and alert when lag occurs
    the contents of this function are modified from and in-class example
*/
void looptime(int step) {
    static int startTime = millis()-step; //initiated such that no delay will happen on the first invocation

    int curTime = millis();
    if (curTime-startTime < step) {
        delay(step - (curTime-startTime));
    } else if(curTime-startTime > step) { // dont do anything if the time is perfect
        Serial.print("lagging behind ");
        Serial.print(curTime-startTime-step);
        Serial.println(" milliseconds");
    }

    startTime = millis();
}
