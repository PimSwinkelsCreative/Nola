#include "Button.h"
Button::Button(byte pin, buttonPolarity polarity)
{
    this->pin = pin;
    btnPolarity = polarity;
    lastReading = LOW;
    invertInput = (btnPolarity == activeLow || btnPolarity == activeLow_pullup);
    init();
}
void Button::init()
{
    if (btnPolarity == activeLow || btnPolarity == activeHigh) {
        pinMode(pin, INPUT);
    } else if (btnPolarity == activeLow_pullup) {
        pinMode(pin, INPUT_PULLUP);
    } else {
        pinMode(pin, INPUT_PULLDOWN);
    }
    update();
}
void Button::update()
{
    // You can handle the debounce of the button directly
    // in the class, so you don't have to think about it
    // elsewhere in your code
    bool newReading;
    if (invertInput) {
        newReading = digitalRead(pin); // pin is active low by default
    }else{
      newReading = !digitalRead(pin);
    }

    if (newReading != lastReading) {
        lastButtonChangeTime = millis();
        if (newReading == LOW && lastReading == HIGH) {
            buttonReleasedSinceLastPress = true;
            lastButtonPressTime = millis();
        } else if (millis() - lastButtonPressTime > DEBOUNCETIME && millis() - lastButtonPressTime < LONGPRESSTIME) {
            flag = true;
        }
        lastReading = newReading;
    }

    if (state == LOW && millis() - lastButtonPressTime >= LONGPRESSTIME && buttonReleasedSinceLastPress) {
        longPressFlag = true;
        buttonReleasedSinceLastPress = false;
    }

    if (millis() - lastButtonChangeTime > DEBOUNCETIME) {
        // Update the 'state' attribute only if debounce is checked
        state = newReading;
        prevState = state;
    }
}

byte Button::getState()
{
    update();
    return state;
}
bool Button::isPressed() { return (getState() == LOW); }

bool Button::checkFlag()
{
    update();
    return flag;
}

bool Button::checkLongPressFlag()
{
    update();
    return longPressFlag;
}

void Button::clearFlag() { flag = false; }
void Button::clearLongPressFlag() { longPressFlag = false; }
