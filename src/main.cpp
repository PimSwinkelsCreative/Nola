#include <Arduino.h>

#include "dipswitches.h"
#include "ledControl.h"

// timing:
uint16_t delayTime = 500;

uint16_t brightness = 0;

unsigned long lastLedUpdate = 0;
unsigned long fadeTime = 1000000;

// configuration
uint8_t config, address;

// timing and stating
unsigned long lastLedupdate = 0;
unsigned int ledUpdateInterval = 20;


//function declarations:
RGBWColor16 getColorFromDipswitch(uint8_t (*dipswitchFunction)());

void setup()
{
    delay(3000);
    // start the serial monitor
    Serial.begin(115200);

    // setup the leds
    setupLeds();

    // setAllLedsTo(RGBWColor16( 65535, 0, 0, 0 ));
    // updateLeds();
    // delay(5000);

    // setup the dipswitches
    setupDipswitches();

    // get the configuration and address info
    config = getConfig();
    address = getAddrress();
}

void loop()
{
    if (millis() - lastLedUpdate > ledUpdateInterval) {
        lastLedUpdate = millis();

        // play the rotating animation based on the dip switch settings:
        RGBWColor16 foregroundColor = getColorFromDipswitch(getConfig);
        RGBWColor16 backgroundColor = getColorFromDipswitch(getAddrress);
        updateTwoColorRotationAnimation(foregroundColor, backgroundColor, 2000, 10);

        updateLeds();

    }
}

RGBWColor16 getColorFromDipswitch(uint8_t (*dipswitchFunction)())
{
    RGBWColor16 outputColor;
    uint8_t colorSettings = (*dipswitchFunction)();
    outputColor.r = uint16_t((colorSettings & 0b11) << 10);
    outputColor.g = uint16_t((colorSettings & 0b1100) << 8);
    outputColor.b = uint16_t((colorSettings & 0b110000) << 6);
    outputColor.w = uint16_t((colorSettings & 0b11000000) << 4);
    return outputColor;
}
