#include "animations.h"

//=========GENERAL============//

LightDot* myLightDots[N_LEDS];
uint8_t nLightDots = 0;

void updateLightDots()
{
    for (int i = 0; i < nLightDots; i++) {
        if (myLightDots[i]->update() == false) {
            deleteLightDot(i);
        }
    }
}

void startNewLightDot(uint8_t channel, uint16_t duration, RGBWColor16 color, float fadeInTime, float fadeOutTime)
{
    if (nLightDots >= N_LEDS) {
        Serial.println("ERROR: more LightDots than lights!");
        return;
    }
    myLightDots[nLightDots] = new LightDot(channel, duration, color, fadeInTime, fadeOutTime);
    Serial.println("created a new LightDot! with index: " + String(nLightDots));
    nLightDots++;
    Serial.println("Number of LightDots: " + String(nLightDots));
}

void deleteLightDot(uint8_t index)
{
    if (index < 0 || index >= nLightDots) {
        Serial.println("ERROR: cannot delete LightDot. Index out of range! index: " + String(index));
        return;
    }

    Serial.println("deleting LightDot with index: " + String(index));
    myLightDots[index]->shutDown();

    // shift all the remaining instances to the lowest indexes:
    for (int i = index; i < nLightDots - 1; i++) {
        // myLightDots[i] = myLightDots[i + 1];
        memcpy(myLightDots[i], myLightDots[i + 1], sizeof(LightDot));
    }
    // lower the amoiunt of LightDots by 1
    nLightDots--;
    // delete the object of the last LightDot:
    delete myLightDots[nLightDots];
    myLightDots[nLightDots] = NULL;
}

LightDot::LightDot(uint8_t _channel, uint16_t _duration, RGBWColor16 _color, float _fadeInTime, float _fadeOutTime)
{
    startTime = millis();
    duration = _duration;
    channel = _channel;
    color = _color;
    fadeInTime = _fadeInTime;
    fadeOutTime = _fadeOutTime;
}

bool LightDot::update()
{
    uint16_t animationTime = millis() - startTime;
    if (animationTime < duration) {
        if (animationTime < int(fadeInTime)) {
            setLedTarget(channel, fadeColor(RGBWColor16(0, 0, 0, 0), color, float(animationTime) / fadeInTime));
        } else if (animationTime >= (duration - int(fadeOutTime))) {
            setLedTarget(channel, fadeColor(color, RGBWColor16(0, 0, 0, 0), (float(animationTime) - float(duration) + fadeOutTime) / fadeOutTime));
        } else {
            setLedTarget(channel, color);
        }
        return true; // LightDot needs to stay in memory
    }
    return false; // LightDot needs to be deleted
}

void LightDot::shutDown()
{
    setLedTarget(channel, RGBWColor16(0, 0, 0, 0));
}

//============JUMPING LIGHT==========//

//==============RAINDROPS========///
