#pragma once
#include "ledControl.h"
#include <Arduino.h>

#define N_NOLAS 4

#define RAINDROP_ANIMATION_LENGTH 30

const uint8_t rainDropQueues[N_NOLAS][RAINDROP_ANIMATION_LENGTH] = {
    { 1, 0, 0, 0, 4, 0, 0, 0, 0, 2, 0, 0, 6, 0, 0, 4, 0, 0, 0, 3, 0, 0, 5, 0, 2, 0, 0, 0, 4, 0 },
    { 0, 0, 6, 0, 0, 4, 0, 0, 1, 0, 3, 0, 0, 6, 0, 0, 5, 0, 3, 0, 0, 2, 0, 0, 0, 0, 0, 6, 0, 0 },
    { 0, 5, 0, 0, 0, 0, 0, 6, 0, 0, 0, 2, 0, 0, 3, 0, 0, 0, 0, 0, 6, 0, 0, 5, 0, 2, 0, 0, 0, 4 },
    { 0, 0, 0, 4, 0, 0, 2, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 5, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0 }
};

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
    bool update(); // returns false if it needs to be deleted
    void shutDown();
};