/*
    Peter Elliott

    lunar lander
    draw.cpp
*/
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "precalc.h"
#include "draw.h"


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


/*
    setup the display
*/
void draw_init() {
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST7735_BLACK);

    tft.drawFastHLine( // draw the surface
        0,
        TFT_HEIGHT - FLOOR,
        TFT_WIDTH,
        ST7735_WHITE
    );
    tft.fillRect( // initialize the safety light
        TFT_WIDTH-6,
        TFT_HEIGHT-6,
        TFT_WIDTH,
        TFT_HEIGHT,
        ST7735_RED
    );
    tft.setTextColor(ST7735_WHITE, ST7735_BLACK); // make sure the text colors are right
}


/*
    remove the last lander and draw the new one
*/
void redraw_lander(lander_t* lander) {
    static lander_t last = *lander;

    draw_lander(&last, ST7735_BLACK);
    draw_lander(lander, ST7735_WHITE);

    last = *lander;
}


/*
    rotate points of an array in the form {x, y, x, y, ...} around 0,0
    using the matrix multiplication:
    ┌              ┐┌ ┐
    │cos(θ) -sin(θ)││x│
    │sin(θ)  cos(θ)││y│
    └              ┘└ ┘
*/
void rot(const int* points, float* buf, int len, int n) {
    for (int i = 0; i < len; i += 2) {
        buf[i] =  points[i] * COS_TABLE[n] - points[i+1] * SIN_TABLE[n];
        buf[i+1] =  points[i] * SIN_TABLE[n] + points[i+1] * COS_TABLE[n];
    }
}


const int lander_vec[18] = {3, -3, 3, 3, 7, 3, 7, -3, 10, 5, 10, -5, 7, -3, 11, 0, 7, 3}; //the points needed to draw the lander


/*
    draw a lander in any color you want.
    usually white to draw, or black to clear.
*/
void draw_lander(lander_t* lander, uint16_t color) {

    float buf[18];
    rot(lander_vec, buf, 18, lander->angle);

    tft.drawCircle(lander->x, lander->y, 3, color); // draw the circle

    for (int i = 2; i < 10; i +=2) { //draw the lines of the box
        tft.drawLine(
            (int)(lander->x+buf[(i-2)%8]),
            (int)(lander->y+buf[(i-1)%8]),
            (int)(lander->x+buf[(i)%8]),
            (int)(lander->y+buf[(i+1)%8]),
            color
        );
    }
    for (int i = 4; i < 7; i+=2) { // draw the legs
        tft.drawLine(
            (int)(lander->x+buf[i]),
            (int)(lander->y+buf[i+1]),
            (int)(lander->x+buf[i+4]),
            (int)(lander->y+buf[i+5]),
            color
        );
    }

    if (lander->on && lander->fuel > 0) { // draw the engine plume
        tft.drawLine(
            (int)(lander->x+buf[12]),
            (int)(lander->y+buf[13]),
            (int)(lander->x+buf[14]),
            (int)(lander->y+buf[15]),
            color
        );
        tft.drawLine(
            (int)(lander->x+buf[14]),
            (int)(lander->y+buf[15]),
            (int)(lander->x+buf[16]),
            (int)(lander->y+buf[17]),
            color
        );
    }
}


/*
    draw throttle amount, fuel remaining, safe landing indicator
*/
void redraw_data(lander_t* lander) {
    static int last_throt = 0;
    static int last_fuel = 0;
    static bool last_safety = 1;

    // redraw throttle
    int int_throt = (int)(lander->thrust * (TFT_WIDTH/2));
    if (int_throt > last_throt) {
        tft.fillRect(
            last_throt,
            TFT_HEIGHT-6,
            int_throt-last_throt,
            TFT_HEIGHT,
            ST7735_WHITE
        );
    } else if (int_throt < last_throt) {
        tft.fillRect(
            int_throt,
            TFT_HEIGHT-6,
            last_throt-int_throt,
            TFT_HEIGHT,
            ST7735_BLACK
        );
    }
    last_throt = int_throt;


    // redraw fuel guage
    if ((int)lander->fuel != last_fuel) {
        tft.setCursor(TFT_WIDTH/2 + 10, TFT_HEIGHT - 7);

        if ((int)lander->fuel < 100) tft.print('0'); // poor man's format
        if ((int)lander->fuel < 10) tft.print('0');

        tft.print(max((int)lander->fuel, 0)); // never print negative fuel

        last_fuel = (int)lander->fuel;
    }

    // redraw safety indicator
    bool safe = safe_to_land(lander);
    if (safe && !last_safety) {
        tft.fillRect(
            TFT_WIDTH-6,
            TFT_HEIGHT-6,
            TFT_WIDTH,
            TFT_HEIGHT,
            ST7735_GREEN
        );
    } else if (!safe && last_safety) {
        tft.fillRect(
            TFT_WIDTH-6,
            TFT_HEIGHT-6,
            TFT_WIDTH,
            TFT_HEIGHT,
            ST7735_RED
        );
    }
    last_safety = safe;
}


/*
    prints a message for either vicory or defeat
    and call draw_flag()
*/
void finish(lander_t* lander) {
    tft.setCursor(0, 0);
    if (safe_to_land(lander)) {
        Serial.println("landed succesfully");

        draw_flag(lander);

        tft.setTextColor(ST7735_GREEN, ST7735_BLACK);
        tft.print("The eagle has landed");
    } else {
        Serial.println("crash landed");

        draw_lander(lander, ST7735_BLACK);
        draw_crater(lander);

        tft.setTextColor(ST7735_RED, ST7735_BLACK);
        tft.print("Houston, we've had a problem");
    }
}


/*
    draw the american flag next to the lander
*/
void draw_flag(lander_t* lander) {
    for (int i = 0; i < 7; ++i) { //red and white lines
        tft.drawFastHLine(
            (int)(lander->x+12),
            (int)(lander->y-4+i),
            11,
            (i%2==0)? ST7735_RED : ST7735_WHITE
        );
    }

    tft.fillRect( //blue rectangle
        (int)(lander->x+12),
        (int)(lander->y-4),
        5, 4, ST7735_BLUE
    );

    tft.drawFastVLine( //flagpole
        (int)(lander->x+11),
        (int)(lander->y-4),
        15, ST7735_WHITE
    );
}


const int crater_dat[8] = {0, -5, 6, -3, 12, 2, 15, 0}; // vector points in format {x, y, x, y, ...}


/*
    draw a crater where the lander was
    called on defeat
*/
void draw_crater(lander_t* lander) {
    tft.drawFastHLine( // remove the surface where the crater is
        (int)lander->x - 15,
        TFT_HEIGHT-FLOOR,
        30,
        ST7735_BLACK
    );

    for (int i = 3; i < 8; i += 2) { // draw the crater from the middle outwards
        tft.drawLine(
            (int)lander->x+crater_dat[i-3],
            TFT_HEIGHT-25-crater_dat[i-2],
            (int)lander->x+crater_dat[i-1],
            TFT_HEIGHT-25-crater_dat[i],
            ST7735_WHITE
        );
        tft.drawLine(
            (int)lander->x-crater_dat[i-3],
            TFT_HEIGHT-25-crater_dat[i-2],
            (int)lander->x-crater_dat[i-1],
            TFT_HEIGHT-25-crater_dat[i],
            ST7735_WHITE
        );
    }
}
