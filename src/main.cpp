#include <Arduino.h>

#include "dipswitches.h"
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
unsigned int ledUpdateInterval = 20;

void setup() {
  delay(3000);
  // start the serial monitor
  Serial.begin(115200);

  // setup the leds
  setupLeds();

  // setAllLedsTo(RGBWColor16( 65535, 0, 0, 0 ));
  // updateLeds();
  // delay(5000);

  // setup the dipswitches
  setupDipswitches();

  // get the configuration and address info
  config = getConfig();
  address = getAddrress();
}

void loop() {
  if (millis() - lastLedUpdate > ledUpdateInterval) {
    lastLedUpdate = millis();

    RGBWColor16 foregroundColor = RGBWColor16(0, 0, 0, 0);
    uint8_t foregroundColorSettings = getConfig();
    foregroundColor.r = uint16_t((foregroundColorSettings & 0b11) << 10);
    foregroundColor.g = uint16_t((foregroundColorSettings & 0b1100) << 8);
    foregroundColor.b = uint16_t((foregroundColorSettings & 0b110000) << 6);
    foregroundColor.w = uint16_t((foregroundColorSettings & 0b11000000) << 4);

    RGBWColor16 backgroundColor = RGBWColor16(0, 0, 0, 0);
    uint8_t backgroundColorSettings = getAddrress();
    backgroundColor.r = uint16_t((backgroundColorSettings & 0b11) << 10);
    backgroundColor.g = uint16_t((backgroundColorSettings & 0b1100) << 8);
    backgroundColor.b = uint16_t((backgroundColorSettings & 0b110000) << 6);
    backgroundColor.w = uint16_t((backgroundColorSettings & 0b11000000) << 4);

    RGBWColor16 outputColors[N_LEDS];

    const float animationDuration = 2000.0;
    const int animationSize = N_LEDS;
    float animationDirection =
        10*sin(2 * PI * float(millis()) / (animationDuration * 2.8));
    Serial.print(animationDirection);
    for (int i = 0; i < N_LEDS; i++) {
      float channelOffset =
          (animationDirection) + float(i) / float(animationSize);
      if (channelOffset > 1.0) channelOffset -= 1.0;
      if (channelOffset < 0) channelOffset += 1.0;

      float fadeProgress =
          1 + cos(2.0 * PI * (0.5 + 0.5 * channelOffset));
      outputColors[i] =
          fadeColor(backgroundColor, foregroundColor, fadeProgress);
      setLedTarget(i, outputColors[i]);

      if (i == 0) {
        Serial.print("\tfadeProgress: " + String(fadeProgress));
        Serial.print("\tR: " + String(outputColors[i].r) +
                     "G: " + String(outputColors[i].g) +
                     "B: " + String(outputColors[i].b) +
                     "W: " + String(outputColors[i].w));
      }
    }

    updateLeds();
    Serial.println();
  }
}
