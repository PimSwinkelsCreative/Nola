#include "animations.h"

//=========GENERAL============//

uint32_t animationStartTime = 0;

LightDot* myLightDots[N_LEDS];
uint8_t nLightDots = 0;

void resetAllAnimations() {
  animationStartTime = millis();  // reset the clock

  // delete All LightDots
  for (int i = 0; i < nLightDots; i++) {
    deleteLightDot(i);
  }
}

void updateHeartBeat(RGBWColor16 color, uint8_t channel, float bpm,
                     float intensity) {
  intensity = constrain(intensity, 0, 1);

  uint16_t heartBeatPeriod = uint16_t(60000.0 / bpm);
  uint16_t animationTime = millis() % heartBeatPeriod;

  RGBWColor16 baseColor =
      fadeColor(RGBWColor16(0, 0, 0, 0), color, HEARTBEAT_BASE_INTENSITY);
  RGBWColor16 peakColor =
      fadeColor(RGBWColor16(0, 0, 0, 0), color, HEARTBEAT_PEAK_INTENSITY);
  uint16_t fadeTime =
      uint16_t(float(heartBeatPeriod) * HEARTBEAT_PULSE_DURATION);
  float fadePercentage = float(animationTime % fadeTime) / float(fadeTime);

  RGBWColor16 outputColor = baseColor;

  if (animationTime <= fadeTime ||
      (animationTime > 2 * fadeTime && animationTime <= 3 * fadeTime)) {
    // rising edges of the heartbeat:
    outputColor = fadeColor(baseColor, peakColor, fadePercentage);

  } else if ((animationTime > fadeTime && animationTime <= 2 * fadeTime) ||
             (animationTime > 3 * fadeTime && animationTime <= 4 * fadeTime)) {
    // falling edges of the heartbeat:
    outputColor = fadeColor(peakColor, baseColor, fadePercentage);
  }

  setLedTarget(channel,
               fadeColor(RGBWColor16(0, 0, 0, 0), outputColor, intensity));
}

void updateLightDots() {
  for (int i = 0; i < nLightDots; i++) {
    if (myLightDots[i]->update() == false) {
      deleteLightDot(i);
    }
  }
}

void startNewLightDot(uint8_t channel, uint16_t duration, RGBWColor16 color,
                      float fadeInTime, float fadeOutTime) {
  if (nLightDots >= N_LEDS) {
    Serial.println("ERROR: more LightDots than lights!");
    return;
  }
  myLightDots[nLightDots] =
      new LightDot(channel, duration, color, fadeInTime, fadeOutTime);
  Serial.println("created a new LightDot! with index: " + String(nLightDots));
  nLightDots++;
  Serial.println("Number of LightDots: " + String(nLightDots));
}

void deleteLightDot(uint8_t index) {
  if (index < 0 || index >= nLightDots) {
    Serial.println(
        "ERROR: cannot delete LightDot. Index out of range! index: " +
        String(index));
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

LightDot::LightDot(uint8_t _channel, uint16_t _duration, RGBWColor16 _color,
                   float _fadeInTime, float _fadeOutTime) {
  startTime = millis();
  duration = _duration;
  channel = _channel;
  color = _color;
  fadeInTime = _fadeInTime;
  fadeOutTime = _fadeOutTime;
}

bool LightDot::update() {
  uint16_t animationTime = millis() - startTime;
  if (animationTime < duration) {
    if (animationTime < int(fadeInTime)) {
      setLedTarget(channel, fadeColor(RGBWColor16(0, 0, 0, 0), color,
                                      float(animationTime) / fadeInTime));
    } else if (animationTime >= (duration - int(fadeOutTime))) {
      setLedTarget(channel, fadeColor(color, RGBWColor16(0, 0, 0, 0),
                                      (float(animationTime) - float(duration) +
                                       fadeOutTime) /
                                          fadeOutTime));
    } else {
      setLedTarget(channel, color);
    }
    return true;  // LightDot needs to stay in memory
  }
  return false;  // LightDot needs to be deleted
}

void LightDot::shutDown() { setLedTarget(channel, RGBWColor16(0, 0, 0, 0)); }

//===========SHY LIGHT================//
void updateShyLight(uint8_t address) {
  uint16_t animationTime =
      (millis() - animationStartTime) % SHYLIGHTS_ANIMATION_TIME;
  if (animationTime < shyLightsFadeTimes[address][0]) {
    // handle the fadeIn
    float brightness =
        constrain(float(animationTime) / float(SHYLIGHTS_START_FADEIN), 0, 1);
    // determine if the orange color should be fully on
    uint8_t fullyOrangeChannels =
        animationTime / (shyLightsFadeTimes[address][0] / N_LEDS);
    for (int i = 0; i < N_LEDS; i++) {
      RGBWColor16 heartbeatColor = SHYLIHGTS_WHITE_COLOR;
      if (i <= fullyOrangeChannels) {
        heartbeatColor += SHYLIGHTS_ORANGE_COLOR;
      } else if (i == fullyOrangeChannels + 1) {
        // fade the specific channel
        float channelBrightness =
            float(animationTime % (shyLightsFadeTimes[address][0] / N_LEDS)) /
            float(shyLightsFadeTimes[address][0] / N_LEDS);
        RGBWColor16 colorToAdd = fadeColor(
            RGBWColor16(0, 0, 0, 0), SHYLIGHTS_ORANGE_COLOR, channelBrightness);
        heartbeatColor += colorToAdd;
      }
      updateHeartBeat(heartbeatColor, i, HEARTBEAT_BPM, brightness);
    }
  } else if (animationTime < (shyLightsFadeTimes[address][0] +
                              shyLightsFadeTimes[address][1])) {
    // handle the fadeOut, always have white and orange both on
    RGBWColor16 heartbeatColor = SHYLIHGTS_WHITE_COLOR;
    heartbeatColor += SHYLIGHTS_ORANGE_COLOR;
    float brightness =
        1.0 - float(animationTime - shyLightsFadeTimes[address][0]) /
                  float(shyLightsFadeTimes[address][1]);
    for (int i = 0; i < N_LEDS; i++) {
      updateHeartBeat(heartbeatColor, i, HEARTBEAT_BPM, brightness);
    }
  } else {
    setAllLedsTo(RGBWColor16(0, 0, 0, 0));
  }
}

//============JUMPING LIGHT==========//

//==============RAINDROPS========///
