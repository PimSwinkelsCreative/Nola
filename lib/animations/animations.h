#pragma once
#include "ledControl.h"
#include <Arduino.h>

void updateRaindrops();

void startNewRaindrop(uint8_t channel, uint16_t duration, RGBWColor16 color);

void deleteRaindrop(uint8_t index);

class Raindrop {
private:
    uint32_t startTime;
    uint16_t duration;
    uint8_t channel;
    RGBWColor16 color;
    float fadeInTime;
    float fadeOutTime;

public:
    Raindrop(uint8_t _channel, uint16_t _duration, RGBWColor16 _color);
    bool update();  //returns false if it needs to be deleted
    void shutDown();
};