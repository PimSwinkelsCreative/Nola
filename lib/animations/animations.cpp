#include "animations.h"






Raindrop* myRaindrops[N_LEDS];
uint8_t nRaindrops = 0;

void updateRaindrops()
{
    for (int i = 0; i < nRaindrops; i++) {
        if (myRaindrops[i]->update() == false) {
            deleteRaindrop(i);
        }
    }
}

void startNewRaindrop(uint8_t channel, uint16_t duration, RGBWColor16 color)
{
    if (nRaindrops >= N_LEDS) {
        Serial.println("ERROR: more raindrops than lights!");
        return;
    }
    myRaindrops[nRaindrops] = new Raindrop(channel, duration, color);
    Serial.println("created a new raindrop! with index: " + String(nRaindrops));
    nRaindrops++;
    Serial.println("Number of raindrops: " + String(nRaindrops));
}

void deleteRaindrop(uint8_t index)
{
    if (index < 0 || index >= nRaindrops) {
        Serial.println("ERROR: cannot delete raindrop. Index out of range! index: " + String(index));
        return;
    }

    Serial.println("deleting raindrop with index: " + String(index));
    myRaindrops[index]->shutDown();

    // shift all the remaining instances to the lowest indexes:
    for (int i = index; i < nRaindrops - 1; i++) {
        // myRaindrops[i] = myRaindrops[i + 1];
        memcpy(myRaindrops[i],myRaindrops[i+1],sizeof(Raindrop));
    }
    // lower the amoiunt of raindrops by 1
    nRaindrops--;
    // delete the object of the last raindrop:
    delete myRaindrops[nRaindrops];
    myRaindrops[nRaindrops] = NULL;
}

Raindrop::Raindrop(uint8_t _channel, uint16_t _duration, RGBWColor16 _color)
{
    startTime = millis();
    duration = _duration;
    channel = _channel;
    color = _color;
    fadeInTime = float(duration) * 0.1;
    fadeOutTime = float(duration) * 0.5;
}

bool Raindrop::update()
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
        return true; // raindrop needs to stay in memory
    }
    return false; // raindrop needs to be deleted
}

void Raindrop::shutDown()
{
    setLedTarget(channel, RGBWColor16(0, 0, 0, 0));
}