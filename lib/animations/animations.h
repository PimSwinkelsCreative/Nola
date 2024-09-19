#pragma once
#include <Arduino.h>

#include "ledControl.h"

//=====================GENERAL=============================//
#define N_NOLAS 4

void resetAllAnimations();

void updateHeartBeat(RGBWColor16 color, uint8_t channel, float bpm,
                     float intensity);

void updateLightDots();

void startNewLightDot(uint8_t channel, uint16_t duration, RGBWColor16 color,
                      float fadeInTime = 0, float fadeOutTime = 0);

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
  LightDot(uint8_t _channel, uint16_t _duration, RGBWColor16 _color,
           float _fadeInTime, float _fadeOutTime);
  bool update();  // returns false if it needs to be deleted
  void shutDown();
};

//======================SHY LIGHTS==================//

#define HEARTBEAT_BPM 30
#define HEARTBEAT_BASE_INTENSITY 0.4
#define HEARTBEAT_PEAK_INTENSITY 0.65
#define HEARTBEAT_PULSE_DURATION 0.14  // cannot be higher than 0.25!

#define SHYLIGHTS_ANIMATION_TIME 28000
#define SHYLIGHTS_START_FADEIN 500

#define SHYLIHGTS_WHITE_COLOR RGBWColor16(2000, 2000, 2000, 0)
#define SHYLIGHTS_ORANGE_COLOR RGBWColor16(4000, 4000, 0, 1000)

const uint16_t shyLightsFadeTimes[N_NOLAS][2] = {
    {8000, 10000}, {14000, 8000}, {18000, 6000}, {22000, 6000}};

void updateShyLight(uint8_t address);

//============================JUMPING LIGHT=====================//
#define JUMPING_LIGHT_ANIMATION_LENGTH 27

#define JUMPING_LIGHT_LENGTH 5000
#define JUMPING_LIGHT_LONG_FADE 4000
#define JUMPING_LIGHT_SHORT_FADE 200

#define JUMPING_LIGHT_COLOR RGBWColor16(0, 2000, 4095, 1000)

const int jumpingLightQueues[N_NOLAS][JUMPING_LIGHT_ANIMATION_LENGTH] = {
    {1, 0, 0, 0, 0, 0,  0, 0, 0, 1, 0, 0, 0, 0,
     0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0},
    {0,  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0}};

void updateJumpingLights(uint8_t address);

//===============RAINDROP ANIMATION================================//
#define RAINDROP_ANIMATION_LENGTH 30

#define RAINDROP_DROP_LENGTH 1000
#define RAINDROP_COLOR RGBWColor16(0, 0, 0, 1500)
#define RAINDROP_FADE_IN 50
#define RAINDROP_FADE_OUT 500

const uint8_t rainDropQueues[N_NOLAS][RAINDROP_ANIMATION_LENGTH] = {
    {1, 0, 0, 0, 4, 0, 0, 0, 0, 2, 0, 0, 6, 0, 0,
     4, 0, 0, 0, 3, 0, 0, 5, 0, 2, 0, 0, 0, 4, 0},
    {0, 0, 6, 0, 0, 4, 0, 0, 1, 0, 3, 0, 0, 6, 0,
     0, 5, 0, 3, 0, 0, 2, 0, 0, 0, 0, 0, 6, 0, 0},
    {0, 5, 0, 0, 0, 0, 0, 6, 0, 0, 0, 2, 0, 0, 3,
     0, 0, 0, 0, 0, 6, 0, 0, 5, 0, 2, 0, 0, 0, 4},
    {0, 0, 0, 4, 0, 0, 2, 0, 0, 1, 0, 0, 0, 3, 0,
     0, 0, 5, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0}};

void updateRaindrops(uint8_t address);

//============BREATHING=============//

#define BREATHING_ANIMATION_LENGTH 30
#define BREATHING_FADE_IN_LENGTH 2000
#define BREATHING_FADE_OUT_LENGTH 2000

#define BREATHING_BASE_INTENSITY 0.25
#define BREATHING_PEAK_INTENSITY 0.9

#define BREATHING_COLOR RGBWColor16(4095, 3000, 0, 1000)

const uint8_t breathingQueues[N_NOLAS][BREATHING_ANIMATION_LENGTH] = {
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

void updateBreathingAnimation(uint8_t address);
