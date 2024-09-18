#pragma once
#include "ledControl.h"
#include <Arduino.h>

//=====================GENERAL=============================//
#define N_NOLAS 4

void updateLightDots();

void startNewLightDot(uint8_t channel, uint16_t duration, RGBWColor16 color, float fadeInTime = 0, float fadeOutTime = 0);

void deleteLightDot(uint8_t index);

class LightDot {
private:
    uint32_t startTime;
    uint16_t duration;
    uint8_t channel;
    RGBWColor16 color;
    float fadeInTime;
    float fadeOutTime;

public:
    LightDot(uint8_t _channel, uint16_t _duration, RGBWColor16 _color, float _fadeInTime, float _fadeOutTime);
    bool update(); // returns false if it needs to be deleted
    void shutDown();
};

//============================JUMPING LIGHT=====================//
#define JUMPING_LIGHT_ANIMATION_LENGTH 27

#define JUMPING_LIGHT_LENGTH 5000
#define JUMPING_LIGHT_LONG_FADE 4000
#define JUMPING_LIGHT_SHORT_FADE 200

#define JUMPING_LIGHT_COLOR RGBWColor16(0,2000,4095,1000)

const int jumpingLightQueues[N_NOLAS][JUMPING_LIGHT_ANIMATION_LENGTH] = {
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0 }
};

//===============RAINDROP ANIMATION================================//
#define RAINDROP_ANIMATION_LENGTH 30

#define RAINDROP_DROP_LENGTH 1000
#define RAINDROP_COLOR RGBWColor16(0, 0, 0, 4095)
#define RAINDROP_FADE_IN 200
#define RAINDROP_FADE_OUT 500

const uint8_t rainDropQueues[N_NOLAS][RAINDROP_ANIMATION_LENGTH] = {
    { 1, 0, 0, 0, 4, 0, 0, 0, 0, 2, 0, 0, 6, 0, 0, 4, 0, 0, 0, 3, 0, 0, 5, 0, 2, 0, 0, 0, 4, 0 },
    { 0, 0, 6, 0, 0, 4, 0, 0, 1, 0, 3, 0, 0, 6, 0, 0, 5, 0, 3, 0, 0, 2, 0, 0, 0, 0, 0, 6, 0, 0 },
    { 0, 5, 0, 0, 0, 0, 0, 6, 0, 0, 0, 2, 0, 0, 3, 0, 0, 0, 0, 0, 6, 0, 0, 5, 0, 2, 0, 0, 0, 4 },
    { 0, 0, 0, 4, 0, 0, 2, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 5, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0 }
};
