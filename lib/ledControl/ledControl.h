#pragma once

#include "pinout.h"
#include <TLC5947.h>

// hardware setup:
#define N_LEDS 6
#define LED_RESOLUTION 16

// define a struct that can be used to pass the parameters to the animation tasks:
struct animationStruct {
    RGBWColor primaryColor;
    RGBWColor secondaryColor;
    uint16_t fadeTime;
};

void setupLeds();

void setAllLedsTo(RGBWColor color);

void setLedTarget(uint8_t index, RGBWColor color);

void updateLeds();

RGBWColor remapColor(RGBWColor color);

void updateLeds();

void startFadeAnimation(RGBWColor color, uint16_t fadeTime);

void updateFade(void * parameter);

RGBWColor dimColor(RGBWColor color, float brightness);

RGBWColor fadeColor(RGBWColor startColor, RGBWColor endColor, float fadeProgress);

RGBWColor changeWhiteLevel(RGBWColor color, float saturationMultiplier);
