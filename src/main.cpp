#include <Arduino.h>

#include "button.h"
#include "dipswitches.h"
#include "ledControl.h"

// timing:
uint16_t delayTime = 500;

uint16_t brightness = 0;

unsigned long lastLedUpdate = 0;
unsigned long fadeTime = 1000000;

// timing and stating
unsigned long lastLedupdate = 0;
unsigned int ledUpdateInterval = 20;
unsigned long lastDipswitchUpdate = 0;
unsigned int dipswitchUpdateInterval = 100;
uint8_t currentState = 0;
uint8_t address = 0;

// button initialization:
Button button(BUTTON_N, activeLow);

// function declarations:
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
}

void loop()
{
    // read the dipswitches periodically:
    if (millis() - lastDipswitchUpdate >= dipswitchUpdateInterval) {
        lastDipswitchUpdate = millis();
        address = getAddress();
        currentState = getConfig();
    }

    // update the leds periodically:
    if (millis() - lastLedUpdate >= ledUpdateInterval) {
        lastLedUpdate = millis();

        // // play the rotating animation based on the dip switch settings:
        // RGBWColor16 foregroundColor = getColorFromDipswitch(getConfig);
        // RGBWColor16 backgroundColor = getColorFromDipswitch(getAddrress);
        // updateTwoColorRotationAnimation(foregroundColor, backgroundColor, 2000, 10);

        switch (currentState) {
        case 1:
            setAllLedsTo(RGBWColor16(4095, 0, 0, 0));
            break;
        case 2:
            setAllLedsTo(RGBWColor16(0, 4095, 0, 0));
            break;
        case 3:
            setAllLedsTo(RGBWColor16(0, 0, 4095, 0));
            break;
        case 4:
            setAllLedsTo(RGBWColor16(0, 0, 0, 4095));
            break;
        default:
            //"off" state, no lights
            setAllLedsTo(RGBWColor16(0, 0, 0, 0));
            break;
        }
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
