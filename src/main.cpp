#include <Arduino.h>

#include "animations.h"
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
uint8_t prevState = 0;
uint8_t address = 0;
uint8_t prevAddress = 0;

// button initialization:
Button button(BUTTON_N, activeLow);

// function declarations:
RGBWColor16 getColorFromDipswitch(uint8_t (*dipswitchFunction)());

void setup() {
  delay(3000);
  // start the serial monitor
  Serial.begin(115200);

  // setup the leds
  setupLeds();

  // setup the dipswitches
  setupDipswitches();

  // reset all animations so they will start from 0
  resetAllAnimations();
}

void loop() {
  // read the dipswitches periodically:
  if (millis() - lastDipswitchUpdate >= dipswitchUpdateInterval) {
    lastDipswitchUpdate = millis();
    address = getAddress();
    currentState = getConfig();

    // reset the animation if the config is changed
    if (currentState != prevState) {
      prevState = currentState;

      resetAllAnimations();
    }
  }

  // update the leds periodically:
  if (millis() - lastLedUpdate >= ledUpdateInterval) {
    lastLedUpdate = millis();

    switch (currentState) {
      case 1:
        // SHY LIGHTS
        // updateHeartBeat(RGBWColor16(4000,4000,4000,0),0,40,1);
        updateShyLight(address);
        break;
      case 2:
        // ANXIOUS
        // TBD
        break;
      case 3:
        // JUMPING LIGHTS
        updateJumpingLights(address);
        break;
      case 4:
        // RAIN
        updateRaindrops(address);
        break;
      default:
        //"off" state, no lights
        setAllLedsTo(RGBWColor16(0, 0, 0, 0));
        break;
    }
    updateLeds();
  }
}

RGBWColor16 getColorFromDipswitch(uint8_t (*dipswitchFunction)()) {
  RGBWColor16 outputColor;
  uint8_t colorSettings = (*dipswitchFunction)();
  outputColor.r = uint16_t((colorSettings & 0b11) << 10);
  outputColor.g = uint16_t((colorSettings & 0b1100) << 8);
  outputColor.b = uint16_t((colorSettings & 0b110000) << 6);
  outputColor.w = uint16_t((colorSettings & 0b11000000) << 4);
  return outputColor;
}
