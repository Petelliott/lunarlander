/*
    Peter Elliott

    lunar lander
    pincnf.h
*/
#ifndef PINCNF_H
#define PINCNF_H

// standard U of A library settings, assuming Atmel Mega SPI pins
#define SD_CS    5  // Chip select line for SD card
#define TFT_CS   6  // Chip select line for TFT display
#define TFT_DC   7  // Data/command line for TFT
#define TFT_RST  8  // Reset line for TFT (or connect to +5V)

#define TFT_WIDTH 128
#define TFT_HEIGHT 160


#define THROTTLE_PIN 3 // analog
#define ANGLE_PIN 2    // analog
#define BUTTON_PIN 13  // digital

#define RAND_PIN 12    // analog

#endif
