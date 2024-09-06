#include "ledControl.h"
#include <math.h>

// led data:
RGBWColor16 leds[N_LEDS];

// create the TLC5947 object:
// The led array needs to be initialized prior to this object creation
TLC5947 ledDriver(leds, N_LEDS, LED_SCLK, LED_SIN, LED_LATCH, LED_BLANK);

uint8_t framesPerSecond = 25;

uint8_t currentAnimation = 0;

// the taskhandles for all animations:
TaskHandle_t updateFadeTask;

RGBWColor16 remapColor(RGBWColor16 color)
{
    return RGBWColor16({ color.w, color.b, color.g, color.r });
}

void setupLeds()
{
    ledDriver.setAllLedsTo(RGBWColor16({ 0, 0, 0, 0 }));
    ledDriver.update();
    ledDriver.update(); // for some reason update needs to be done twice??
}

void setAllLedsTo(RGBWColor16 color)
{
    Serial.println("R: "+String(color.r)+"\tG: "+String(color.g)+"\tB: "+String(color.b)+"\tW: "+String(color.w));
    ledDriver.setAllLedsTo(remapColor(color));
}

void setLedTarget(uint8_t index, RGBWColor16 color)
{
    ledDriver.setLedTo(index, remapColor(color));
}

void updateLeds()
{
    ledDriver.update();
}

RGBWColor16 dimColor(RGBWColor16 color, float brightness)
{
    brightness = constrain(brightness, 0, 1);
    RGBWColor16 outputColor;
    outputColor.r = uint16_t(float(color.r) * brightness);
    outputColor.g = uint16_t(float(color.g) * brightness);
    outputColor.b = uint16_t(float(color.b) * brightness);
    outputColor.w = uint16_t(float(color.w) * brightness);
    return outputColor;
}

RGBWColor16 fadeColor(RGBWColor16 startColor, RGBWColor16 endColor, float fadeProgress)
{
    fadeProgress = constrain(fadeProgress, 0, 1);
    RGBWColor16 outputColor;
    outputColor.r = uint16_t(float(startColor.r) * (1 - fadeProgress) + float(endColor.r) * fadeProgress);
    outputColor.g = uint16_t(float(startColor.g) * (1 - fadeProgress) + float(endColor.g) * fadeProgress);
    outputColor.b = uint16_t(float(startColor.b) * (1 - fadeProgress) + float(endColor.b) * fadeProgress);
    outputColor.w = uint16_t(float(startColor.w) * (1 - fadeProgress) + float(endColor.w) * fadeProgress);
    return outputColor;
}

RGBWColor16 changeWhiteLevel(RGBWColor16 color, float saturationMultiplier)
{
    // determine whitelevel
    uint16_t RGBWhiteLevel = min(min(color.r, color.g), color.b);
    uint16_t RGBWHiteValueToSubtract = constrain(uint16_t(float(RGBWhiteLevel) * saturationMultiplier), 0, (1 << 16) - 1);

    RGBWColor16 outputColor;

    outputColor.r -= RGBWHiteValueToSubtract;
    outputColor.g -= RGBWHiteValueToSubtract;
    outputColor.b -= RGBWHiteValueToSubtract;
    outputColor.w = constrain(uint16_t(float(color.w) * saturationMultiplier), 0, (1 << 16) - 1);

    return outputColor;
}
