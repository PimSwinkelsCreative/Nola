#include "ledControl.h"

#include <math.h>

#define DEBUG_ANIMATIONS

// led data:
RGBWColor16 leds[N_LEDS];

// create the TLC5947 object:
// The led array needs to be initialized prior to this object creation
TLC5947 ledDriver(leds, N_LEDS, LED_SCLK, LED_SIN, LED_LATCH, LED_BLANK);

uint16_t maxColorValue;

RGBWColor16 remapColor(RGBWColor16 color)
{
    return RGBWColor16(color.w, color.b, color.g, color.r);
}

void setupLeds()
{
    maxColorValue = (1<<LED_RESOLUTION)-1;
    ledDriver.setAllLedsTo(RGBWColor16(0, 0, 0, 0));
    ledDriver.update();
}

void setAllLedsTo(RGBWColor16 color)
{
    color = correctColor(color);
    color = remapColor(color);
    ledDriver.setAllLedsTo(color);
}

void setLedTarget(uint8_t index, RGBWColor16 color)
{
    color = correctColor(color);
    color = remapColor(color);
    ledDriver.setLedTo(index, color);
}

void updateLeds() { ledDriver.update(); }

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

RGBWColor16 fadeColor(RGBWColor16 startColor, RGBWColor16 endColor,
    float fadeProgress)
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
    uint16_t RGBWHiteValueToSubtract = constrain(
        uint16_t(float(RGBWhiteLevel) * saturationMultiplier), 0, (1 << 12) - 1);

    RGBWColor16 outputColor;

    outputColor.r -= RGBWHiteValueToSubtract;
    outputColor.g -= RGBWHiteValueToSubtract;
    outputColor.b -= RGBWHiteValueToSubtract;
    outputColor.w = constrain(uint16_t(float(color.w) * saturationMultiplier), 0,
        (1 << 12) - 1);

    return outputColor;
}

RGBWColor16 correctColor(RGBWColor16 color)
{
    int power = 2;

    RGBWColor16 outputColor;
    double red, green, blue, white;
    red = double(color.r) / 4095.0;
    green = double(color.g) / 4095.0;
    blue = double(color.b) / 4095.0;
    white = double(color.w) / 4095.0;

    red = pow(red, power);
    green = pow(green, power);
    blue = pow(blue, power);
    white = pow(white, power);

    red *= 4095.0;
    green *= 4095.0;
    blue *= 4095.0;
    white *= 4095.0;

    color.r = constrain((uint16_t)red,0,maxColorValue);
    color.g = constrain((uint16_t)green,0,maxColorValue);
    color.b = constrain((uint16_t)blue,0,maxColorValue);
    color.w = constrain((uint16_t)white,0,maxColorValue);

    return color;
}

void updateTwoColorRotationAnimation(RGBWColor16 foregroundColor, RGBWColor16 backgroundColor, float animationDuration, float rotationAmount)
{
    RGBWColor16 outputColors[N_LEDS];

    float animationDirection = 10 * sin(2 * PI * float(millis()) / (animationDuration * 2.8));
    Serial.print(animationDirection);
    for (int i = 0; i < N_LEDS; i++) {
        float channelOffset = (animationDirection) + float(i) / float(N_LEDS);
        if (channelOffset > 1.0)
            channelOffset -= 1.0;
        if (channelOffset < 0)
            channelOffset += 1.0;

        float fadeProgress = 1 + cos(2.0 * PI * (0.5 + 0.5 * channelOffset));
        outputColors[i] = fadeColor(backgroundColor, foregroundColor, fadeProgress);
        setLedTarget(i, outputColors[i]);

#ifdef DEBUG_ANIMATIONS
        if (i == 0) {
            Serial.print("\tfadeProgress: " + String(fadeProgress));
            Serial.print("\tR: " + String(outputColors[i].r) + "G: " + String(outputColors[i].g) + "B: " + String(outputColors[i].b) + "W: " + String(outputColors[i].w));
            Serial.println();
        }
#endif
    }
}
