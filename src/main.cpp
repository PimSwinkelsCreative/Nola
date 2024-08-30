#include "ledControl.h"
#include <Arduino.h>
#include <dipswitches.h>

// timing:
uint16_t delayTime = 500;

uint16_t brightness = 0;

unsigned long lastLedUpdate = 0;
unsigned long fadeTime = 1000000;

// configuration
uint8_t config, address;

void setup()
{
    // start the serial monitor
    Serial.begin(115200);

    // setup the leds
    setupLeds();

    // setup the dipswitches
    setupDipswitches();

    // get the configuration and address info
    config = getConfig();
    address = getAddrress();

    //start the fade animation in white
    startFadeAnimation(RGBWColor({0,0,0,65535}),5000);
}

void loop()
{
    // for(int i=0;i<1000;i++){
    //     setAllLedsTo(dimColor(RGBWColor({0,0,0,65535}),float(i)/1000.0));
    //     updateLeds();
    //     delay(10);
    // }

}
