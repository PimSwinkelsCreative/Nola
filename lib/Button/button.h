#ifndef MY_BUTTON_H
#define MY_BUTTON_H
#include <Arduino.h>

#define LONGPRESSTIME 2000
#define DEBOUNCETIME 50

enum buttonPolarity { activeHigh,
    activeLow,
    activeHigh_pulldown,
    activeLow_pullup
};

class Button {
private:
    byte pin;
    byte state;
    byte prevState;
    byte lastReading;
    buttonPolarity btnPolarity;
    bool invertInput;
    bool flag;
    bool longPressFlag;
    bool buttonReleasedSinceLastPress = true;
    unsigned long lastButtonPressTime = 0;
    unsigned long lastButtonChangeTime = 0;

public:
    Button(byte pin, buttonPolarity = activeLow_pullup);
    void init();
    void update();
    byte getState();
    bool isPressed();
    bool checkFlag();
    bool checkLongPressFlag();
    void clearFlag();
    void clearLongPressFlag();
};
#endif