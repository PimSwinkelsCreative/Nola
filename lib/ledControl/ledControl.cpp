#include "ledControl.h"

#include <math.h>

#define DEBUG_ANIMATIONS

// led data:
RGBWColor16 leds[N_LEDS];

// create the TLC5947 object:
// The led array needs to be initialized prior to this object creation
TLC5947 ledDriver(leds, N_LEDS, LED_SCLK, LED_SIN, LED_LATCH, LED_BLANK,
                  10000000, false);

uint16_t maxColorValue;

RGBWColor16 remapColor(RGBWColor16 color) {
  return RGBWColor16(color.w, color.b, color.g, color.r);
}

void setupLeds() {
  maxColorValue = (1 << LED_RESOLUTION) - 1;
  ledDriver.setAllLedsTo(RGBWColor16(0, 0, 0, 0));
  ledDriver.update();
}

void setAllLedsTo(RGBWColor16 color) {
//   color = correctColor(color);
  color = remapColor(color);
  ledDriver.setAllLedsTo(color);
}

void setLedTarget(uint8_t index, RGBWColor16 color) {
//   color = correctColor(color);
  color = remapColor(color);
  ledDriver.setLedTo(index, color);
}

void updateLeds(uint16_t brightness) {
  if (brightness < maxColorValue) {
    if (brightness > 0) {
      float dimRatio = float(brightness) / float(maxColorValue);
      for (int i = 0; i < N_LEDS; i++) {
        leds[i] = dimColor(leds[i], dimRatio);
        // leds[i] = correctColor(leds[i]);
      }
    } else {
      for (int i = 0; i < N_LEDS; i++) {
        leds[i] = RGBWColor16(0, 0, 0, 0);
      }
    }
  }
  ledDriver.update();
}

RGBWColor16 dimColor(RGBWColor16 color, float brightness) {
  brightness = constrain(brightness, 0, 1);
  RGBWColor16 outputColor;
  outputColor.r = uint16_t(float(color.r) * brightness);
  outputColor.g = uint16_t(float(color.g) * brightness);
  outputColor.b = uint16_t(float(color.b) * brightness);
  outputColor.w = uint16_t(float(color.w) * brightness);
  return outputColor;
}

RGBWColor16 fadeColor(RGBWColor16 startColor, RGBWColor16 endColor,
                      float fadeProgress) {
  fadeProgress = constrain(fadeProgress, 0, 1);
  RGBWColor16 outputColor;
  outputColor.r = uint16_t(float(startColor.r) * (1 - fadeProgress) +
                           float(endColor.r) * fadeProgress);
  outputColor.g = uint16_t(float(startColor.g) * (1 - fadeProgress) +
                           float(endColor.g) * fadeProgress);
  outputColor.b = uint16_t(float(startColor.b) * (1 - fadeProgress) +
                           float(endColor.b) * fadeProgress);
  outputColor.w = uint16_t(float(startColor.w) * (1 - fadeProgress) +
                           float(endColor.w) * fadeProgress);
  return outputColor;
}

RGBWColor16 changeWhiteLevel(RGBWColor16 color, float saturationMultiplier) {
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

uint16_t correctBrightness(uint16_t brightness) {
  int power = 2;
  double outBrightness = double(brightness) / 4095.0;
  outBrightness = pow(outBrightness, power);
  outBrightness *= 4095.0;
  return (uint16_t)constrain(outBrightness, 0, maxColorValue);
}

RGBWColor16 correctColor(RGBWColor16 color) {
  color.r = correctBrightness(color.r);
  color.g = correctBrightness(color.g);
  color.b = correctBrightness(color.b);
  color.w = correctBrightness(color.w);

  return color;
}
