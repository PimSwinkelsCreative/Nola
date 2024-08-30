#include "ledControl.h"
#include <math.h>

// led data:
RGBWColor leds[N_LEDS];

// create the TLC5947 object:
// The led array needs to be initialized prior to this object creation
TLC5947 ledDriver(leds, N_LEDS, LED_SCLK, LED_SIN, LED_LATCH, LED_BLANK);

uint8_t framesPerSecond = 25;

uint8_t currentAnimation = 0;

// the taskhandles for all animations:
TaskHandle_t updateFadeTask;

RGBWColor remapColor(RGBWColor color)
{
    return RGBWColor({ color.w, color.b, color.g, color.r });
}

void setupLeds()
{
    ledDriver.setAllLedsTo(RGBWColor({ 0, 0, 0, 0 }));
    ledDriver.update();
    ledDriver.update(); // for some reason update needs to be done twice??
}

void setAllLedsTo(RGBWColor color)
{
    Serial.println("R: "+String(color.r)+"\tG: "+String(color.g)+"\tB: "+String(color.b)+"\tW: "+String(color.w));
    ledDriver.setAllLedsTo(remapColor(color));
}

void setLedTarget(uint8_t index, RGBWColor color)
{
    ledDriver.setLedTo(index, remapColor(color));
}

void updateLeds()
{
    ledDriver.update();
}

void startFadeAnimation(RGBWColor color, uint16_t fadeTime)
{
    currentAnimation = 1;

    static animationStruct fadeInfo;
    fadeInfo.primaryColor = color;
    fadeInfo.fadeTime = fadeTime;

    xTaskCreatePinnedToCore(
        updateFade, /* Function to implement the task */
        "updateFadeTask", /* Name of the task */
        10000, /* Stack size in words */
        (void*) &fadeInfo, /* Task input parameter */
        0, /* Priority of the task */
        &updateFadeTask, /* Task handle. */
        0); /* Core where the task should run */
}

void updateFade(void * parameter)
{
    animationStruct *fadeInfo = (animationStruct*)parameter;

    RGBWColor fadeColor = fadeInfo->primaryColor;
    uint16_t fadeTime = fadeInfo->fadeTime;
    unsigned long animationStartTime = millis();
    unsigned long lastFrame = millis();
    while (currentAnimation == 1) {
        if (millis() - lastFrame >= (1000 / framesPerSecond)) {
            lastFrame = millis();
            float brightness;
            uint16_t currentFadeTime = (millis() - animationStartTime) % fadeTime;
            if (currentFadeTime < fadeTime / 2) {
                brightness = 2.0 * float(currentFadeTime) / float(fadeTime);
            } else {
                brightness = 2.0 * float(fadeTime - currentFadeTime) / float(fadeTime);
            }
            setAllLedsTo(dimColor(fadeColor, brightness));
            updateLeds();
            vTaskDelay(1);
        }
    }
    setAllLedsTo(RGBWColor({ 0, 0, 0, 0 }));
    updateLeds();
    vTaskDelete(NULL); // terminate and delete the task
}

RGBWColor dimColor(RGBWColor color, float brightness)
{
    brightness = constrain(brightness, 0, 1);
    RGBWColor outputColor;
    outputColor.r = uint16_t(float(color.r) * brightness);
    outputColor.g = uint16_t(float(color.g) * brightness);
    outputColor.b = uint16_t(float(color.b) * brightness);
    outputColor.w = uint16_t(float(color.w) * brightness);
    return outputColor;
}

RGBWColor fadeColor(RGBWColor startColor, RGBWColor endColor, float fadeProgress)
{
    fadeProgress = constrain(fadeProgress, 0, 1);
    RGBWColor outputColor;
    outputColor.r = uint16_t(float(startColor.r) * (1 - fadeProgress) + float(endColor.r) * fadeProgress);
    outputColor.g = uint16_t(float(startColor.g) * (1 - fadeProgress) + float(endColor.g) * fadeProgress);
    outputColor.b = uint16_t(float(startColor.b) * (1 - fadeProgress) + float(endColor.b) * fadeProgress);
    outputColor.w = uint16_t(float(startColor.w) * (1 - fadeProgress) + float(endColor.w) * fadeProgress);
    return outputColor;
}

RGBWColor changeWhiteLevel(RGBWColor color, float saturationMultiplier)
{
    // determine whitelevel
    uint16_t RGBWhiteLevel = min(min(color.r, color.g), color.b);
    uint16_t RGBWHiteValueToSubtract = constrain(uint16_t(float(RGBWhiteLevel) * saturationMultiplier), 0, (1 << 16) - 1);

    RGBWColor outputColor;

    outputColor.r -= RGBWHiteValueToSubtract;
    outputColor.g -= RGBWHiteValueToSubtract;
    outputColor.b -= RGBWHiteValueToSubtract;
    outputColor.w = constrain(uint16_t(float(color.w) * saturationMultiplier), 0, (1 << 16) - 1);

    return outputColor;
}
