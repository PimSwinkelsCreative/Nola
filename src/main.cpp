#include <Arduino.h>
#include <dipswitches.h>

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
unsigned int ledUpdateInterval = 100;

void setup()
{

    delay(3000);
    // start the serial monitor
    Serial.begin(115200);

    // setup the leds
    setupLeds();

    setAllLedsTo(RGBWColor({ 65535, 0, 0, 0 }));
    updateLeds();
    delay(5000);

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
        uint8_t colorSettings = getConfig();
        RGBWColor outputColor;
        // outputColor = RGBWColor({ (colorSettings & 0b11) << 14, (colorSettings & 0b1100) << 12, (colorSettings & 0b110000) << 10, (colorSettings & 0b11000000) << 8 });
        outputColor.r = uint16_t(colorSettings & 0b11) << 14;
        outputColor.g = uint16_t(colorSettings & 0b1100) << 12;
        outputColor.b = uint16_t(colorSettings & 0b110000) << 10;
        outputColor.w = uint16_t(colorSettings & 0b11000000) << 8;
  
        // Serial.println("R: " + String(outputColor.r) + "\tG: " + String(outputColor.g) + "\tB: " + String(outputColor.b) + "\tW: " + String(outputColor.w));

        setAllLedsTo(RGBWColor({1000,outputColor.g,outputColor.b,outputColor.w}));

        updateLeds();
    }
}
