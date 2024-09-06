#pragma once

#include "pinout.h"
#include <TLC5947.h>

// hardware setup:
#define N_LEDS 6
#define LED_RESOLUTION 16

void setupLeds();

void setAllLedsTo(RGBWColor16 color);

void setLedTarget(uint8_t index, RGBWColor16 color);

void updateLeds();

RGBWColor16 remapColor(RGBWColor16 color);

void updateLeds();

RGBWColor16 dimColor(RGBWColor16 color, float brightness);

RGBWColor16 fadeColor(RGBWColor16 startColor, RGBWColor16 endColor, float fadeProgress);

RGBWColor16 changeWhiteLevel(RGBWColor16 color, float saturationMultiplier);
