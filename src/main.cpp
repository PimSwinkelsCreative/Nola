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
unsigned long lastAnimationUpdate = 0;
unsigned int animationUpdateInterval = 1000;
uint8_t currentState = 0;
uint8_t prevState = 0;
uint8_t address = 0;
uint8_t prevAddress = 0;
uint8_t animationCounter = 0;

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

        // reset the animation if the config is changed
        if (currentState != prevState) {
            prevState = currentState;
            animationCounter = 0;
        }
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
            // SHY LIGHTS

            break;
        case 2:
            // ANXIOUS

            break;
        case 3:
            // JUMPING LIGHTS
            if (millis() - lastAnimationUpdate >= animationUpdateInterval) {
                lastAnimationUpdate = millis();
                if (jumpingLightQueues[address][animationCounter] != 0) {
                    if (jumpingLightQueues[address][animationCounter] == 1) {
                        for (int i = 0; i < N_LEDS; i++) {
                            startNewLightDot(i, JUMPING_LIGHT_LENGTH, JUMPING_LIGHT_COLOR, JUMPING_LIGHT_LONG_FADE, JUMPING_LIGHT_SHORT_FADE);
                        }

                    } else if (jumpingLightQueues[address][animationCounter] == -1) {
                        for (int i = 0; i < N_LEDS; i++) {
                            startNewLightDot(i, JUMPING_LIGHT_LENGTH, JUMPING_LIGHT_COLOR, JUMPING_LIGHT_SHORT_FADE, JUMPING_LIGHT_LONG_FADE);
                        }
                    }
                }
                animationCounter++;
                animationCounter %= JUMPING_LIGHT_ANIMATION_LENGTH;
            }
            updateLightDots();
            break;
        case 4:
            // RAIN
            if (millis() - lastAnimationUpdate >= animationUpdateInterval) {
                lastAnimationUpdate = millis();
                if (animationCounter < RAINDROP_ANIMATION_LENGTH) {
                    if (rainDropQueues[address][animationCounter] > 0) {
                        startNewLightDot(rainDropQueues[address][animationCounter] - 1, RAINDROP_DROP_LENGTH, RAINDROP_COLOR, RAINDROP_FADE_IN, RAINDROP_FADE_OUT);
                    }
                    animationCounter++;
                }
                // animation should not loop
            }
            updateLightDots();
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
